#include "HF_gpio.h"

// static bool IN1_startReading=true;

bool check_mode()
{
  return gpio_get_level(STARTUP_BTN);
}

void init_check_BTN()
{
  gpio_pad_select_gpio(STARTUP_BTN);
  gpio_set_direction(STARTUP_BTN, GPIO_MODE_INPUT);
}

void init_output(){
  gpio_pad_select_gpio(RELAY_1);
  gpio_pad_select_gpio(RELAY_2);
   gpio_pad_select_gpio(INPUT_1);

  gpio_set_direction(RELAY_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(RELAY_2, GPIO_MODE_OUTPUT);
  gpio_set_direction(RELAY_2, GPIO_MODE_OUTPUT);
  gpio_set_direction(INPUT_1, GPIO_MODE_INPUT);

  // gpio_set_intr_type(INPUT_1, GPIO_INTR_POSEDGE);
  gpio_set_intr_type(INPUT_1, GPIO_INTR_ANYEDGE);
  gpio_isr_handler_add(INPUT_1, In1Out1_isr_handler, NULL);
  gpio_intr_enable(INPUT_1);

#ifdef WIEGAND_OUTPUT_2
  gpio_set_direction(INPUT_2, GPIO_MODE_INPUT);
  gpio_set_intr_type(INPUT_2, GPIO_INTR_POSEDGE);
  gpio_isr_handler_add(INPUT_2, In1Out2_isr_handler, NULL);
  gpio_intr_enable(INPUT_2);
#endif 

}


uint8_t check_pressed(void)
{  
     
      init_check_BTN();
      uint8_t result=NOT_PRESSED;
      if(check_mode()==PRESSED)
      {
        uint64_t previous=xTaskGetTickCount();
        uint64_t current;
        while(1)
        {
          current = xTaskGetTickCount();
          if(check_mode()==PRESSED)
          {
              ESP_LOGW("BTN 0"," PRESSED  %lld   %lld",current,previous);
              if ((current - previous) >= LONG_PRESS_TIME)
              {                
                ESP_LOGI("BTN 0","LONG PRESS DETECTED");
                result=BTN_VAL_CLEAR_NVS;
                break;
              }
          }
          else 
          {
              if ((current - previous) >= SHORT_PRESS_TIME) 
              {
                ESP_LOGI("BTN 0","SHORT PRESS DETECTED"); 
                result=BTN_VAL_ACCESSPOINT; 
                break;
              }
              ESP_LOGW("BTN 0","NO LONG PRESS DETECTED");
              result=NOT_PRESSED;
              break;
            }
          vTaskDelay(DELAY_200_MS);
        }
    }
    xSemaphoreGive(mutex);
    return result;
}

void OpenDoor_R2(void){
  gpio_set_level(RELAY_2,TURN_ON);
}

void  OpenDoor_R1(void){
  gpio_set_level(RELAY_1,TURN_ON);
}

void IRAM_ATTR R2_OpenDoor(void)
{
  gpio_set_level(RELAY_2,TURN_ON);
  xTimerReset(tmr_R2_Relay,( TickType_t) 5);
}

void IRAM_ATTR  R1_OpenDoor(void)
{
  gpio_set_level(RELAY_1,TURN_ON);
  xTimerReset(tmr_R1_Relay,( TickType_t) 5);
}

void R1_CloseDoor(void)
{
  gpio_set_level(RELAY_1,TURN_OFF);
}

void R2_CloseDoor(void)
{
  gpio_set_level(RELAY_2,TURN_OFF);
}

void  R1_Turn_Relay( TimerHandle_t xTimer )
{
  R1_CloseDoor();
}

void  R2_Turn_Relay( TimerHandle_t xTimer )
{
  R2_CloseDoor();
}

void IRAM_ATTR In1Out1_isr_handler(void* arg) 
{
    xTimerResetFromISR(tmr_IN1_last,&xHigherPriorityTaskWoken);
}

#ifdef WIEGAND_OUTPUT_2
 void IRAM_ATTR In1Out2_isr_handler(void* arg) {
    if(IN2_startReading){
    IN2_startReading =false;
    xTimerResetFromISR(tmr_IN2_last,&xHigherPriorityTaskWoken);}
 }
#endif

void input1_Handler(TimerHandle_t xTimer)
{
  xSemaphoreTake(xTMutex, portMAX_DELAY);    
  if(gpio_get_level(INPUT_1)==in1State) 
  {
    ESP_LOGE("IN_0","False Edge %d \n",in1State);
  }
  else 
  {
    in1State=!(in1State);                                                      
    if((isConnected)&&(isStandalone!=1))
    {
        if(in1State) 
        {
          xSemaphoreGive(xSemaphoreInput_1_ON);
        } 
        else         
        {
          xSemaphoreGive(xSemaphoreInput_1_OFF);
        } 
    }
    // ESP_LOGW("IN_0","Any Ede  %d \n",in1State);
  }
  xSemaphoreGive(xTMutex);       
}

#ifdef WIEGAND_OUTPUT_2

void input2_Handler(TimerHandle_t xTimer)
{
  if (gpio_get_level(INPUT_2))
  { 
    xSemaphoreGive(xSemaphoreInput_2);
  } 
  // ESP_LOGE(TAG_Lock,"INPUT2!"); }
  IN2_startReading=true;  
}

void _input1Handler()
{
            uint64_t previous=xTaskGetTickCount();
            uint64_t current=0;
            while(1)
            {
              current = xTaskGetTickCount();
              if(gpio_get_level(INPUT_1))
              {
                // printf(" PRESSED  %lld   %lld\n",current,previous);
                if ((current - previous) >= 200) 
                {   
                  printf("PRESS DETECTED \n");   
                  break;
                }
               }
              else break;
              vTaskDelay(10/portTICK_PERIOD_MS);
            }
  IN1_startReading=true;
}
#endif


