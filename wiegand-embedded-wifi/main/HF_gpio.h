#ifndef _HF_GPIO__H__
#define _HF_GPIO__H__

#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "HF_cmd_list.h"


#ifdef __cplusplus
extern "C" {
#endif

#define TAG_Lock "Relay"
#define RELAY_1  26
#define RELAY_2  25
#define TURN_ON 1
#define TURN_OFF 0
#define INPUT_1 15
#define INPUT_2 27

// extern SemaphoreHandle_t xSemaphoreInput_1_ON;
// extern SemaphoreHandle_t xSemaphoreInput_1_OFF;
// extern SemaphoreHandle_t  xTMutex;
// extern TimerHandle_t tmr_IN1_last;

// extern TimerHandle_t tmr_R1_Relay;
// extern TimerHandle_t tmr_R2_Relay;


void IRAM_ATTR R1_Turn_Relay( TimerHandle_t xTimer );
void IRAM_ATTR R2_Turn_Relay( TimerHandle_t xTimer );

void input1_action(TimerHandle_t xTimer);

void init_output(void);
bool check_mode(void);

void init_check_BTN(void);
uint8_t check_pressed(void);

void IRAM_ATTR  R1_OpenDoor(void);
void IRAM_ATTR  R2_OpenDoor(void);

void OpenDoor_R2(void);
void OpenDoor_R1(void);

void R1_CloseDoor(void);
void R2_CloseDoor(void);

void IRAM_ATTR In1Out1_isr_handler(void* arg);
void IRAM_ATTR In1Out2_isr_handler(void* arg);

void input1_Handler();

#ifdef WIEGAND_OUTPUT_2
void input2_Handler();
extern SemaphoreHandle_t xSemaphoreInput_2;
extern TimerHandle_t tmr_IN2_last;
bool IN2_startReading=true;
#endif

#ifdef __cplusplus
}
#endif


#endif 

