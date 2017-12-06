#ifndef _FOG2_OTA_NOTIFICATION_H_
#define _FOG2_OTA_NOTIFICATION_H_

typedef enum {
    FOG2_OTA_CHECK_FAILURE = 1,             //���OTAʧ��
    FOG2_OTA_NO_NEW_VERSION = 2,            //��鷢�����°汾
    FOG2_OTA_IN_UPDATE = 3,                 //����OTA����
    FOG2_OTA_UPDATE_SUCCESS = 4,            //���³ɹ�
    FOG2_OTA_UPDATE_FAILURE = 5             //����ʧ��
} FOG2_OTA_EVENT_TYPE;

extern WEAK void user_fog_v2_ota_notification(FOG2_OTA_EVENT_TYPE ota_event_type);

#endif
