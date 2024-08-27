#ifndef _HF_HTTPSERVER__H__
#define _HF_HTTPSERVER__H__

#include <esp_http_server.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "nvs.h"
#include <dirent.h> 
#include <time.h>
#include "ds3231.h"
#include "HF_cmd_list.h"
#include "cJSON.h"

#define UPLOD_LIST_BUFF_SIZE    4096 
#define DELETE_USERS_BUFFER     512 
#define  ZERO                   0 

#define DATA_LENGTH             4
#define CARD_ID_LENGTH          8
#define USER_LINE_LENGTH        96
#define USER_DATA_LENGTH        100
#define USER_NAME_LENGTH        12 
#define FILE_NAME_LENGTH        12 
#define LOG_NAME_LENGTH         12
#define FILE_PATH_LENGTH        32 
#define LOG_DATE_LENGTH         9
#define BUFFER_LENGTH_64        64
#define LINE_LENGTH_64          64
#define LINE_LENGTH_60          60
#define FILE_PATH_LENGTH        32


#define COMPAER_LEN_3           3

#define  RETURN_OK              1
#define  RETURN_ERROR_1         -1

#define HTTP_MSG_LEN            48
 
#define  ADDING_ACTION          1
#define  UPDATING_ACTION        2
#define  DELETE_ACTION          3
#define  ADDING_LITE_ACTION     4 

#define TIME_REQ_BUFFER         256
#define BUFFER_48_B_LEN         48
#define BUFFER_128_B_LEN        128
#define BUFFER_256_B_LEN        256
#define BUFFER_512_B_LEN        512
#define LOGIN_INFO_LEN          30
#define NTP_ADDRESS_LEN         48

#define CARD_ID_LEN             8
#define LAST_NAME_LENGTH        21
#define FIRST_NAME_LENGTH       21
#define CARD_ID_BUF_LENGTH      9
#define PHONE_BUF_LENGTH        16
#define MAX_USERCOUNTER         200000000

#define SCHUDLE_ID_1            1
#define SCHUDLE_ID_2            2
#define SCHUDLE_ID_3            3
#define DAYS_OF_WEEK            7



#ifdef __cplusplus
extern "C" {
#endif


enum multiAction_error
{
    ACTION_OK               =0,
    EMPTY_ROOT              =1,
    RECORD_COUNT_ERROR      =2,
    ADD_USER_ERROR          =3,
    ADD_OVERFLOW_ERROR      =4,
    USER_NAME_ERROR         =5,
    USER_LASTNAME_ERROR     =6,
    USER_PHONE_ERROR        =7,
    USER_CARDID_ERROR       =8,
    USER_GENDER_ERROR       =9,
    USER_PROFILID_ERROR     =10,
    CARD_ID_FORMAT_ERROR    =11,
    ACTION_TYPE_ERROR       =12,
    USER_ID_ERROR           =13,
    FILE_NOT_EXIST          =14,
    OPEN_FILE_ERROR         =15,
    DELETE_FILE_ERROR       =16,   
    MORE_RECORD_ERROR       =17,
};


extern void  scann();
extern void connect_wifi(char* ssid,char* pass);

extern int get_str_key(nvs_handle_t handle, char* parameter,char* output);
extern int add_str_key(nvs_handle_t handle, char* parameter1, char* parameter2 );
extern int get_nvs_int_value(nvs_handle_t nvsHandle,char* paramter);
extern void set_nvs_int_value(nvs_handle_t nvsHandle,char* paramter,int value);
extern bool login_check(nvs_handle_t nvsHandle,char * loginId, char* password);

extern int  add_user(uint32_t userCounter,uint8_t profile,char* name, char* lastname,char * cardId,char* phone, char gender);
extern int  update_user(char* id,uint8_t  profile,char* name, char* lastname,char * cardId,char* phone, char gender);
extern void upload_profile(role_t* schedule,uint8_t id);
extern int del_user(char* cardId);
extern int del_log(char* name);
extern int clear_logs();

extern void ntp_service_init(void);
extern void ntp_stop_RQ(void);



// extern TimerHandle_t tmr_R1_Relay;
// extern TimerHandle_t tmr_R2_Relay;
// extern TimerHandle_t tmr_R1_last;
// extern TimerHandle_t tmr_R2_last;
// extern nvs_handle_t nvsHandle; 

// extern uint8_t  cardType;
// extern i2c_dev_t dev;

void stop_webserver(httpd_handle_t server);
httpd_handle_t start_webserver(void);


#ifdef __cplusplus
}
#endif

#endif 