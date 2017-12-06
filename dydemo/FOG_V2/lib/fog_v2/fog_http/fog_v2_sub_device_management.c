#include "fog_v2_include.h"
#include "mico.h"
#include "fog_v2.h"
#include "string.h"

#define app_log(M, ...)                     custom_log("FOG_V2_SUB_MANAGEMENT", M, ##__VA_ARGS__)

#if (FOG_V2_USE_SUB_DEVICE == 1)

void init_subdevice_des( void ); //��ʼ������

uint32_t get_subdevice_local_available_num(void);
mico_queue_t *get_sub_device_queue_addr_by_index(uint32_t index);
bool get_sub_device_queue_is_available_by_index(uint32_t index);
char *get_sub_device_id_by_index(uint32_t index);
char *get_sub_device_commands_topic_by_index( uint32_t index );
char *get_sub_device_cmd_topic_by_index( uint32_t index );
char *get_sub_device_mac_by_index( uint32_t index );
char *get_sub_device_product_id_by_index( uint32_t index );
OSStatus fog_v2_get_sub_device_all_available(FOG_V2_SUBDEVICE_AVAILABLE_CB user_callbck);

bool get_sub_device_queue_usable_index(uint32_t *index);
bool get_sub_device_queue_index_by_mac(uint32_t *index, const char *s_product_id, const char *s_mac);
bool get_sub_device_queue_index_by_deviceid(uint32_t *index, const char *s_device_id);
bool get_sub_device_queue_index_by_mqtt_topic(uint32_t *index, const char *s_mqtt_topic);

bool sub_device_queue_get(const char *s_product_id, const char *s_mac, const char *s_device_id); //�������豸��Դ
bool sub_device_queue_put_by_mac(const char *s_product_id, const char *s_mac); //�ͷ����豸��Դ ͨ��mac��ַ
bool sub_device_queue_put_by_deviceid(const char *s_deviceid);
bool sub_device_queue_put_by_mqtt_topic(const char *s_mqtt_topic);

void get_sub_device_queue_index_info(uint32_t index);
void get_sub_device_queue_all_info(void);

bool fog_v2_golobal_des_is_init(void); //�ж�ȫ��ָ���Ƿ��ʼ��
OSStatus init_subdevice_des_mutex( ); //��ʼ����
OSStatus lock_subdevice_des_mutex( ); //��ȡ��
OSStatus unlock_subdevice_des_mutex( ); //�ͷ���

static bool release_subdevice( uint32_t index );
static void cli_get_sub_device_info(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
//----------------------------------------------------------------

struct cli_command mxchip_subdevice_cmd[] = {
    { "sub_info", "get one subdevice info", cli_get_sub_device_info}
};

//�ж�ȫ��ָ���Ƿ��ʼ��
bool fog_v2_golobal_des_is_init(void)
{
     if(get_fog_des_g() != NULL)
     {
         return true;
     }else
     {
         return false;
     }
}

//��ʼ����
OSStatus init_subdevice_des_mutex( )
{
    if(get_fog_des_g() != NULL)
    {
        return mico_rtos_init_mutex( &(get_fog_des_g()->sub_des_mutex));
    }else
    {
        return kGeneralErr;
    }
}

//������
OSStatus lock_subdevice_des_mutex( )
{
//    app_log("++++++");

    if(get_fog_des_g() != NULL)
    {
        return mico_rtos_lock_mutex( &(get_fog_des_g()->sub_des_mutex));
    }else
    {
        return kGeneralErr;
    }
}

//�ͷ���
OSStatus unlock_subdevice_des_mutex( )
{
//    app_log("------");

     if(get_fog_des_g() != NULL)
     {
        return mico_rtos_unlock_mutex( &(get_fog_des_g()->sub_des_mutex));
     }else
     {
        return kGeneralErr;
     }
}

//get subdevice num information
OSStatus fog_v2_get_subdevice_num_info(uint32_t *tatal, uint32_t *used)
{
    uint32_t used_count = 0;
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    *tatal = FOG_V2_SUB_DEVICE_MAX_NUM;

    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( get_sub_device_queue_is_available_by_index( i ) == true )
        {
            used_count ++;
        }
    }

    *used = used_count;

    return kGeneralErr;
}


//��ʼ�����豸����
void check_subdevice_password( void )
{
    uint32_t i = 0;

    lock_subdevice_des_mutex();

    if ( get_fog_des_g( )->sub_des_password != FOG_V2_SUB_DEVICE_DES_PASSWORD )
    {
        app_log("password error,clean subdevice des!!!");

        for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
        {
            memset( &(get_fog_des_g( )->sub_des[i]), 0, sizeof(FOG_SUB_DES_S) );
        }

        get_fog_des_g( )->sub_des_password = FOG_V2_SUB_DEVICE_DES_PASSWORD; //��������
        mico_system_context_update( mico_system_context_get( ) );
    } else
    {
        app_log("password ok!!!");
    }

    unlock_subdevice_des_mutex();

    return;
}

//��ȡ���豸������index�±�����豸�Ƿ���� ����ֵ�� true-����  false-������
bool get_sub_device_queue_is_available_by_index(uint32_t index)
{
    bool state = false;

    lock_subdevice_des_mutex(); //���� +++++++

    state = get_fog_des_g()->sub_des[index].s_is_activate;

    unlock_subdevice_des_mutex(); //���� ------

    return state;
}

void get_sub_device_queue_index_info(uint32_t index)
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return;
    }

    if(index >= FOG_V2_SUB_DEVICE_MAX_NUM)
    {
        app_log("index error!");
        return;
    }

    lock_subdevice_des_mutex(); //���� +++++++

    app_log("====================================");
    app_log("index:%ld", index);
    app_log("s_is_activate:%s", (get_fog_des_g()->sub_des[index].s_is_activate == TRUE) ? "true" : "flase");
    app_log("s_mqtt_topic_cmd: %s", (get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd == NULL) ? "null point" : get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd);
    app_log("s_mqtt_topic_commands: %s", (get_fog_des_g()->sub_des[index].s_mqtt_topic_commands == NULL) ? "null point" : get_fog_des_g()->sub_des[index].s_mqtt_topic_commands);
    app_log("s_device_mac:%s", (*(get_fog_des_g()->sub_des[index].s_device_mac) == 0) ? "nothing" : get_fog_des_g()->sub_des[index].s_device_mac);
    app_log("s_product_id:%s",  (*(get_fog_des_g()->sub_des[index].s_product_id) == 0) ? "nothing" : get_fog_des_g()->sub_des[index].s_product_id);
    app_log("s_device_id:%s", (*(get_fog_des_g()->sub_des[index].s_device_id) == 0) ? "nothing" : get_fog_des_g()->sub_des[index].s_device_id);
    app_log("====================================");

    unlock_subdevice_des_mutex(); //���� ------
}

void get_sub_device_queue_all_info(void)
{
    uint32_t i = 0;
    uint32_t count = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return;
    }

    for(i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i ++)
    {
        if(get_sub_device_queue_is_available_by_index(i) == true)
        {
            get_sub_device_queue_index_info(i);
            count ++;
        }else
        {
            app_log("index = %ld not active", i);
        }
    }

    app_log("total:%d, used:%ld", FOG_V2_SUB_DEVICE_MAX_NUM, count);

    return;
}

//get subdevice available list
OSStatus fog_v2_get_sub_device_all_available(FOG_V2_SUBDEVICE_AVAILABLE_CB user_callbck)
{
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false || user_callbck == NULL)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    for(i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i ++)
    {
        if(get_sub_device_queue_is_available_by_index(i) == true)
        {
            user_callbck(get_fog_des_g()->sub_des[i].s_product_id, get_fog_des_g()->sub_des[i].s_device_mac );
        }
    }

    return kNoErr;
}


//��ȡ��Ч�����豸����
uint32_t get_subdevice_local_available_num(void)
{
    uint32_t i = 0, count = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return -1;
    }

    lock_subdevice_des_mutex(); //���� +++++++

    for(i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i ++)
    {
        if(get_fog_des_g()->sub_des[i].s_is_activate == true)
        {
            count ++;
        }
    }

    unlock_subdevice_des_mutex(); //���� ------
    return count;
}

//��ȡ���豸�����еĿ����±� ����ֵ�� true-�ɹ�  false-ʧ��
bool get_sub_device_queue_usable_index(uint32_t *index)
{
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    require_string(index != NULL, exit, "param is error!");

    lock_subdevice_des_mutex(); //���� +++++++

    for(i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i ++)
    {
        if(get_fog_des_g()->sub_des[i].s_is_activate == false)
        {
            *index = i;
            unlock_subdevice_des_mutex(); //���� ------
            return true;
        }
    }

    unlock_subdevice_des_mutex(); //���� ------

    exit:
    return false;
}


//ͨ��index�õ�queue�ĵ�ַ
mico_queue_t *get_sub_device_queue_addr_by_index(uint32_t index)
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if(index >= FOG_V2_SUB_DEVICE_MAX_NUM)
        return NULL;

    lock_subdevice_des_mutex(); //���� +++++++
    if(get_fog_des_g()->sub_des[index].s_is_activate == true)
    {
        unlock_subdevice_des_mutex(); //���� ------
        return &(get_fog_des_g()->sub_des[index].queue);
    }else
    {
        unlock_subdevice_des_mutex(); //���� ------
        return NULL;
    }
}

//ͨ��index �õ�device id
char *get_sub_device_id_by_index( uint32_t index )
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if(index >= FOG_V2_SUB_DEVICE_MAX_NUM)
        return NULL;

    lock_subdevice_des_mutex(); //���� +++++++
    if ( get_fog_des_g( )->sub_des[index].s_is_activate == true )
    {
        unlock_subdevice_des_mutex(); //���� ------
        return get_fog_des_g( )->sub_des[index].s_device_id;
    } else
    {
        unlock_subdevice_des_mutex(); //���� ------
        return NULL;
    }
}

//ͨ��index �õ�mqtt commandsƵ��
char *get_sub_device_commands_topic_by_index( uint32_t index )
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if(index >= FOG_V2_SUB_DEVICE_MAX_NUM)
        return NULL;

    lock_subdevice_des_mutex(); //���� +++++++
    if ( get_fog_des_g( )->sub_des[index].s_is_activate == true )
    {
        unlock_subdevice_des_mutex(); //���� ------
        return get_fog_des_g( )->sub_des[index].s_mqtt_topic_commands;
    } else
    {
        unlock_subdevice_des_mutex(); //���� ------
        return NULL;
    }
}

//ͨ��index �õ�mqtt cmdƵ��
char *get_sub_device_cmd_topic_by_index( uint32_t index )
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if(index >= FOG_V2_SUB_DEVICE_MAX_NUM)
        return NULL;

    lock_subdevice_des_mutex(); //���� +++++++
    if ( get_fog_des_g( )->sub_des[index].s_is_activate == true )
    {
        unlock_subdevice_des_mutex(); //���� ------
        return get_fog_des_g( )->sub_des[index].s_mqtt_topic_cmd;
    } else
    {
        unlock_subdevice_des_mutex(); //���� ------
        return NULL;
    }
}

char *get_sub_device_mac_by_index( uint32_t index )
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if ( index >= FOG_V2_SUB_DEVICE_MAX_NUM )
        return NULL;

    lock_subdevice_des_mutex( ); //���� +++++++
    if ( get_fog_des_g( )->sub_des[index].s_is_activate == true )
    {
        unlock_subdevice_des_mutex( ); //���� ------
        return get_fog_des_g( )->sub_des[index].s_device_mac;
    } else
    {
        unlock_subdevice_des_mutex( ); //���� ------
        return NULL;
    }
}

//ͨ��index�õ���ƷID
char *get_sub_device_product_id_by_index( uint32_t index )
{
    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return NULL;
    }

    if ( index >= FOG_V2_SUB_DEVICE_MAX_NUM )
        return NULL;

    lock_subdevice_des_mutex( ); //���� +++++++
    if ( get_fog_des_g( )->sub_des[index].s_is_activate == true )
    {
        unlock_subdevice_des_mutex( ); //���� ------
        return get_fog_des_g( )->sub_des[index].s_product_id;
    } else
    {
        unlock_subdevice_des_mutex( ); //���� ------
        return NULL;
    }
}

//����sub device��mac��ַ��ȡqueue���±� s_mac��ҪΪ��д��ĸ ���ص���available��Ԫ��
bool get_sub_device_queue_index_by_mac(uint32_t *index, const char *s_product_id, const char *s_mac)
{
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(s_mac == NULL || s_product_id == NULL)
    {
        app_log("param error");
        return false;
    }

    lock_subdevice_des_mutex(); //���� +++++++
    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if((get_fog_des_g()->sub_des[i].s_device_mac != NULL) && (get_fog_des_g()->sub_des[i].s_is_activate == true))
        {
            if ( (strcmp( get_fog_des_g( )->sub_des[i].s_device_mac, s_mac ) == 0) && (strcmp( get_fog_des_g( )->sub_des[i].s_product_id, s_product_id ) == 0) )
            {
                *index = i;
                unlock_subdevice_des_mutex(); //���� ------
                return true;
            }
        }
    }

    unlock_subdevice_des_mutex(); //���� ------
    return false;
}

//����sub device��deviceid��ַ��ȡ�±�
bool get_sub_device_queue_index_by_deviceid(uint32_t *index, const char *s_device_id)
{
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(s_device_id == NULL)
    {
        return false;
    }

    lock_subdevice_des_mutex(); //���� +++++++
    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( (get_fog_des_g( )->sub_des[i].s_device_id != NULL) && (get_fog_des_g( )->sub_des[i].s_is_activate == true) )
        {
            if ( strcmp( get_fog_des_g( )->sub_des[i].s_device_id, s_device_id ) == 0 )
            {
                *index = i;
                unlock_subdevice_des_mutex( ); //���� ------
                return true;
            }
        }
    }

    unlock_subdevice_des_mutex(); //���� ------
    return false;
}

//����sub device��mqtt topic�������±�
bool get_sub_device_queue_index_by_mqtt_topic(uint32_t *index, const char *s_mqtt_topic)
{
    uint32_t i = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(s_mqtt_topic == NULL)
    {
        return false;
    }

    lock_subdevice_des_mutex(); //���� +++++++
    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( (get_fog_des_g()->sub_des[i].s_device_id != NULL) && (get_fog_des_g( )->sub_des[i].s_is_activate == true) )
        {
            if ( strstr( s_mqtt_topic, get_fog_des_g()->sub_des[i].s_device_id ) != NULL )
            {
                *index = i;
                app_log("index = %ld", *index);
                unlock_subdevice_des_mutex(); //���� ------
                return true;
            }
        }
    }

    unlock_subdevice_des_mutex(); //���� ------
    return false;
}


//�������豸��Դ
bool sub_device_queue_get(const char *s_product_id, const char *s_mac, const char *s_device_id)
{
    uint32_t index = 0;
    OSStatus err = kGeneralErr;
    mico_queue_t *queue_p = NULL;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if((s_mac == NULL) || (s_product_id == NULL) || (s_device_id == NULL) )
    {
        app_log("para error");
        return false;
    }

    if(strlen(s_product_id) >= sizeof(get_fog_des_g()->sub_des[index].s_product_id))
    {
        app_log("para s_product_id is too long");
        return false;
    }

    if(strlen(s_mac) >= sizeof(get_fog_des_g()->sub_des[index].s_device_mac))
    {
        app_log("para s_device_mac is too long");
        return false;
    }

    if(strlen(s_device_id) >= sizeof(get_fog_des_g()->sub_des[index].s_device_id))
    {
        app_log("para s_device_id is too long");
        return false;
    }

    if ( get_sub_device_queue_index_by_mac( &index, s_product_id, s_mac ) == false )
    {
        if ( get_sub_device_queue_usable_index( &index ) == false )
        {
            app_log("get_sub_device_queue_usable_index() is failed");
            return false;
        }
    }else
    {
        app_log("++++++++already active: product_id:%s, mac:%s++++++++", s_product_id, s_mac);
        return true; //����Ҫ�ٳ�ʼ��
    }

    lock_subdevice_des_mutex(); //���� +++++++

    memset(&(get_fog_des_g()->sub_des[index]), 0, sizeof(FOG_SUB_DES_S));    //��յ�ǰ���豸������Ϣ

    queue_p = &(get_fog_des_g()->sub_des[index].queue);

    err = mico_rtos_init_queue( queue_p, "subdevice mqtt queue", sizeof(SUBDEVICE_RECV_DATA_S *), SUB_DEVICE_MQTT_QUEUE_SIZE );
    require_noerr( err, exit );

    get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd = malloc(FOG_V2_SUB_DEVICE_MQTT_TOPIC_LEN);
    require_action(get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd != NULL, exit, err = kNoMemoryErr);

    get_fog_des_g()->sub_des[index].s_mqtt_topic_commands = malloc(FOG_V2_SUB_DEVICE_MQTT_TOPIC_LEN);
    require_action(get_fog_des_g()->sub_des[index].s_mqtt_topic_commands != NULL, exit, err = kNoMemoryErr);

    sprintf(get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd, "c2d/%s/cmd", s_device_id);
    sprintf(get_fog_des_g()->sub_des[index].s_mqtt_topic_commands, "c2d/%s/commands", s_device_id);

    strcpy(get_fog_des_g()->sub_des[index].s_product_id, s_product_id);
    strcpy(get_fog_des_g()->sub_des[index].s_device_mac, s_mac);
    strcpy(get_fog_des_g()->sub_des[index].s_device_id, s_device_id);

    get_fog_des_g()->sub_des[index].s_is_activate = true;

    app_log("init subdevice info, index = %ld", index);

  exit:
    if(err != kNoErr)
    {
        if(*queue_p != NULL)
        {
            mico_rtos_deinit_queue(queue_p);
        }

        if(get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd != NULL)
        {
            free(get_fog_des_g()->sub_des[index].s_mqtt_topic_cmd);
        }

        if(get_fog_des_g()->sub_des[index].s_mqtt_topic_commands != NULL)
        {
            free(get_fog_des_g()->sub_des[index].s_mqtt_topic_commands);
        }

        app_log("sub_device_queue_get() error!!");

        mico_system_context_update(mico_system_context_get());
        unlock_subdevice_des_mutex(); //���� ------
        return false;
    }else
    {
        mico_system_context_update(mico_system_context_get());
        unlock_subdevice_des_mutex(); //���� ------
        return true;
    }
}

//�ͷ����豸��Դ
static bool release_subdevice( uint32_t index )
{
    OSStatus err = kNoErr;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    lock_subdevice_des_mutex(); //���� +++++++
    //�ͷ���Դ
    if ( get_fog_des_g( )->sub_des[index].queue != NULL )
    {
        mico_rtos_deinit_queue( &(get_fog_des_g( )->sub_des[index].queue) );
    } else
    {
        err = kGeneralErr;
        app_log("[ERROR]queue is NULL");
    }

    if ( get_fog_des_g( )->sub_des[index].s_mqtt_topic_cmd != NULL )
    {
        free( get_fog_des_g( )->sub_des[index].s_mqtt_topic_cmd );
        get_fog_des_g( )->sub_des[index].s_mqtt_topic_cmd = NULL;
    } else
    {
        err = kGeneralErr;
        app_log("[ERROR]s_mqtt_topic_cmd is NULL");
    }

    if ( get_fog_des_g( )->sub_des[index].s_mqtt_topic_commands != NULL )
    {
        free( get_fog_des_g( )->sub_des[index].s_mqtt_topic_commands );
        get_fog_des_g( )->sub_des[index].s_mqtt_topic_commands = NULL;
    } else
    {
        err = kGeneralErr;
        app_log("[ERROR]s_mqtt_topic_commands is NULL");
    }

    memset( &(get_fog_des_g( )->sub_des[index]), 0, sizeof(FOG_SUB_DES_S) );

    get_fog_des_g( )->sub_des[index].s_is_activate = false;

    err = mico_system_context_update(mico_system_context_get());
    require_noerr( err, exit );

    err = kNoErr;
    exit:
    if(err != kNoErr)
    {
        unlock_subdevice_des_mutex(); //���� ------
        return false;
    }else
    {
        unlock_subdevice_des_mutex(); //���� ------
        return true;
    }
}


//ͨ��mac��ַ�ͷ����豸��Դ
bool sub_device_queue_put_by_mac(const char *s_product_id, const char *mac)
{
    uint32_t index = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(mac == NULL || s_product_id == NULL)
    {
        return false;
    }

    if(get_sub_device_queue_index_by_mac(&index, s_product_id, mac) == false)
    {
        return false;
    }

    app_log("release index = %ld", index);
    return release_subdevice(index);
}

//ͨ��deviceid��ַ�ͷ����豸��Դ
bool sub_device_queue_put_by_deviceid(const char *s_deviceid)
{
    uint32_t index = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(s_deviceid == NULL)
    {
        return false;
    }

    if(get_sub_device_queue_index_by_deviceid(&index, s_deviceid) == false)
    {
        return false;
    }

    return release_subdevice(index);
}

//ͨ��mqtt��topic��ַ�ͷ����豸��Դ
bool sub_device_queue_put_by_mqtt_topic(const char *s_mqtt_topic)
{
    uint32_t index = 0;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    if(s_mqtt_topic == NULL)
    {
        return false;
    }

    if(get_sub_device_queue_index_by_mqtt_topic(&index, s_mqtt_topic) == false)
    {
        return false;
    }

    return release_subdevice(index);
}

OSStatus fog_v2_subdevice_des_init_topic_queue( void )
{
    uint32_t i = 0;
    mico_queue_t *queue_p = NULL;
    OSStatus err = kGeneralErr;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return false;
    }

    lock_subdevice_des_mutex(); //���� +++++++

    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( get_fog_des_g( )->sub_des[i].s_is_activate == true )
        {
            if(strlen(get_fog_des_g( )->sub_des[i].s_device_id ) != FOG_V2_SUBDEVICEID_LEN ||
               strlen(get_fog_des_g( )->sub_des[i].s_product_id) != FOG_V2_SUBPRODUCT_ID_LEN ||
               strlen(get_fog_des_g( )->sub_des[i].s_device_mac) == 0 ||
               strlen(get_fog_des_g( )->sub_des[i].s_device_mac) >= sizeof(get_fog_des_g( )->sub_des[i].s_device_mac) )
            {
                app_log("[FATLT ERROR] find error subdevice info, i = %ld", i);
                memset(&(get_fog_des_g( )->sub_des[i]), 0, sizeof(FOG_SUB_DES_S));
                get_fog_des_g( )->sub_des[i].s_is_activate = false;
                continue;
            }

            queue_p = &(get_fog_des_g( )->sub_des[i].queue);

            err = mico_rtos_init_queue( queue_p, "subdevice mqtt queue", sizeof(SUBDEVICE_RECV_DATA_S *), SUB_DEVICE_MQTT_QUEUE_SIZE );
            require_action_string( err == kNoErr, exit, err = kNoMemoryErr, "mico_rtos_init_queue error");

            get_fog_des_g( )->sub_des[i].s_mqtt_topic_cmd = malloc( FOG_V2_SUB_DEVICE_MQTT_TOPIC_LEN );
            require_action( get_fog_des_g()->sub_des[i].s_mqtt_topic_cmd != NULL, exit, err = kNoMemoryErr );

            get_fog_des_g( )->sub_des[i].s_mqtt_topic_commands = malloc( FOG_V2_SUB_DEVICE_MQTT_TOPIC_LEN );
            if ( get_fog_des_g( )->sub_des[i].s_mqtt_topic_commands == NULL )
            {
                app_log("malloc error!");

                if ( get_fog_des_g( )->sub_des[i].s_mqtt_topic_cmd != NULL )
                {
                    free( get_fog_des_g( )->sub_des[i].s_mqtt_topic_cmd );
                }

                err = kNoMemoryErr;
                break;
            }

            sprintf( get_fog_des_g( )->sub_des[i].s_mqtt_topic_cmd, "c2d/%s/cmd", get_fog_des_g( )->sub_des[i].s_device_id );
            sprintf( get_fog_des_g( )->sub_des[i].s_mqtt_topic_commands, "c2d/%s/commands", get_fog_des_g( )->sub_des[i].s_device_id );

            app_log("index = %ld init queue ok", i);
        }
    }

    err = kNoErr;

    exit:
    unlock_subdevice_des_mutex(); //���� ------

    return err;
}

//���ĵ�ǰ������Ч���豸�� topic
OSStatus fog_v2_subdevice_des_add_topic( void )
{
    uint32_t i = 0;
    OSStatus err = kGeneralErr;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( get_fog_des_g( )->sub_des[i].s_is_activate == true )
        {
            err = add_mqtt_topic_by_mac( get_fog_des_g( )->sub_des[i].s_product_id, get_fog_des_g( )->sub_des[i].s_device_mac );
            require_noerr( err, exit );

            app_log("device add topic, product_id:%s, mac:%s", get_fog_des_g( )->sub_des[i].s_product_id, get_fog_des_g( )->sub_des[i].s_device_mac);
        }
    }

    err = kNoErr;

    exit:
    return err;
}

OSStatus fog_v2_subdevice_des_set_status( bool status )
{
    uint32_t i = 0;
    OSStatus err = kGeneralErr;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    for ( i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i++ )
    {
        if ( get_fog_des_g( )->sub_des[i].s_is_activate == true )
        {
            err = fog_v2_set_subdevice_status( get_fog_des_g( )->sub_des[i].s_product_id, get_fog_des_g( )->sub_des[i].s_device_mac, status);
            if(err == kNoErr)
            {
                app_log("device set %s, product_id:%s, mac:%s", (status == true) ? "online" : "offline" ,get_fog_des_g( )->sub_des[i].s_product_id, get_fog_des_g( )->sub_des[i].s_device_mac);
            }else
            {
                app_log("set device status error!");
            }
        }
    }

    return kNoErr;
}


OSStatus get_remote_subdevice_list( void )
{
    int i = 0, j = 0;
    OSStatus err = kGeneralErr;
    char *remote_device_list = NULL;
    const char *remote_subdevice_list_member = NULL, *local_subdevice_list_member = NULL;
    json_object *http_body_json_obj = NULL, *data_json_obj = NULL, *node_json_obj = NULL;
    int remote_subdevice_list_len = 0;
    bool get_http_response_status = false;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    remote_device_list = malloc(REMOTE_DEVICE_LIST_BUFF_LEN);
    require_action_string(remote_device_list != NULL, exit, err = kNoMemoryErr, "malloc error!");

    memset(remote_device_list, 0, REMOTE_DEVICE_LIST_BUFF_LEN);

    err = fog_v2_subdeice_get_list(remote_device_list, REMOTE_DEVICE_LIST_BUFF_LEN, &get_http_response_status);
    require_noerr_string(err, exit, "fog_v2_subdeice_get_list() error");

    require_string( get_http_response_status == true, exit, "get_http_response_status is error" );

    require_string( ((*remote_device_list == '{') && (*(remote_device_list + strlen( remote_device_list ) - 1) == '}')), exit, "JSON format error" );

    http_body_json_obj = json_tokener_parse( remote_device_list );
    if ( http_body_json_obj == NULL )
    {
        app_log("num_of_chunks:%d, free:%d", MicoGetMemoryInfo()->num_of_chunks, MicoGetMemoryInfo()->free_memory);
        app_log("remote_device_list:%s", remote_device_list);
        err = kGeneralErr;
        goto exit;
    }

    data_json_obj = json_object_object_get( http_body_json_obj, "data" );
    require_string( data_json_obj != NULL, exit, "get data error!" );

    remote_subdevice_list_len = json_object_array_length(data_json_obj);
    require_action_string(remote_subdevice_list_len >= 0, exit, err = kGeneralErr, "remote_subdevice_list_len is error");

    app_log("remote subdevice list len is %d", remote_subdevice_list_len);

    app_log("local subdevice list len is %ld", get_subdevice_local_available_num());

    for(i = 0; i < FOG_V2_SUB_DEVICE_MAX_NUM; i ++) //�����������нڵ�
    {
        if(get_sub_device_queue_is_available_by_index(i) == false)
        {
            continue;
        }

        for(j = 0; j < remote_subdevice_list_len; j ++)  //�����ƶ����нڵ�
        {
            node_json_obj = json_object_array_get_idx(data_json_obj, j);
            if(node_json_obj == NULL)
            {
                app_log("json_object_array_get_idx() error");
                continue;
            }

            remote_subdevice_list_member = json_object_get_string(node_json_obj);
            if((remote_subdevice_list_member == NULL))
            {
                app_log("json_object_get_string() error");
                continue;
            }

            local_subdevice_list_member = get_sub_device_id_by_index(i);
            if(local_subdevice_list_member == NULL)
            {
                app_log("[ERROR]local_subdevice_list_member is NULL");
                continue;
            }

            if(strcmp(local_subdevice_list_member, remote_subdevice_list_member) == 0)
            {
                app_log("[SUCCESS]compare success, deviceid:%s, product_id: %s, mac:%s", local_subdevice_list_member, \
                        get_sub_device_product_id_by_index(i), get_sub_device_mac_by_index(i));
                break;
            }
        }

        if(j == remote_subdevice_list_len) //û���ҵ�ƥ��ɹ���  ɾ��������Դ
        {
            push_cmd_to_subdevice_queue(MQTT_CMD_SUB_UNBIND, get_sub_device_id_by_index(i));

            //release_subdevice(i);
        }
    }

    err = kNoErr;
 exit:
    if(remote_device_list != NULL)
    {
        free(remote_device_list);
        remote_device_list = NULL;
    }

    if ( http_body_json_obj != NULL )
    {
        json_object_put( http_body_json_obj );
        http_body_json_obj = NULL;
    }

    return err;
}

static void cli_get_sub_device_info(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t index = 0;

    if (argc == 1)
    {
        get_sub_device_queue_all_info();

    } else if(argc == 2)
    {
        index = atoi(argv[1]);
        get_sub_device_queue_index_info(index);
    }else
    {
        app_log("argc is error");
    }

    return;
}



void subdevice_add_cli(void)
{
    cli_register_commands(mxchip_subdevice_cmd, sizeof(mxchip_subdevice_cmd)/sizeof(struct cli_command));

    return;
}

//�ϵ�ʱ��ʼ��fog_des
OSStatus fog_v2_subdevice_des_init(void)
{
    OSStatus err = kGeneralErr;

    if(fog_v2_golobal_des_is_init() == false)
    {
        app_log("subdevice not init!");
        return kGeneralErr;
    }

    app_log("\r\n@@@@@@@@@@@@@@ sub device des init start @@@@@@@@@@@@@\r\n");

    //��ʼ����
    init_subdevice_des_mutex();

    //�������
    check_subdevice_password();

    //������̨����߳�
    start_gateway_bind_monitor();

    //�Ա��ص����豸��Ϣ��ʼ��
    err = fog_v2_subdevice_des_init_topic_queue();
    require_noerr(err, exit);

    //���ƶ˵��б��������� ���ͱ����б����Ա�
    get_remote_subdevice_list();

    //��������״̬(��ѡ��Ŀ)
    err = fog_v2_subdevice_des_set_status(false);
    require_noerr(err, exit);

    //����������cli
    subdevice_add_cli();

    app_log("\r\n@@@@@@@@@@@@@@ sub device des init stop @@@@@@@@@@@@@\r\n");

    err = kNoErr;
 exit:
    if(err != kNoErr)
    {
        app_log("[ERROR]fog_v2_subdevice_des_init() error!");
    }

    return err;
}



#endif
