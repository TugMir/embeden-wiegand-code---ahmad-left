#include "HF_reader.h"

static bool isr_set=true;                                                 // State of ISR Driver Configuration 
static volatile DRAM_ATTR uint8_t R1_counter=0;                           // Count the Readed data bits for Reader 1 
static volatile DRAM_ATTR uint8_t R2_counter=0;                           // control the Readed data bits for Reader 1 
static DRAM_ATTR  uint8_t R1_databits[BufferSize];                        // Hold HF Card code in Binary format for Reader 1 
static DRAM_ATTR  uint8_t R2_databits[BufferSize];                        // Hold HF Card code in Binary format for Reader 1 
static DRAM_ATTR char R1_post_data[Data_Size];                            // Hold HF Card code in HEX format for Reader 1 
static DRAM_ATTR char R2_post_data[Data_Size];                            // Hold HF Card code in HEX format for Reader 2 
static volatile DRAM_ATTR bool R1_startReading =true;                     // Define  stat point to run Timer 1
static volatile DRAM_ATTR bool R2_startReading =true;     
static DRAM_ATTR char card_2[9];
static DRAM_ATTR uint32_t R2_code;
static volatile DRAM_ATTR uint32_t R1_last=0;
static volatile DRAM_ATTR uint32_t R2_last=0;
static DRAM_ATTR char card_1[9];
static DRAM_ATTR uint32_t R1_code;
static volatile DRAM_ATTR uint8_t  permission_R1=0;
static volatile DRAM_ATTR uint8_t  permission_R2=0;

static void mask_R1_irq(void);
static void unmask_R1_irq(void);
static void mask_R2_irq(void);
static void unmask_R2_irq(void);
// #define PARITY_CHECK                     1

void wiegand_init(int data0 , int data1 , void (*Data0ISR) (void* arg), void (*Data1ISR) (void* arg))
{
  gpio_pad_select_gpio(data0);
  gpio_pad_select_gpio(data1);
// set the correct direction
  gpio_set_direction(data0, GPIO_MODE_INPUT);
  gpio_set_direction(data1, GPIO_MODE_INPUT);
  gpio_set_pull_mode(data0,GPIO_PULLUP_ONLY);
  gpio_set_pull_mode(data1,GPIO_PULLUP_ONLY);
  vTaskDelay(50/portTICK_PERIOD_MS); 

  //GPIO_INTR_NEGEDGE rising edge  - GPIO_INTR_POSEDGE 
  gpio_set_intr_type(data0, GPIO_INTR_POSEDGE);
  gpio_set_intr_type(data1, GPIO_INTR_POSEDGE);
  // install ISR service with default configuration
  if(isr_set){                                                                        // Prevent reconfiguration of ISR Driver  
    isr_set=false;
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM|ESP_INTR_FLAG_LEVEL1|ESP_INTR_FLAG_LEVEL2|ESP_INTR_FLAG_LEVEL3);
  }
  // attach the interrupt service routine
  gpio_isr_handler_add(data0, Data0ISR, NULL);
  gpio_isr_handler_add(data1, Data1ISR, NULL);

  gpio_intr_enable(data0);
  gpio_intr_enable(data1);
}

void IRAM_ATTR R1_Data0_isr_handler(void* arg)
{
  if(R1_startReading)
  {
      R1_startReading =false;
      xTimerResetFromISR(tmr_R1,&xHigherPriorityTaskWoken);
  }
  R1_databits[R1_counter++] = 0;
}

void IRAM_ATTR R1_Data1_isr_handler(void* arg) 
{
  if(R1_startReading)
  {
    R1_startReading =false;
    xTimerResetFromISR(tmr_R1,&xHigherPriorityTaskWoken);
  }
  R1_databits[R1_counter++] = 1;
}

void IRAM_ATTR R2_Data0_isr_handler(void* arg) {
  if(R2_startReading)
  {
    R2_startReading =false;
    xTimerResetFromISR(tmr_R2,&xHigherPriorityTaskWoken);
  }
  R2_databits[R2_counter++] = 0;
}

void IRAM_ATTR R2_Data1_isr_handler(void* arg) 
{
 if(R2_startReading)
 {
    R2_startReading =false;
    xTimerResetFromISR(tmr_R2,&xHigherPriorityTaskWoken);
  }
  R2_databits[R2_counter++] = 1;
}

void reader1_config(){
  wiegand_init(Data0R1 , Data1R1,&R1_Data0_isr_handler,&R1_Data1_isr_handler);
  R1_counter=0;
  memset(R1_post_data,0,Data_Size*sizeof(R1_post_data[0]));
  memset(R1_databits,0,BufferSize*sizeof(R1_databits[0]));
  vTaskDelay(50/portTICK_PERIOD_MS);
  ESP_LOGW("HF_READER","Reader1 has been initialized");
}

void reader2_config(){
  wiegand_init(Data0R2 , Data1R2,&R2_Data0_isr_handler,&R2_Data1_isr_handler);
  R2_counter=0;
  memset(R2_post_data,0, Data_Size*sizeof(R2_post_data[0])); 
  memset(R2_databits,0,BufferSize*sizeof(R2_databits[0]));
  vTaskDelay(50/portTICK_PERIOD_MS);
  ESP_LOGW("HF_READER","Reader2 has been initialized");
}


void IRAM_ATTR read_card_1(void *arg)
{ 
  ESP_LOGW(TAG_ReadCard,"Starting Task read 1 : %s ",pcTaskGetTaskName(NULL));
  while(1)
  {
    vTaskSuspend(NULL);
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    R1_code=0;
    if (card_type==CARD_32_BIT)
    {
      for (int i=0; i<32; i++)
          R1_code=(R1_code<<1)|R1_databits[i];
      // for (int i=0; i<4; i++)                                                                     // Reverce Card Id 
      //   for (int j=0; j<8; j++){ R1_code=(R1_code<<1)|R1_databits[32-(i*8)+j-8];}
    }
    else if(card_type==CARD_26_BIT)
    {
#ifdef PARITY_CHECK
      uint8_t sum=R1_databits[0];
#endif
      for (int i=1; i<25; i++)
      {
          R1_code=(R1_code<<1)|R1_databits[i];
#ifdef PARITY_CHECK
          if(R1_databits[i]) 
          {
            sum++;
          }
          if(i==12) 
          {
            if(sum%2==0) 
             { 
               sum=R1_databits[25];
             }
             else
             {
               printf("ERR_L[%d]\n",sum);
               R1_code=0; 
               break;
             }
          }
          if(i==24)
          {
             if(sum%2==0) 
             {
               printf("ERR_H[%d]\n",sum);
               R1_code=0; 
               break;
             }
          }
#endif
      }  

      // for (int i=0; i<3; i++)                                                                     // Reverce Card Id
      //   for (int j=0; j<8; j++){R1_code=(R1_code<<1)|R1_databits[24-(i*8)+j-7];}
      // // printf("%d %d",R1_databits[0],R1_databits[card_type-1]);
    }
    else if (card_type==CARD_34_BIT)
    {
#ifdef PARITY_CHECK
      uint8_t sum=R1_databits[0];
#endif
      for (int i=1; i<33; i++)
      {
          R1_code=(R1_code<<1)|R1_databits[i];
#ifdef PARITY_CHECK
          if(R1_databits[i]) 
          {
            sum++;
          }
          if(i==16) 
          {
            if(sum%2==0) 
             { 
               sum=R1_databits[33];
             }
             else
             {
               printf("ERR_L[%d]\n",sum);
               R1_code=0; 
               break;
             }
          }
          if(i==32)
          {
             if(sum%2==0) 
             {
               printf("ERR_H[%d]\n",sum);
               R1_code=0; 
               break;
             }
          }
  #endif 
      }
    }
    
    // unmask_R1_irq();  
    if((R1_last == R1_code))
    {
      goto end;
    }  
    else 
    {
      R1_last = R1_code; 
      xTimerReset(tmr_R1_last,( TickType_t) 10); 
    } 
    
    memset(R1_databits,0, BufferSize*sizeof(R1_databits[0]));
    bzero(card_1,9);
    sprintf(card_1,"%08x",R1_code);
    esp_task_wdt_reset(); 
    if((R1_code!=4294967295)&&(R1_code!=0)&&(strlen(card_1)==8))
    {
      if (ismqtt)
      {
        printf("Reader|1|Card_Id: 0x%s   %d  %d\n",card_1,mqttState, heap_caps_get_free_size(MALLOC_CAP_DEFAULT) ); 
        if(mqttState)publish_QC(card_1 ,1);
        goto end;
      }      
      R1_state=false;
      if(isConnected)
      {
        memset(R1_post_data,0,Data_Size*sizeof(R1_post_data[0]));
        if(isStandalone!=1){
          printf("Reader|1|Card_Id: 0x%s   %d\n",card_1, heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
          // sprintf(R1_post_data, "TAG|1:%s",card_1);
          sprintf(R1_post_data, "TAG|%d:%s",R1_call,card_1);
          if(xQueueSendToFront(HFcardQueue, (void*)R1_post_data , (TickType_t)10 ) !=1){ 
                printf("could not sended this message = %s \n",R1_post_data); 
          }
          else 
          {
                 xEventGroupClearBits(R1_connection_flag, CONNECTED_BIT_R1);
                 esp_task_wdt_reset();
                 xSemaphoreGive(xSemaphoreWebsocket_R1);
                 xEventGroupWaitBits(R1_connection_flag,CONNECTED_BIT_R1, pdTRUE, pdFALSE, xTicksToWait);
          }
          lastdenied=R1_code;
        } 
      }
      esp_task_wdt_reset(); 
      if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {ESP_LOGE("DS3231", "Could not get time.");}
      if(!R1_state)
      {
        permission_R1=0;
        if(is_exists(card_1))
        {
            if ((R1_call%2==1))
            { 
              R1_OpenDoor();
            }
            else
            {
              R2_OpenDoor();
            }
            permission_R1=1;
        }
        else
        {
          permission_R1=0; 
          lastdenied=R1_code;
        }
        if(write_to_file(card_1,R1_call,permission_R1,rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec))
        {
          ESP_LOGI("R1_log","%s,%d |%d: %d",card_1,permission_R1,recordCount,esp_get_free_heap_size());
        }              
        else
        {
          ESP_LOGE("Reader1","0x%s Unable to save Log %d",card_1,heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        }                
       if(isConnected)
       {
        memset(R1_post_data,0,Data_Size*sizeof(R1_post_data[0]));
        if(isStandalone==1)
        {
          if(mod_log==1)
          {
            sprintf(R1_post_data, "%s;%d;%d;%04d-%02d-%02d;%02d:%02d:%02d",card_1,R1_call,permission_R1,rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);
            if(xQueueSendToFront(HFcardQueue, (void*)R1_post_data , (TickType_t)10 ) !=1)
            {
              printf("could not sended this message = %s \n",R1_post_data);
            }
            else 
            {
              xSemaphoreGive(xSemaphoreWebsocket_R1);
            }
          }
        } 
       }         
      }
    }
    else
    {
      ESP_LOGI(TAG_ReadCard,"R1 Invaled Card ID %s",card_1);
    }

    end:
    permission_R1=0;  
    // for(int i=0 ; i<20;i++)
    // {
    //   printf("Waiting[%d]\n",i); 
    //   vTaskDelay(1000/portTICK_RATE_MS);
    // }
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));  

 }
}

void IRAM_ATTR read_card_2(void *arg)
{ 
  ESP_LOGW(TAG_ReadCard,"Starting Task read2: %s",pcTaskGetTaskName(NULL));
  while(1){
    vTaskSuspend(NULL);
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    R2_code=0;

    if (card_type==CARD_32_BIT)
    {
      for (int i=0; i<32; i++)
          R2_code=(R2_code<<1)|R2_databits[i];
  //  for (int i=0; i<4; i++)           // Reverce Card Id 
  //     for (int j=0; j<8; j++){ R2_code=(R2_code<<1)|R2_databits[32-(i*8)+j-8];}
    }
    else if(card_type==CARD_26_BIT)
    { 
#ifdef PARITY_CHECK
      uint8_t sum=R2_databits[0];
#endif 
      for (int i=1; i<25; i++)
      {
          R2_code=(R2_code<<1)|R2_databits[i];

#ifdef PARITY_CHECK
          if(R2_databits[i]) 
          {
            sum++;
          }
          if(i==12) 
          {
            if(sum%2==0) 
             { 
               sum=R2_databits[25];
             }
             else
             {
               printf("ERR_L2[%d]\n",sum);
               R2_code=0; 
               break;
             }
          }
          if(i==24)
          {
             if(sum%2==0) 
             {
               printf("ERR_H2[%d]\n",sum);
               R2_code=0; 
               break;
             }
          }
#endif   
      } 
    }
    else if (card_type==CARD_34_BIT)
    {
#ifdef PARITY_CHECK
      uint8_t sum=R2_databits[0];
#endif
      for (int i=1; i<33; i++)
      {
          R2_code=(R2_code<<1)|R2_databits[i];
#ifdef PARITY_CHECK
          if(R2_databits[i]) 
          {
            sum++;
          }
          if(i==16) 
          {
            if(sum%2==0) 
             { 
               sum=R2_databits[33];
             }
             else
             {
               printf("ERR_L2[%d]\n",sum);
               R2_code=0; 
               break;
             }
          }
          if(i==32)
          {
             if(sum%2==0) 
             {
               printf("ERR_H2[%d]\n",sum);
               R2_code=0; 
               break;
             }
          }
#endif
      }
    }
    // unmask_R2_irq();

    if((R2_last == R2_code))
    {
      goto end;
    }
    else 
    {
      R2_last = R2_code; 
      xTimerReset(tmr_R2_last,( TickType_t) 10);  
    }  


    memset(R2_databits,0, BufferSize*sizeof(R2_databits[0]));
    bzero(card_2,9);
    sprintf(card_2,"%08x",R2_code);
    esp_task_wdt_reset();
    if((R2_code!=4294967295)&&(R2_code!=0)&&(strlen(card_2)==8))
    {
      if (ismqtt)
      {
        printf("Reader|2|Card_Id: 0x%s   %d %d\n",card_2,mqttState, heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );        
        if(mqttState)publish_QC(card_2 ,2);
        goto end;
      }
      R2_state=false;      
      if(isConnected)
      {
        memset(R2_post_data,0,Data_Size*sizeof(R2_post_data[0]));
        if(isStandalone!=1)
        {
          printf("Reader|2|Card_Id: 0x%s   %d\n",card_2, heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
          sprintf(R2_post_data, "TAG|%d:%s",R2_call,card_2);
          // sprintf(R2_post_data, "TAG|2:%s",card_2);

          if(xQueueSendToFront(HFcardQueue, (void*)R2_post_data , (TickType_t)10 ) !=1)
          { 
                printf("could not sended this message = %s \n",R2_post_data); 
          }
          else 
          {
                 xEventGroupClearBits(R2_connection_flag, CONNECTED_BIT_R2);
                 esp_task_wdt_reset();
                 xSemaphoreGive(xSemaphoreWebsocket_R2);
                 xEventGroupWaitBits(R2_connection_flag,CONNECTED_BIT_R2, pdTRUE, pdFALSE, xTicksToWait);
          }
          lastdenied=R2_code;
        }              
      }      
      if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {ESP_LOGE("DS3231", "Could not get time.");}
      esp_task_wdt_reset();
      if(!R2_state)
      {
        permission_R2=0;
        if(is_exists(card_2))
        {
            if ((R2_call%2==1))
            {
              R1_OpenDoor();
            }
            else
            {
              R2_OpenDoor();
            }
            permission_R2=1;
        }
        else
        {
          permission_R2=0; 
          lastdenied=R2_code;
        }
        if(write_to_file(card_2,R2_call,permission_R2,rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec))
        {
          ESP_LOGI("R2_Log","%s,%d |%d: %d",card_2,permission_R2,recordCount,esp_get_free_heap_size() );
        }
        else
        {
          ESP_LOGE("Reader1","0x%s Unable to save Log %d",card_2,heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        } 
        if(isConnected)
        {
          memset(R2_post_data,0,Data_Size*sizeof(R2_post_data[0]));
          if(isStandalone==1)
          {
            if(mod_log==1)
            {
              sprintf(R2_post_data, "%s;%d;%d;%04d-%02d-%02d;%02d:%02d:%02d",card_2,R2_call,permission_R2,rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);
              if(xQueueSendToFront(HFcardQueue, (void*)R2_post_data , (TickType_t)10 ) !=1)
              { 
                  printf("could not sended this message = %s \n",R2_post_data);
              }
              else 
              {
                xSemaphoreGive(xSemaphoreWebsocket_R2);
              }
            }
          }
        }
      }
    }
    else {ESP_LOGI(TAG_ReadCard,"R2 Invaled Card ID %s",card_2);} 
    end:
    permission_R2=0;
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
    
 }
}

void  IRAM_ATTR R2_clear( TimerHandle_t xTimer )
{
  if (R2_counter==card_type)
  {
      if(eTaskGetState(taskRead_2)==eBlocked) 
      {
        vTaskSuspend(taskRead_2); 
      } 
      vTaskResume(taskRead_2);
    //  if(eTaskGetState(taskRead_2)==eRunning)
    //   {
    //     mask_R2_irq();
    //   } 
  }
  else
  {
      int _code=0;
      for (int i=0; i<R2_counter; i++)
      {
        _code=(_code<<1)|R2_databits[i];
      }
      ESP_LOGE("R2","Invaled Data, len[%d] ID:%x",R2_counter,_code);
      memset(R2_databits,0, BufferSize*sizeof(R2_databits[0]));        
  }
  R2_startReading =true;
  R2_counter = 0;
}

void  IRAM_ATTR R1_clear(TimerHandle_t xTimer )
{
  if (R1_counter==card_type)
  {
      if(eTaskGetState(taskRead_1)==eBlocked)
      {
      }       
      vTaskResume(taskRead_1);
      // if(eTaskGetState(taskRead_1)==eRunning)
      // {
      //   mask_R1_irq();
      // } 
  }
  else
  {
      int _code=0;
      for (int i=0; i<R1_counter; i++)
      {
        _code=(_code<<1)|R1_databits[i];
      }
        ESP_LOGE("R1","Invaled Data, len[%d] ID:%x",R1_counter,_code);
      memset(R1_databits,0, BufferSize*sizeof(R1_databits[0]));     
  }
  R1_startReading =true;
  R1_counter = 0;
}

void  IRAM_ATTR R1_lastcard(TimerHandle_t xTimer )
{
  R1_last=0;
}

void  IRAM_ATTR R2_lastcard(TimerHandle_t xTimer )
{
  R2_last=0;
}

static void mask_R1_irq(void)
{
    
    ESP_ERROR_CHECK(gpio_intr_disable(Data0R1));
    ESP_ERROR_CHECK(gpio_intr_disable(Data1R1));
}

static void unmask_R1_irq(void)
{
    ESP_ERROR_CHECK(gpio_intr_enable(Data0R1));
    ESP_ERROR_CHECK(gpio_intr_enable(Data1R1));
}

static void mask_R2_irq(void)
{
    ESP_ERROR_CHECK(gpio_intr_disable(Data0R2));
    ESP_ERROR_CHECK(gpio_intr_disable(Data1R2));
}

static void unmask_R2_irq(void)
{
    ESP_ERROR_CHECK(gpio_intr_enable(Data0R2));
    ESP_ERROR_CHECK(gpio_intr_enable(Data1R2));
}





