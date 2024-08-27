#ifndef _GLOBLEVARIABLES__H__
#define _GLOBLEVARIABLES__H__

#include "freertos/event_groups.h"
#include "typeDefinitions.h"
#include "mqtt_client.h"
#include <esp_http_server.h>
#include "nvs_flash.h"
#include "nvs.h"


#ifdef __cplusplus
extern "C" {
#endif





struct tm rtcinfo ={
  .tm_year=2022,
  .tm_mon=02,
  .tm_mday=01,
  .tm_hour=00,
  .tm_min=00,
  .tm_sec=00,
  .tm_wday=00,
};




i2c_dev_t dev;

char MAC[18];
int port=8080;

int8_t  mode=0; 
int8_t  WiFi_mode=3;
uint8_t press=0;  
int8_t R1_call=0x01;
int8_t R2_call=0x02;  

int8_t R_type=20;

int8_t Out1_time=1;
int8_t Out2_time=1;
int8_t wait_time=5;


// Reader Setting 

// uint8_t card_type=32;
DRAM_ATTR   uint8_t  card_type = 26 ;
DRAM_ATTR   uint32_t lastdenied=0;




//char* log_path="/spiffs/log.txt";
//card_t userlist[210]; 

int USERCOUNT=0;
int userCounter=0;
unsigned int recordCount=0;
int f_logCount=0;
char Firmware[12]= "FW|0.0.1\0"; 


char ap_ssid[25]="TPS_WController\0";
char ap_pass[17]="0123456789\0";
char ap_ip[16]= "192.168.0.24\0"; 
char netmask[16]= "255.255.255.0\0";
char gateway[16] = "192.168.0.24\0";


char st_SSID[25];
char st_pass[17];
char myIP[16];

// login 
char login_name[25]="admin";
char login_pass[17]="123456";




//ap_list_t wifi_ap_list[20];

bool isConnected=false;
bool status=false;

SemaphoreHandle_t  xMutex;
SemaphoreHandle_t  xTMutex;
SemaphoreHandle_t mutex;

TaskHandle_t wifi_handler=NULL;
TaskHandle_t taskTCPsocket=NULL;
TaskHandle_t task_delFlogs=NULL;
TaskHandle_t task_mqttBrodcast=NULL;

QueueHandle_t HFcardQueue;

DRAM_ATTR  TaskHandle_t taskRead_1=NULL;
DRAM_ATTR  TaskHandle_t taskRead_2=NULL;

DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_1_ON = NULL;
DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_1_OFF = NULL;
DRAM_ATTR  SemaphoreHandle_t xSemaphoreInput_2 = NULL;
DRAM_ATTR  SemaphoreHandle_t xSemaphoreWebsocket_R2 = NULL;
DRAM_ATTR  SemaphoreHandle_t xSemaphoreWebsocket_R1 = NULL;



DRAM_ATTR  TimerHandle_t tmr_R1_Relay;
DRAM_ATTR  TimerHandle_t tmr_R2_Relay;
DRAM_ATTR  TimerHandle_t tmr_R1;
DRAM_ATTR  TimerHandle_t tmr_R2;
DRAM_ATTR  TimerHandle_t tmr_R1_last;
DRAM_ATTR  TimerHandle_t tmr_R2_last;
DRAM_ATTR  TimerHandle_t tmr_IN1_last;
DRAM_ATTR  TimerHandle_t tmr_IN2_last;

TimerHandle_t tmr_startup;


httpd_handle_t server = NULL;
nvs_handle_t nvsHandle; 
esp_mqtt_client_handle_t client;


EventGroupHandle_t connection_flag;
EventGroupHandle_t wifi_connection_flag;

EventGroupHandle_t R1_connection_flag;
EventGroupHandle_t R2_connection_flag;
const int CONNECTED_BIT = BIT0;          //1 << 0  //wifi connect

const int CONNECTED_BIT_R1 = BIT0;          //1 << 0  // Reader 1 
const int CONNECTED_BIT_R2=BIT1;            //1 << 1  // Reader 2 

const int CONNECTED_BIT_scanRes=BIT0;       //1 << 4  // wifi Scan

const int CONNECTED_BIT_RESET=BIT5;       //1 << 4    // NVS Reset 


const TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;
const TickType_t xTicksToWait_wifi = 5000 / portTICK_PERIOD_MS;
DRAM_ATTR    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

DRAM_ATTR   bool R1_state=false;
DRAM_ATTR   bool R2_state=false;
DRAM_ATTR   uint8_t isStandalone=1;
DRAM_ATTR   uint8_t mod_log=2;


char retry=0;
bool httpstate=false;

ap_list_t wifi_Ap_list[20];
  uint16_t APnum=0;

  role_t schedule_0[7];

  role_t schedule_1[7];

  role_t schedule_2[7];


  struct timeval tv;
  long upTime=0;
// MQTT SECTION 

  uint32_t MQTT_port =1883;
  uint8_t ismqtt=0;
  char MQTT_server[51] =  "mqtt://a2.pr14.bakelor.com";
  char MQTT_name [31]    = "drony";
  char MQTT_pass[21]  =   "2010Mesut8";

  char sub_topic1[48];
  char sub_topic2[48];

  bool mqttState=false;

  char topic[40];                      //"bakelor/product/pr14/device/wiegand" 

  char latitude[21]= "29.0";
  char longitude[21]= "0.0";

// input  
  bool in1State=0;
// bool in2State=0;
  int sock;

  int ntpState  = 0;
  int ntpRate   = 10;
  char ntpAddress[48]="0.tr.pool.ntp.org";

  bool ntp_Status =false;


#ifdef __cplusplus
}
#endif

#endif 



