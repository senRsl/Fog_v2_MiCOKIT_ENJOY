#ifndef __FOG_HTTP_CLIENT_H_
#define __FOG_HTTP_CLIENT_H_

#define HTTP_SEND_TIME_OUT              (3000)
#define HTTP_RECV_TIME_OUT              (3000)
#define HTTP_KEEP_IDLE_TIME             (6)
#define HTTP_KEEP_INTVL_TIME            (3)
#define HTTP_KEEP_COUNT                 (3)


#define HTTP_YIELD_TMIE                 (2000) //http��ʱʱ��

#define HTTP_REQUEST_BODY_MAX_LEN       (2048)
#define HTTP_REQUEST_HOST_NAME_MAX_LEN  (64)
#define HTTP_REQUEST_REQ_URI_MAX_LEN    (64)
#define HTTP_REQUEST_JWT_MAX_LEN        (512)

#define HTTP_RESPONSE_BODY_MAX_LEN      (2048)    //HTTP���ؽ��յ���󳤶�Ϊ2K

#define HTTP_HEAD_METHOD_POST           ("POST")
#define HTTP_HEAD_METHOD_GET            ("GET")


typedef enum {
    HTTP_POST = 1,
    HTTP_GET = 2
} FOG_HTTP_METHOD;

typedef enum {
    HTTP_REQUEST_ERROR = 1,     //�������
    HTTP_CONNECT_ERROR = 2,     //����ʧ��
    HTTP_RESPONSE_SUCCESS = 3,  //�ɹ�
    HTTP_RESPONSE_FAILURE = 4   //ʧ��
} FOG_HTTP_RESPONSE_E;


typedef struct _FOG_HTTP_RESPONSE_SETTING
{
    FOG_HTTP_RESPONSE_E send_status;    //����״̬
    uint32_t http_res_id;
    int32_t status_code;                //http������
    char *fog_response_body;
}FOG_HTTP_RESPONSE_SETTING_S;


typedef struct _FOG_HTTP_REQUEST_SETTING
{
    uint32_t http_req_id;
    bool is_jwt;    //�Ƿ�jwt��ʽ��¼
    FOG_HTTP_METHOD method;
    char jwt[HTTP_REQUEST_JWT_MAX_LEN];
    char request_uri[HTTP_REQUEST_REQ_URI_MAX_LEN];
    char host_name[HTTP_REQUEST_HOST_NAME_MAX_LEN];
    char *http_body;
}FOG_HTTP_REQUEST_SETTING_S;


extern mico_queue_t fog_http_request_queue;  //FOG HTTP�ķ����������
extern mico_queue_t fog_http_response_queue; //FOG HTTP�Ľ�����Ӧ����

extern OSStatus start_fogcloud_http_client(void);

#endif

