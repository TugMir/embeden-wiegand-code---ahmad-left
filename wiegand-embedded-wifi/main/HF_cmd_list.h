#ifndef _HF_CMD_LIST__H__
#define _HF_CMD_LIST__H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "mqtt_client.h"
#include <esp_http_server.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "typeDefinitions.h"


// #define LOG_DBG                    1   // Save Log  for { Reset - Wifi connect - Wifi disconnect } Actions

#define MAX_USER_COUNT              1000

#define HOLD_READING_TIME           200
#define OPEN_WAITING_TIME           2000   
#define STARTUP_TIME                30000  
#define LONG_PRESS_TIME             500 
#define SHORT_PRESS_TIME            100
#define INPUT_DELAY                 100

#define CONFIG_SCL_GPIO		        22                                                                   
#define CONFIG_SDA_GPIO		        21
#define STARTUP_BTN                 27

#define STA_SSID                    "STASSID"
#define STA_PASS                    "STAPASS"
#define USER_COUNT                  "USERCONT"
#define USER_COUNTER                "USERCOUNTER"

#define F_LOG_COUNT                 "logcount"
#define R_LOG_COUNT                 "recordcount"
// #define NEWEST_FLOG  "newstflog"

#define AP_IP                       "ap_ip"
#define AP_NETMASK                  "ap_mask"

#define AP_SSID                     "ap_ssid"
#define AP_PASSWORD                 "ap_pass"
#define AP_GATEWAY                  "ap_gateway"


#define LOGINID                     "loginuser"
#define LOGINPASS                   "loginpass" 

#define LOG_ID_VAL                  "admin"
#define LOG_PASS_VAL                "123456"

#define R1_MODE                     "R1mode"
#define R2_MODE                     "R2mode"

#define CARD_TYPE                   "cardtype"
#define FIRMWARE                    "Firmware"


#define WiFi_MODE                   "wifimode"


#define G1_OPEN_TIME                "G1wait"
#define G2_OPEN_TIME                "G2wait"
#define WAITTIME                    "WaitTime" 

#define READER_MODE                 "R_mode"
#define READER_TYPE                 "R_type"
#define LOG_MODE                    "log_mod"

#define PORT                        "port"

#define MQTTPORT                    "mqtprt"
#define MQTTSTATE                   "mqtst"
#define MQTTSERVER                  "mqtsr"
#define MQTTUSER                    "mqtusr"
#define MQTTPASS                    "mqtpas"

#define LONGITUDE                   "xz"
#define LATITUDE                    "xy"

#define NTPSTATE                    "ntpS"
#define NTPADDRS                    "ntpAD"
#define NTPRATE                     "ntpR"    

//-------------------------------

#define TAG_TCP                     "TCP Server"
#define Cmd_restart                 "restart"

#define Cmd_Open_R1                 "OPEN|1"
#define Cmd_Close_R1                "CLOSE|1"
#define Cmd_Open_Delay_R1           "OPEN|1:"

#define Cmd_Open_R2                 "OPEN|2"
#define Cmd_Close_R2                "CLOSE|2"
#define Cmd_Open_Delay_R2           "OPEN|2:"

#define REMOVE_LOG                  "CLEARLOG"
#define SEND_LOG                    "LOG"

#define FINISH_SEND                 "LOG|FINISH"
#define START_SEND                  "LOG|START "

#define INPUT_1_ON                  "IN|1:1"
#define INPUT_1_OFF                 "IN|1:0"

#define INPUT_2_ON                  "IN|2:1"
#define INPUT_2_OFF                 "IN|2:0"

#define MAX_RECORD                  110
#define F_RECORD_COUNT              10

#define WDT_TIMEOUT                 8 
#define OUT1TPOIC                   "bakelor/pr14/prod/%s/out0"
#define OUT2TPOIC                   "bakelor/pr14/prod/%s/out1"
#define BRODTPOIC                   "bakelor/pr14/prod/%s/"
#define PUBTPOIC                    "bakelor/pr14/prod/%s/w"

#define OPEN                        '1'
#define CLOSE                       '0'


#define  BTN_VAL_ACCESSPOINT        1
#define  BTN_VAL_CLEAR_NVS          2  

#define PRESSED                     1
#define LONG_PRESSED                2
#define NOT_PRESSED                 0

#define OK_RESULT                   0 
#define NO_RESULT                   1 

#define VAL_0                       0


#define DELAY_10_MS                     10/portTICK_PERIOD_MS      
#define DELAY_100_MS                    100/portTICK_PERIOD_MS      
#define DELAY_200_MS                    200/portTICK_PERIOD_MS     
#define DELAY_300_MS                    300/portTICK_PERIOD_MS      
#define DELAY_500_MS                    500/portTICK_PERIOD_MS      

#define DELAY_1_SEC                     1000/portTICK_PERIOD_MS     
#define DELAY_5_SEC                     5000/portTICK_PERIOD_MS 

#define MIN_TO_SEC                      60
#define RESET_WAITING_TIME              10* MIN_TO_SEC

#define SHEDULE_NO_0                    0
#define SHEDULE_NO_1                    1
#define SHEDULE_NO_2                    2

//----------------

#define PASSWOR_MIN_LEN                 8           
#define PASSWOR_MAX_LEN                 16

#define SSID_LEN                        25
#define PASS_LEN                        17 

//----------------

#define ACCESSPOINT_MODE                0
#define STATION_MODE                    1
#define STATION_2_STA_MODE              2
#define WIFI_MODE_STATION               1 
#define WIFI_MODE_ACCESSPOINT           2
#define WIFI_MODE_MIXED                 3

//----------------------

#define CARD_26_BIT             26
#define CARD_32_BIT             32
#define CARD_34_BIT             34

//--------------------------
extern SemaphoreHandle_t  xMutex;
extern SemaphoreHandle_t  xTMutex;
extern SemaphoreHandle_t mutex;

extern TaskHandle_t wifi_handler;
extern TaskHandle_t taskTCPsocket;
extern TaskHandle_t task_delFlogs;
extern TaskHandle_t task_mqttBrodcast;

extern QueueHandle_t HFcardQueue;


extern DRAM_ATTR  TaskHandle_t taskRead_1;
extern DRAM_ATTR  TaskHandle_t taskRead_2;

extern DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_1_ON ;
extern DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_1_OFF ;
extern DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_2 ;
extern DRAM_ATTR  SemaphoreHandle_t xSemaphoreWebsocket_R2 ;
extern DRAM_ATTR  SemaphoreHandle_t xSemaphoreWebsocket_R1 ;


extern DRAM_ATTR  TimerHandle_t tmr_R1_Relay;
extern DRAM_ATTR  TimerHandle_t tmr_R2_Relay;
extern DRAM_ATTR  TimerHandle_t tmr_R1;
extern DRAM_ATTR  TimerHandle_t tmr_R2;
extern DRAM_ATTR  TimerHandle_t tmr_R1_last;
extern DRAM_ATTR  TimerHandle_t tmr_R2_last;
extern DRAM_ATTR  TimerHandle_t tmr_IN1_last;
extern DRAM_ATTR  TimerHandle_t tmr_IN2_last;

extern TimerHandle_t tmr_startup;

extern httpd_handle_t server ;
extern nvs_handle_t nvsHandle; 
extern esp_mqtt_client_handle_t client;

extern int ntpState;
extern int ntpRate;
extern char ntpAddress[48];
extern bool ntp_Status;

extern int port;
extern int8_t  mode; 
extern int8_t  WiFi_mode;
extern uint8_t press;  
extern int8_t R1_call;
extern int8_t R2_call;  
extern int8_t R_type;
extern int8_t Out1_time;
extern int8_t Out2_time;
extern int8_t wait_time;
extern int USERCOUNT;
extern int userCounter;
extern unsigned int recordCount;
extern int f_logCount;
extern bool isConnected;
extern bool status;
extern char retry;
extern bool httpstate;
extern struct timeval tv;
extern long upTime;
extern uint32_t MQTT_port ;
extern uint8_t ismqtt;
extern bool mqttState;
extern uint16_t APnum;
extern bool in1State;
extern int sock;



extern ap_list_t wifi_Ap_list[20];
extern char MQTT_server[51];
extern char MQTT_name [31];
extern char MQTT_pass[21];
extern char sub_topic1[48];
extern char sub_topic2[48];
extern char topic[40];                      
extern char latitude[21];
extern char longitude[21];
extern role_t schedule_0[7];
extern role_t schedule_1[7];
extern role_t schedule_2[7];
extern char Firmware[12]; 
extern char ap_ssid[SSID_LEN];
extern char ap_pass[PASS_LEN];
extern char ap_ip[16]; 
extern char netmask[16];
extern char gateway[16];
extern char st_SSID[25];
extern char st_pass[17];
extern char myIP[16];
extern char login_name[25];
extern char login_pass[17];
extern char MAC[18];


extern DRAM_ATTR   uint8_t  card_type;
extern DRAM_ATTR   uint32_t lastdenied;
extern DRAM_ATTR   BaseType_t xHigherPriorityTaskWoken;
extern DRAM_ATTR   bool R1_state;
extern DRAM_ATTR   bool R2_state;
extern DRAM_ATTR   uint8_t isStandalone;
extern DRAM_ATTR   uint8_t mod_log;

  
extern EventGroupHandle_t connection_flag;
extern EventGroupHandle_t wifi_connection_flag;
extern EventGroupHandle_t R1_connection_flag;
extern EventGroupHandle_t R2_connection_flag;

extern const int CONNECTED_BIT ;          //1 << 0  //wifi connect
extern const int CONNECTED_BIT_R1 ;          //1 << 0  // Reader 1 
extern const int CONNECTED_BIT_R2;            //1 << 1  // Reader 2 
extern const int CONNECTED_BIT_scanRes;       //1 << 4  // wifi Scan
extern const int CONNECTED_BIT_RESET;       //1 << 4    // NVS Reset 
extern const TickType_t xTicksToWait;
extern const TickType_t xTicksToWait_wifi ;
extern i2c_dev_t dev;

extern struct tm rtcinfo;
#ifdef __cplusplus
}
#endif

#endif 


