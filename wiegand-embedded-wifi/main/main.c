#include "ds3231.h"
#include "globleVariables.h"
#include "HF_nvs.h"
#include "HF_cmd_list.h"
#include "HF_TCPsocket.h"
 #include "HF_httpserver.h"
#include "HF_gpio.h"
#include "HF_reader.h"
#include "HF_mqtt.h"
#include "HF_wifi.h"
#include "HF_sntp.h"
#include "HF_storage.h"

static int Timer_id_1;
static int Timer_id_2;
static int Timer_id_3;
static int Timer_id_4;
static int Timer_id_5;
static int Timer_id_6;
static int Timer_id_7;
static int rst_cnt=0;


void app_main()
{   
      esp_int_wdt_init(); 
      ESP_ERROR_CHECK(esp_task_wdt_init(WDT_TIMEOUT, true));
      ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
      mutex = xSemaphoreCreateMutex();
      if (ds3231_init_desc(&dev, I2C_NUM_0, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO) != ESP_OK) 
      {
        ESP_LOGE("DS3231", "Could not init device descriptor.");
        // esp_restart();
      }
      vTaskDelay(DELAY_10_MS);
      if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) 
      {
        ESP_LOGE("DS3231", " Could not get time.");
      }
      vTaskDelay(DELAY_10_MS);
      tv.tv_sec= get_unix_time(rtcinfo) ; 
      settimeofday(&tv, NULL);
      if(gettimeofday(&tv,NULL) == 0) 
      {
        upTime=tv.tv_sec;
      }         
      vTaskDelay(DELAY_10_MS);               
      if(init_nvs_hf(&nvsHandle)==false)
      {
        esp_restart();
      }
        
      if(init_storage()==false)
      {
        esp_restart();
      }
      vTaskDelay(DELAY_10_MS);
      flash_info();
      esp_task_wdt_reset(); 
      esp_log_level_set("wifi", ESP_LOG_NONE);              //ESP_LOG_DEBUG  ESP_LOG_WARN ESP_LOG_NONE ESP_LOG_ERROR 
      connection_flag = xEventGroupCreate();
      wifi_connection_flag =xEventGroupCreate();
      R1_connection_flag =xEventGroupCreate();
      R2_connection_flag =xEventGroupCreate(); 
      xSemaphoreTake(mutex, portMAX_DELAY);
      press=check_pressed();
      xSemaphoreTake(mutex, portMAX_DELAY);
      xEventGroupClearBits(connection_flag, CONNECTED_BIT_RESET);
      if(press==BTN_VAL_CLEAR_NVS) 
      {
        reset_user_values(&nvsHandle);
      }
      else xEventGroupSetBits(connection_flag,CONNECTED_BIT_RESET);
      xEventGroupWaitBits(connection_flag,CONNECTED_BIT_RESET, pdTRUE, pdFALSE, xTicksToWait);
      #ifdef  LOG_DBG
      if (writeDeviceLog(5))
      {
        ESP_LOGI("MAIN", "STORAGE SUCCESS");
      }
      #endif 
      xSemaphoreGive(mutex);
      ESP_ERROR_CHECK(esp_netif_init());
      vTaskDelay(DELAY_10_MS);
      ESP_ERROR_CHECK(esp_event_loop_create_default());
      vTaskDelay(DELAY_100_MS);
      xSemaphoreTake(mutex, portMAX_DELAY);
      get_settings(&nvsHandle);
      xSemaphoreTake(mutex, portMAX_DELAY);
      vTaskDelay(DELAY_200_MS); 
      if(press== BTN_VAL_ACCESSPOINT)
      { 
          ESP_LOGE("MAIN","AP PERMANENT /WiFi_MODE : %d",WiFi_mode);  
          wifi_init_softap(); 
      } 
      else
      {
          if (WiFi_mode==WIFI_MODE_STATION)                            
          {
            ESP_LOGE("MAIN","STA  /WiFi_MODE :%d",WiFi_mode); 
            start_sta();
          }    
          else if (WiFi_mode==WIFI_MODE_ACCESSPOINT)
          {
            ESP_LOGE("MAIN","AP SETTINGS /WiFi_MODE :%d",WiFi_mode);  
            wifi_init_softap(); 
          }      
          else 
          {
                ESP_LOGE("MAIN","APSTA /WiFi_MODE :%d",WiFi_mode); 
                tmr_startup=xTimerCreate("Startup Timer",pdMS_TO_TICKS(STARTUP_TIME),pdFALSE, (void*)Timer_id_3,&startup_mode);
                if(xTimerStart(tmr_startup,0)== pdPASS)
                { 
                  wifi_init_softap(); 
                }  
                else
                {
                  start_sta(); 
                  ESP_LOGE("MAIN","TIMER FAILED START STA MODE ");
                } 
          }
      }
          
      ESP_LOGE("MAIN","File Count :%d",show_file_list());
      xMutex = xSemaphoreCreateMutex();
      xTMutex=xSemaphoreCreateMutex();
      xSemaphoreWebsocket_R1= xSemaphoreCreateBinary();
      xSemaphoreWebsocket_R2= xSemaphoreCreateBinary();
      xSemaphoreInput_1_ON= xSemaphoreCreateBinary();
      xSemaphoreInput_1_OFF= xSemaphoreCreateBinary();
      // xSemaphoreInput_2= xSemaphoreCreateBinary();
      HFcardQueue=xQueueCreate(2,Data_Size*sizeof(char));
      if( HFcardQueue == 0 )
      {
        ESP_LOGE("MAIN","failed to create queue1= %p\nRestarting now.",HFcardQueue);                       // Failed to create the queue.
        fflush(stdout);
        esp_restart();
      }
      esp_task_wdt_reset();
      if(!ismqtt)
      {
        xTaskCreate(tcp_server_task, "tcp_server", 8096, (void*)AF_INET, 1, &taskTCPsocket);
      }
      tmr_R1_last=xTimerCreate("R1 last Timer",pdMS_TO_TICKS(wait_time *1000), pdFALSE, (void*)Timer_id_5,&R1_lastcard);
      tmr_R2_last=xTimerCreate("R1 last Timer",pdMS_TO_TICKS(wait_time *1000), pdFALSE, (void*)Timer_id_6,&R2_lastcard);
      tmr_R1=xTimerCreate("R1 Timer",pdMS_TO_TICKS(HOLD_READING_TIME), pdFALSE, (void*)Timer_id_1,&R1_clear);
      tmr_R2=xTimerCreate("R2 Timer",pdMS_TO_TICKS(HOLD_READING_TIME), pdFALSE, (void*)Timer_id_2,&R2_clear);
      tmr_R1_Relay = xTimerCreate("Relay Timer",pdMS_TO_TICKS(Out1_time*1000), pdFALSE, (void*)Timer_id_3, &R1_Turn_Relay);
      tmr_R2_Relay = xTimerCreate("Relay Timer",pdMS_TO_TICKS(Out2_time*1000), pdFALSE, (void*)Timer_id_4, &R2_Turn_Relay);
      tmr_IN1_last=xTimerCreate("IN1 Timer",pdMS_TO_TICKS(INPUT_DELAY), pdFALSE, (void*)Timer_id_7,&input1_Handler);
      xTaskCreatePinnedToCore(read_card_1, "Read_Card", 8096, NULL , 10 ,&taskRead_1,0);
      xTaskCreatePinnedToCore(read_card_2, "Read_Card", 8096, NULL , 10 ,&taskRead_2,1);
      xTaskCreate(del_flogs  ,"Del_FlogS" ,2048,  NULL , 3 ,&task_delFlogs);          
      vTaskDelay(DELAY_10_MS);     

      upload_profile(schedule_0,SHEDULE_NO_0);
      upload_profile(schedule_1,SHEDULE_NO_1);
      upload_profile(schedule_2,SHEDULE_NO_2);
      vTaskDelay(DELAY_10_MS);
      if(ismqtt)
      {                       
        ESP_LOGI("mqtt_TAG", "[MQTT CLIENT] Startup..");
        xTaskCreate(mqtt_brodcast_handller,"MQTT Brodcast" ,4096,  NULL , 3 ,&task_mqttBrodcast); 
        vTaskDelay(DELAY_100_MS);
        mqtt_app_start();
      }  
      esp_task_wdt_reset();        
      vTaskDelay(DELAY_10_MS);
      printf("----------------------------------HF I/O Controller------------------------------------ \n");
      ESP_LOGI("Time:","%04d-%02d-%02d %02d:%02d:%02d/ %02d",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, rtcinfo.tm_wday);
      ESP_LOGI(TAG_WiFi, "%s",MAC );
      ESP_LOGI("HEAP","FREE RAM:%d",heap_caps_get_free_size(MALLOC_CAP_DEFAULT)); 
      #ifdef  LOG_DBG
      if (writeDeviceLog(1))
      {
        ESP_LOGW("MAIN","%s RTC [%d] [%d]",pcTaskGetTaskName(NULL),(int)clock(),(int)time(NULL)); 
      }
      #endif
      
      reader1_config();
      reader2_config();
      init_output();

      if (WiFi_mode==WIFI_MODE_STATION)
      {
          ntp_service_init(); 
          ESP_LOGE("Main","%s %d %d ",ntpAddress,ntpState,ntpRate);
      }
      vTaskDelay(DELAY_10_MS); 
      
      while(1)
      {
          vTaskDelay(DELAY_1_SEC);
          esp_task_wdt_reset();
          if(rst_cnt>=RESET_WAITING_TIME)
          {
              ESP_LOGW("RESTART","Task Restart");
              rst_cnt=0;
              if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) 
              {
                ESP_LOGE("DS3231", "Could not get time.");
              }
              if ((rtcinfo.tm_hour==0)&&(rtcinfo.tm_min<11)&&(rtcinfo.tm_year>2021)) 
              {
                ESP_LOGE("Task", "Restart Esp32.");
                fflush(stdout);
                esp_restart();        
              }  
          }
          rst_cnt++; 
      }        
}

