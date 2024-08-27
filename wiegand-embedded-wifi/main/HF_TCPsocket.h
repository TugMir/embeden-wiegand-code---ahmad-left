#ifndef _HF_TCPSOCKET__H__
#define _HF_TCPSOCKET__H__

#include <string.h>
#include <stdio.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "HF_reader.h"
#include "HF_cmd_list.h"
#include <dirent.h> 


#define TX_MSG_LEN                      64 
#define RX_MSG_LEN                      64
#define NON_BLOCK                       0

#define LINE_LENGTH                     48     
#define VALUE_LENGTH                    52

#define ERROR_OPEN_FILE                 -2
#define ERROR_EMPTY_FILE                -1


#define KEEPALIVE_MS                100
#define KEEPIDLE_SEC                1
#define KEEPINTERVAL_SEC            2
#define KEEPCOUNT_PROB              3


#ifdef __cplusplus
extern "C" {
#endif

// extern const char* Firmware;
// extern char MAC[18];

// extern bool isConnected;                     // used in read_card  to check connection State
// extern SemaphoreHandle_t xSemaphoreWebsocket_R1;
// extern SemaphoreHandle_t xSemaphoreWebsocket_R2;

// extern SemaphoreHandle_t xSemaphoreInput_1_OFF;
// extern SemaphoreHandle_t xSemaphoreInput_1_ON;
// // extern SemaphoreHandle_t xSemaphoreInput_2;

// extern QueueHandle_t HFcardQueue;
// extern TimerHandle_t tmr_R1_Relay;
// extern TimerHandle_t tmr_R2_Relay;


extern void IRAM_ATTR  R1_OpenDoor(void);
extern void IRAM_ATTR  R2_OpenDoor(void);

extern void OpenDoor_R2(void);
extern void OpenDoor_R1(void);

extern void R1_CloseDoor(void);
extern void R2_CloseDoor(void);

extern int del_log(char* name);

int receive_cmd(int sock);
void Clear_receive(int sock);
int send_data(int sock,char* tx_buffer);
void setKeepalive(int listenfd );
int espx_last_socket_errno(int socket);
int send_file(int sock , char* path);
int _send(int s,const char *dataptr);
int heartBeat(int s);
void tcp_server_task(void *pvParameters);


#ifdef __cplusplus
}
#endif

#endif 

