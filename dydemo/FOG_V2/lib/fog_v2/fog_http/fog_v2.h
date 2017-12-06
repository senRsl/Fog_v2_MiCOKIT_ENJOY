#ifndef __FOG_V2_H_
#define __FOG_V2_H_

#include "fog_http_client.h"
#include "fog_v2_config.h"

/*
 *     �豸��fogcloud��汾
 *     �汾����3��������ɣ�r.x.y
 *     r��Ŀǰ�������ں����汾��
 *     x��ż����ʾ�ȶ��汾��������ʾ�����а汾��
 *     y�������޲��Ĵ�����
 */
#define FOG_V2_LIB_VERSION_MAJOR        (2)
#define FOG_V2_LIB_VERSION_MINOR        (6)
#define FOG_V2_LIB_VERSION_REVISION     (0)


#define WAIT_HTTP_RES_MAX_TIME              (7*1000) //http�ȴ��������ʱʱ��Ϊ7s
#define HTTP_MAX_RETRY_TIMES                (4)      //������Դ���

#define FOG_HTTP_SUCCESS                    (0)
#define FOG_HTTP_TOKEN_EXPIRED              (1200)    //token����
#define FOG_HTTP_TOKEN_ERROR                (1201)    //token����
#define FOG_HTTP_TOKEN_INVAILD              (1202)    //��Ч��token

#define FOG_HTTP_PRODUCTI_ID_ERROR          (27020)   //��ƷID�����Ҳ�����Ӧ��Ʒ
#define FOG_HTTP_PRODUCTI_ID_NOT_SUB        (27021)   //���豸�������豸
#define FOG_HTTP_PRODUCTI_ID_NOT_GATEWAY    (27022)   //���豸���Ǹ��豸

#define FOG_HTTP_DEVICE_ID_ERROR            (27030)   //device id�����Ҳ�����Ӧ�豸/���豸
#define FOG_HTTP_PASSSWORD_ERROR            (27032)   //�������
#define FOG_HTTP_DEVICE_HAVE_SUPER_USER     (27061)   //�豸�Ѿ�ӵ�г����û�
#define FOG_HTTP_OTA_NO_UPDATE              (25001)   //û���µĹ̼��汾

//����
#define FOG_V2_ACTIVATE_METHOD              HTTP_POST
#define FOG_V2_ACTIVATE_URI                 ("/device/activate/")

//��ȡ��Ȩ
#define FOG_V2_GET_TOKEN_METHOD             HTTP_POST
#define FOG_V2_GET_TOKEN_URI                ("/device/token-auth/")

//��鳬���û�
#define FOG_V2_CHECK_SUPERUSER_METHOD       HTTP_GET
#define FOG_V2_CHECK_SUPERUSER_URI          ("/device/checkdevicesuperuser/")

//������Ȩ ���
#define FOG_V2_RECOVERY_METHOD              HTTP_POST
#define FOG_V2_RECOVERY_URI                 ("/device/recoverydevicegrant/")

//״̬ͬ��
#define FOG_V2_SYNC_STATUS_METHOD           HTTP_POST
#define FOG_V2_SYNC_STATUS_URI              ("/device/syncstatus/")

//��ȡ�豸��֤��  ����
#define FOG_V2_GENERATE_VERCODE_METHOD      HTTP_POST
#define FOG_V2_GENERATE_VERCODE_URI         ("/device/generatedevicevercode/")

//��ȡ������ʱ��
#define FOG_V2_GET_SERVER_TIME              HTTP_GET
#define FOG_V2_GET_SERVER_TIME_URI          ("/servertime/")

//��������
#define FOG_V2_SEND_EVENT_METHOD            HTTP_POST
#define FOG_V2_SEND_EVENT_URI               ("/device/sendeventadv/")

//OTA���
#define FOG_V2_OTA_UP_DATA_CHECK            HTTP_POST
#define FOG_V2_OTA_UP_DATA_URI              ("/ota/updatecheck/")

//OTA�ϱ�
#define FOG_V2_OTA_UPLOAD_LOG               HTTP_POST
#define FOG_V2_OTA_UPLOAD_URI               ("/ota/otauploadlog/")

//����������ط�������
#define FOG_V2_SEND_EVENT_RULES_PUBLISH    ((0x01) << 0)   //�Ƿ����豸��topicȥpublish����
#define FOG_V2_SEND_EVENT_RULES_DATEBASE   ((0x01) << 1)   //�Ƿ񽫴˴ε�payload���ݴ������ݿ�
#define FOG_V2_SEND_EVENT_RULES_PRODUCT    ((0x01) << 2)   //�Ƿ����豸��Ӧ��Ʒ��topicȥpublish����(�������͸�����)

#define DEVICE_ID_MIN_LEN                   (10)
#define DEVICE_TOKEN_MIN_LEN                (20)
#define DEVICE_MAC_LEN                      (12)

#define FOG_V2_PAYLOAD_LEN_MAX              (2048)

#define FOG_V2_SUB_DEVICE_MAX_NUM           (8) //���豸�����֧����Ŀ
#define FOG_V2_SUB_DEVICE_MQTT_TOPIC_LEN    (64)
#define FOG_V2_SUB_DEVICE_DES_PASSWORD      (0xAAAAAAAA) //���� �ϵ�֮���ȡ��ֵ,������Ǹ�ֵ,��Ҫ������豸������

#define FOG_V2_DEVICEID_LEN                 (36)
#define FOG_V2_PRODUCT_ID_LEN               (36)

#define FOG_V2_SUBDEVICEID_LEN              (36)
#define FOG_V2_SUBPRODUCT_ID_LEN            (36)

typedef struct _FOG_SUB_DES_S
{
    bool s_is_activate;      //�Ƿ��Ѽ������豸
    mico_queue_t queue;      //���豸����Ľ�����Ϣ�������

    char *s_mqtt_topic_cmd;  //cmd��ַ(�豸����ʱ�ǵ��ͷ�)
    char *s_mqtt_topic_commands; //command��ַ(�豸����ʱ�ǵ��ͷ�)

    char s_device_mac[16];   //MAC��ַ

    char s_product_id[40];   //��ƷID
    char s_device_id[40];    //�豸ID
}FOG_SUB_DES_S;

typedef struct _FOG_DES_S
{
    bool is_activate;           //�Ƿ��Ѽ����豸
	bool is_hava_superuser;     //�Ƿ�ӵ�г����û�
	bool is_recovery;           //�Ƿ���Ҫ������Ȩ

    char fog_v2_lib_version[16];//�����汾��  mdns֪ͨ��
    char devicepw[16];          //�豸����
    char mico_version[16];      //MICO�汾��
    char mxchip_sn[16];         //���ģ��sn��Ԥ��
    char device_mac[16];        //MAC��ַ
    char module_type[32];       //ģ���ͺ�
    char firmware[32];          //����汾
    char vercode[64];           //���ƶ�����õ���vercode
    char product_id[64];        //��ƷID
    char device_id[64];         //�豸ID
    char device_token[256];     //token

#if (FOG_V2_USER_FLASH_PARAM == 1)
    char user_app_data[FOG_V2_USER_FLASH_PARAM_LEN];    //user data
#endif

#if (FOG_V2_USE_SUB_DEVICE == 1)
    mico_mutex_t sub_des_mutex;
    uint32_t sub_des_password;
    FOG_SUB_DES_S sub_des[FOG_V2_SUB_DEVICE_MAX_NUM];   //���豸����
#endif
}FOG_DES_S;


extern void user_free_json_obj(json_object **obj);
extern bool get_wifi_status(void);
extern FOG_DES_S *get_fog_des_g(void);
extern void fog_des_clean(void);
extern void fog_set_device_recovery_flag(void);
extern void set_https_connect_status(bool status);
extern bool get_https_connect_status(void);
extern void set_mqtt_connect_status(bool status);
extern void fog_WifiStatusHandler(WiFiEvent status, void* const inContext);
extern OSStatus fog_v2_push_http_req_mutex(bool is_jwt, FOG_HTTP_METHOD method, char *request_uri, char *host_name, char *http_body, FOG_HTTP_RESPONSE_SETTING_S *user_http_resopnse);

extern bool fog_v2_is_have_superuser(void);
extern bool fog_v2_is_https_connect(void);
extern bool fog_v2_is_mqtt_connect(void);

extern OSStatus check_http_body_code(int32_t code);
extern OSStatus process_response_body(const char *http_body, int32_t *code);
extern OSStatus process_response_body_string(const char *http_body, int32_t *code, const char *user_key, char *value, uint32_t value_len);
extern OSStatus process_response_body_bool(const char *http_body, int32_t *code, const char *user_key, bool *user_key_data);

extern OSStatus fog_v2_device_generate_device_vercode(void);
extern OSStatus fog_v2_ota_check(char *resoponse_body, int32_t resoponse_body_len, bool *need_update);
extern OSStatus fog_v2_ota_upload_log(void);


//�û����ýӿ�
extern OSStatus init_fog_v2_service(void);     //��ʼ��fog����
extern OSStatus start_fog_v2_service(void);    //����fog�ķ���
extern OSStatus fog_v2_device_get_server_time(char *server_time_p, uint32_t recv_len); //��ȡʱ��
extern OSStatus fog_v2_device_send_event(const char *payload, uint32_t flag); //���ƶ˷�������
extern bool fog_v2_set_device_recovery_flag(void);

#endif

