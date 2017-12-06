#ifndef __FOG_V2_SUB_H_
#define __FOG_V2_SUB_H_

#include "fog_v2_config.h"

#if (FOG_V2_USE_SUB_DEVICE == 1)

//���豸ע��
#define FOG_V2_SUB_REGISTER_METHOD          HTTP_POST
#define FOG_V2_SUB_REGISTER_URI             ("/device/registersubdevice/")

//���豸ע��
#define FOG_V2_SUB_UNREGISTER_METHOD        HTTP_POST
#define FOG_V2_SUB_UNREGISTER_URI           ("/device/unregistersubdevice/")

//���豸����
#define FOG_V2_SUB_ATTACH_METHOD            HTTP_POST
#define FOG_V2_SUB_ATTACH_URI               ("/device/attachsubdevice/")

//���豸�Ͽ�����
#define FOG_V2_SUB_DETACH_METHOD            HTTP_POST
#define FOG_V2_SUB_DETACH_URI               ("/device/detachsubdevice/")

//���豸��ӳ�ʱ
#define FOG_V2_SUB_ADD_TIMEOUT_METHOD       HTTP_POST
#define FOG_V2_SUB_ADD_TIMEOUT_URI          ("/device/addsubtimeout/")

//���豸��ȡ�б�
#define FOG_V2_SUB_GET_LIST_METHOD          HTTP_GET
#define FOG_V2_SUB_GET_LIST_URI             ("/device/subdevicelist/")

//���豸 ��������
#define FOG_V2_SUB_SENDEVENT_METHOD         HTTP_POST
#define FOG_V2_SUB_SENDEVENT_URI            ("/device/sendeventsub/")

#define SUB_DEVICE_MQTT_QUEUE_SIZE          (3)

typedef struct SUBDEVICE_RECV_DATA{
    uint8_t *data;
    uint32_t data_len;
}SUBDEVICE_RECV_DATA_S;

typedef enum {
    MQTT_CMD_GATEWAY_UNBIND = 1,   //�����豸���
    MQTT_CMD_GATEWAY_BIND = 2,     //�����豸��
    MQTT_CMD_SUB_UNBIND = 3,       //���豸���
} SUBDEVICE_CMD_TYPE;

typedef struct SUBDEVICE_RECV_CMD_DATA{
    SUBDEVICE_CMD_TYPE cmd_type;
    char device_id[64];
}SUBDEVICE_RECV_CMD_DATA_S;

extern OSStatus fog_v2_subdevice_add_timeout(const char *s_product_id);

extern OSStatus fog_v2_add_subdevice( const char *s_product_id, const char *s_mac, bool auto_set_online);
extern OSStatus fog_v2_remove_subdevice( const char *s_product_id, const char *s_mac );
extern OSStatus fog_v2_set_subdevice_status(const char *s_product_id, const char *s_mac, bool online);
extern OSStatus fog_v2_subdevice_send(const char *s_product_id, const char *s_mac, const char *payload, uint32_t flag);
extern OSStatus fog_v2_subdevice_recv(const char *s_product_id, const char *s_mac, char *payload, uint32_t payload_len, uint32_t timeout);
extern OSStatus fog_v2_subdeice_get_list(char *http_response, uint32_t recv_len, bool *get_http_response_success);

extern void start_gateway_bind_monitor(void);
extern bool is_subdevice_cmd_queue_init(void);//�����Ƿ��Ѿ���ʼ��
extern OSStatus push_cmd_to_subdevice_queue(SUBDEVICE_CMD_TYPE type, const char *device_id);//�������в���һ������


#endif // end of FOG_V2_USE_SUB_DEVICE

#endif

