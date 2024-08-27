#ifndef _HF_READER__H__
#define _HF_READER__H__

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_int_wdt.h"
#include  "esp_task_wdt.h"
#include "esp_attr.h"
#include "HF_cmd_list.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define TAG_ReadCard "Read_Card "

#define BufferSize 37                                 // Max size of read card bits  
#define Data_Size 40                                  // size of packet that is sent to TCP server 

#define Data0R1 32                                    //Green line R1=4 R2= 32  
#define Data1R1 33                                    //White line R1=15 R2=33
#define R1_Hold 22

#define Data1R2  18                                   //White line R1=15 R2=5
#define Data0R2  23                                   //Green line R1=4 R2= 2  
#define R2_Hold  19

#define RELAY_1  26
#define RELAY_2  25



#ifdef __cplusplus
extern "C" {
#endif

// extern TimerHandle_t tmr_R1_Relay;
// extern TimerHandle_t tmr_R2_Relay; 
// extern TimerHandle_t tmr_R1_last;
// extern TimerHandle_t tmr_R2_last;
// extern TaskHandle_t taskRead_1;
// extern TaskHandle_t taskRead_2;
// extern SemaphoreHandle_t xSemaphoreWebsocket_R1;
// extern SemaphoreHandle_t xSemaphoreWebsocket_R2;
// extern QueueHandle_t HFcardQueue;
// extern TimerHandle_t tmr_R1;
// extern TimerHandle_t tmr_R2;
// extern struct tm rtcinfo;
// extern bool isConnected;


extern void publish_QC(char* code , int reader);
extern esp_err_t ds3231_get_time(i2c_dev_t *dev, struct tm *time);
extern bool is_exists(char* cardID);

extern void IRAM_ATTR R1_OpenDoor(void);
extern void IRAM_ATTR R2_OpenDoor(void);


extern int write_to_file(char* card,int8_t reader,uint8_t permission,int year,int mon,int day,int hour,int min,int sec);
void wiegand_init(int, int, void (*Data0ISR) (void* arg), void (*Data1ISR) (void* arg));
void read_card_1(void *arg);
void read_card_2(void *arg);
void reader1_config();
void reader2_config();

void  IRAM_ATTR R2_clear( TimerHandle_t xTimer );
void  IRAM_ATTR R1_clear( TimerHandle_t xTimer );

void  IRAM_ATTR R1_lastcard(TimerHandle_t xTimer );
void  IRAM_ATTR R2_lastcard(TimerHandle_t xTimer );



#ifdef __cplusplus
}
#endif

#endif 

