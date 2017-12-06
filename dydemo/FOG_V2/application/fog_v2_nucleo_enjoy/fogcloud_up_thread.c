#include "mico.h"
#include "fogcloud_up_thread.h"
#include "fog_v2_include.h"
#include "json_c/json_object.h"
#include "json_c/json.h"
#include "MiCOKit_STmems.h"
#include "user_common.h"
#include "template_analysis.h"
#include "user_template.h"


#define fogcloud_up_log(M, ...) custom_log("FOG_UP_DATA", M, ##__VA_ARGS__)

#ifndef MICO_SYSTEM_LOG_INFO_DISABLE
  #define system_log(M, ...) custom_log("SYSTEM", M, ##__VA_ARGS__)
  #define system_log_trace() custom_log_trace("SYSTEM")
#else
  #define system_log(M, ...)
  #define system_log_trace()
#endif


EXT_SENSOR_DATA     ext_sensor_data;

void user_upstream_thread(mico_thread_arg_t args);


//�����ʼ��
static OSStatus ext_module_init(void)
{
    OSStatus err = kNoErr;

    //��ʼ���ⲿ������
    err = hts221_sensor_init();             //��ʪ�ȴ�������ʼ��
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: Unable to Init DHT11") );

    //������ ������ǿ��
    light_sensor_init();

    //��̬��������ʼ��
    err = lsm9ds1_acc_gyr_sensor_init();
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: Unable to Init lsm9ds1") );

    //��ʼ�����
    err = dc_motor_init();
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: Unable to Init DC motor") );
    dc_motor_set(0);

    //��ʼ��rgb_led
    hsb2rgb_led_init();
//    hsb2rgb_led_open(240, 100, 100);

    fogcloud_up_log("module init success");

 exit:
    return err;
}

OSStatus ext_moudule_read(void)
{
    OSStatus err = kUnknownErr;

    memset(&ext_sensor_data, 0, sizeof(ext_sensor_data));

    //��ȡ��ʪ��
    err = hts221_Read_Data(&(ext_sensor_data.temperature), &(ext_sensor_data.humidity));
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: hts221_Read_Data()") );
    //fogcloud_up_log("DHT11  T: %3.1fC  H: %3.1f%%", (float)ext_sensor_data.temperature, (float)ext_sensor_data.humidity);

    light_sensor_read(&(ext_sensor_data.infrared_reflective));

    //��ȡ���ٶ�
    err = lsm9ds1_acc_read_data(&ext_sensor_data.ACC_X, &ext_sensor_data.ACC_Y, &ext_sensor_data.ACC_Z);
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: lsm9ds1_acc_read_data()") );

    //��ȡ���ٶ�
    err = lsm9ds1_gyr_read_data(&ext_sensor_data.GYR_X, &ext_sensor_data.GYR_Y, &ext_sensor_data.GYR_Z);
    require_noerr_action( err, exit, fogcloud_up_log("ERROR: lsm9ds1_acc_read_data()") );


    micokit_set_oled((uint8_t)(ext_sensor_data.temperature), (uint8_t)(ext_sensor_data.humidity));

 exit:
    return err;
}

OSStatus ext_moudule_upload(void)
{
    //static bool motor_state = false;
    char *upload_data = NULL;
    OSStatus err = kUnknownErr;
    uint32_t upload_data_len = 1024;

    upload_data = malloc(upload_data_len);
    if(upload_data == NULL)
    {
        return kNoMemoryErr;
    }
    memset(upload_data, 0, upload_data_len);


    //��ʼ���ϴ�����ģ��
    err = init_upload_data(1234, D2C_SEND_PACKETS);
    require_noerr(err, exit);

    //����¶�����
    err = add_float_peripherals_to_upload_data(USER_NODE_TYPE_TEMPERATURE ,USER_NODE_PID_TEMPERATURE, ext_sensor_data.temperature);
    require_noerr(err, exit);

    //���ʪ������
    err = add_float_peripherals_to_upload_data(USER_NODE_TYPE_HUMIDITY, USER_NODE_PID_HUMIDITY, ext_sensor_data.humidity);
    require_noerr(err, exit);

    //��Ӻ���������
    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_LIGHT_INTENSITY, USER_NODE_PID_LIGHT_INTENSITY, ext_sensor_data.infrared_reflective);
    require_noerr(err, exit);

    //��Ӽ��ٶ�
    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ACC_X, ext_sensor_data.ACC_X);
    require_noerr(err, exit);

    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ACC_Y, ext_sensor_data.ACC_Y);
    require_noerr(err, exit);

    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ACC_Z, ext_sensor_data.ACC_Z);
    require_noerr(err, exit);

    //��ӽ��ٶ�
    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ANG_X, ext_sensor_data.GYR_X);
    require_noerr(err, exit);

    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ANG_Y, ext_sensor_data.GYR_Y);
    require_noerr(err, exit);

    err = add_int_peripherals_to_upload_data(USER_NODE_TYPE_ACC_ANG, USER_NODE_PID_ANG_Z, ext_sensor_data.GYR_Z);
    require_noerr(err, exit);

    err = generate_final_upload_data(upload_data, upload_data_len);
    require_noerr(err, exit);

    fogcloud_up_log("[%d]%s", strlen(upload_data), upload_data);

    fog_v2_device_send_event(upload_data, FOG_V2_SEND_EVENT_RULES_PUBLISH);

 exit:
    if(upload_data != NULL)  //������Դ
    {
        free(upload_data);
        upload_data = NULL;
    }

    destory_upload_data(); //������Դ
    return err;
}

void user_upstream_thread(mico_thread_arg_t args)
{
	OSStatus err = kUnknownErr;

	fogcloud_up_log("------------upstream_thread start------------");

    err = ext_module_init();      //�����ʼ��
    require_noerr(err, exit);

	while(1)
	{
        mico_thread_msleep(1300);

        system_log("num_of_chunks:%d,allocted_memory:%d, free:%d, total_memory:%d", MicoGetMemoryInfo()->num_of_chunks, MicoGetMemoryInfo()->allocted_memory, MicoGetMemoryInfo()->free_memory, MicoGetMemoryInfo()->total_memory);

        if(fog_v2_is_have_superuser() == true)
        {
            if(ext_moudule_read() == kNoErr)
            {
                ext_moudule_upload();
            }
        }
	}

 exit:
	mico_rtos_delete_thread(NULL);  // delete current thread

	return;
}
