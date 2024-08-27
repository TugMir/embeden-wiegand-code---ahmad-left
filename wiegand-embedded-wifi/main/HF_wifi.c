#include "HF_wifi.h"

inline void get_mac(void);
static uint8_t cnret=0;
static uint8_t  collision =0;
static esp_err_t ret=0;
static esp_netif_t *ap_netif;
static esp_netif_t *sta_netif;
static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;
static wifi_scan_config_t scan_config =
{
    .ssid = 0,
    .bssid = 0,
    .channel = 0,
    .show_hidden = true,
    .scan_type=WIFI_SCAN_TYPE_ACTIVE,
};


static void sta_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
            ESP_LOGW(TAG_WiFi,"Connect Wifi 1");
            if((mode==STATION_MODE))
            {
                ret=esp_wifi_connect();  
                ESP_LOGW(TAG_WiFi,"Connect Wifi 1:mode:%d  err:%d",mode,ret);
            }
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP) 
    {
            ESP_LOGW(TAG_WiFi,"WIFI_EVENT_STA_STOP"); 
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {  
            ESP_LOGE(TAG_WiFi,"WIFI_EVENT_STA_DISCONNECTED %d",status);  
            status =false;
            if(ntp_Status)
            {
                ntp_stop_RQ();
            }
            
            if(retry==TRY_COUNT) 
            {
                retry++; 
                xEventGroupSetBits(wifi_connection_flag,CONNECTED_BIT);
            } 
            if(mode ==STATION_MODE)  
            {
                if (httpstate)
                { 
                    stop_webserver(server); 
                    ESP_LOGW(TAG_WiFi,"_Disconnected 2");
                } 
                if(!ismqtt)
                {
                    if(eTaskGetState(taskTCPsocket)==eRunning) 
                    {
                        vTaskSuspend(taskTCPsocket);
                    }
                }
            }
            else if(mode==ACCESSPOINT_MODE)  
            {
                if (retry<TRY_COUNT)
                {
                    retry++; 
                    ESP_ERROR_CHECK(esp_wifi_connect());
                    ESP_LOGE(TAG_WiFi,"Disconnected Swiching");
                }
            }
            #ifdef  LOG_DBG
            if (writeDeviceLog(3))
                    ESP_LOGI("WIFI", "LOG_3");
            #endif
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    { 
            collision =0;
            #ifdef  LOG_DBG
            if (writeDeviceLog(2))
            {
                ESP_LOGI("WIFI", "IP_EVENT_STA_GOT_IP..");
            }  
            #endif              
            status=true;                            
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            bzero(myIP,16);
            sprintf(myIP,"%d.%d.%d.%d",IP2STR(&event->ip_info.ip));
            ESP_LOGI(TAG_WiFi, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
            server = start_webserver();         
            if(!ismqtt)
            {
                if(eTaskGetState(taskTCPsocket)==eSuspended)
                {
                    vTaskResume(taskTCPsocket);
                }
            } 
            ESP_LOGI(TAG_WiFi, "connected");
            xEventGroupSetBits(wifi_connection_flag,CONNECTED_BIT);
            if(!ntp_Status)
            {
                ntp_restart_RQ();
            }
            

            // ntp_service_init();  
               
    }
    else if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
            if ((WiFi_mode==WIFI_MODE_MIXED)&&(press!=BTN_VAL_ACCESSPOINT)) 
            {
                xTimerStop(tmr_startup,0);
            }
            wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
            ESP_LOGW(TAG_WiFi, "station "MACSTR" join, AID=%d",MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
            wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
            ESP_LOGW(TAG_WiFi, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
    }
    else if(event_id == SYSTEM_EVENT_SCAN_DONE)
    {
        // ESP_LOGI(TAG_WiFi, "SYSTEM_EVENT_SCAN_DONE");
    }          
}

//WiFi Sta  Handler
void wifi_sta_start()
{ 
    mode=STATION_MODE;
    get_mac();
    if (WiFi_mode!=WIFI_MODE_STATION)
    {      // check if call comes from Ap mode 
            ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT,ESP_EVENT_ANY_ID,&sta_event_handler));
            ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT,IP_EVENT_STA_GOT_IP,&sta_event_handler));
            ESP_ERROR_CHECK(esp_wifi_deinit());
    }

    if(!sta_netif)
    { 
        sta_netif = esp_netif_create_default_wifi_sta(); 
    }
    assert(sta_netif);
    vTaskDelay(DELAY_1_SEC);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.mgmt_sbuf_num = WIFI_MGMT_SBUF;
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&sta_event_handler,NULL,&instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&sta_event_handler,NULL,&instance_got_ip));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM)); 
    wifi_config_t wifi_config={0};
    wifi_config.sta.channel=0;
    strcpy((char*)wifi_config.sta.ssid, (const char*)st_SSID);
    strcpy((char*)wifi_config.sta.password, (const char*)st_pass);
    wifi_config.sta.scan_method = WIFI_FAST_SCAN;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifi_config.sta.threshold.rssi = -100;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));      
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    vTaskDelay(DELAY_500_MS);
    ESP_LOGW("sta_WiFi","SSID:%s   pass:%s", wifi_config.sta.ssid, wifi_config.sta.password);
    ESP_ERROR_CHECK(esp_wifi_start());    
   xTaskCreatePinnedToCore (wifi_task,"wifi_task",	4096,NULL, 4, wifi_handler, 1);              // Create Wifi Monitoring Task 
}

//Task to monitor Wifi Sta connection State 
void wifi_task(void *arg)
{
    vTaskDelay(DELAY_5_SEC);
    ESP_LOGW("sta_WiFi","Start Wifi Task Monitor");
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    uint8_t scf_cnt=0;
    volatile int8_t rssi_val=0;
    while(1)
    {
        esp_task_wdt_reset();             
        if(!status)
        {  
            ret=esp_wifi_connect();
            ESP_LOGW("sta_WiFi","Disconnected.Task.1  err:%d",ret);
            esp_task_wdt_reset();
            if (cnret==60)
            {
                cnret=0;
                ESP_ERROR_CHECK(esp_wifi_stop());
                esp_task_wdt_reset();
                ESP_LOGW("sta_WiFi","Disconnected.Task.2  err:%d",ret);
                vTaskDelay(DELAY_500_MS);
                ESP_ERROR_CHECK(esp_wifi_start());
                esp_task_wdt_reset();
                vTaskDelay(DELAY_300_MS);
            }
            cnret++;
        }
        else 
        {
            if (scf_cnt==12)
            {
                scf_cnt=0;
                ret=esp_wifi_scan_start(&scan_config, true); 
                uint16_t number = DEFAULT_SCAN_LIST_SIZE;
                wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
                uint16_t ap_count = 0;

                memset(ap_info, 0, sizeof(ap_info));
                ret=esp_wifi_scan_get_ap_records(&number, ap_info);
                ret=esp_wifi_scan_get_ap_num(&ap_count);
                if(ap_count >DEFAULT_SCAN_LIST_SIZE) ap_count=DEFAULT_SCAN_LIST_SIZE;
                APnum=ap_count;
                rssi_val=0;                    
                for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) 
                {
                    if (strcmp((const char*) ap_info[i].ssid,(const char*)st_SSID)==0)
                    {
                        rssi_val=ap_info[i].rssi;                            
                    }                  
                }
                if(rssi_val<RSSI_THRESHOLD)
                {
                    ret=esp_wifi_disconnect();
                    ESP_LOGW("WiFi","wifi connection disconnected [%d]!!!!",rssi_val);
                }   

            }          
            else 
            {
                scf_cnt++;
            }


        }
        vTaskDelay(DELAY_5_SEC);
    }
}

//Access Point Handler
void wifi_init_softap()
{ 
    mode=ACCESSPOINT_MODE;
    get_mac();
    vTaskDelay(DELAY_500_MS);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.mgmt_sbuf_num = WIFI_MGMT_SBUF;
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));    
    esp_netif_ip_info_t info_t;
    memset(&info_t, 0, sizeof(esp_netif_ip_info_t));
    ap_netif = esp_netif_create_default_wifi_ap();
    sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    if (ap_netif){
           ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));
           info_t.ip.addr = esp_ip4addr_aton((const char *)ap_ip);
           info_t.netmask.addr = esp_ip4addr_aton((const char *)netmask);
           info_t.gw.addr = esp_ip4addr_aton((const char *)gateway);
           esp_netif_set_ip_info(ap_netif, &info_t);
           ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));
    } 
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&sta_event_handler,NULL,&instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&sta_event_handler,NULL,&instance_any_id));
    wifi_config_t ap_config = 
    {
        .ap = 
        {
            .channel = AP_CHANNEL,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = AP_MAX_CONN,
        },
    }; 


    char _ap_ssid[25];
    char _ap_pass[17];

    bzero (_ap_ssid,25);
    bzero(_ap_pass,17);
   
    if(get_str_key(nvsHandle,AP_SSID,_ap_ssid)) 
    {
        bzero (ap_ssid,25);
        strcpy(ap_ssid,_ap_ssid);      
    }
    else
    {
        uint8_t mac[6];
        memset(mac,0,6*sizeof(char));
        esp_read_mac(mac,ESP_MAC_WIFI_SOFTAP);
        bzero (ap_ssid,25);
        sprintf(ap_ssid,"Wiegand %02X%02X%02X",mac[3],mac[4],mac[5]-1);  
        if(add_str_key(nvsHandle,AP_SSID, ap_ssid )) ESP_LOGW("AP_WiFi","NO SSID  FOR AP, USE DEFUALT %s",ap_ssid);
    }

    if(get_str_key(nvsHandle,AP_PASSWORD,_ap_pass))
    {
            bzero(ap_pass,17);
            strcpy(ap_pass,_ap_pass);
    }
    else
    {
        ESP_LOGW("AP_WiFi","NO PASS  FOR AP, USE DEFUALT %s",ap_pass);
    }

    strcpy((char*)ap_config.ap.ssid, (char*)ap_ssid);
    strcpy((char*)ap_config.ap.password, (char*)ap_pass);
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE)); 
	ESP_ERROR_CHECK(esp_wifi_start());    
    ESP_LOGE("AP_MODE","AP started Network:%s ip:%s",ap_ssid,ap_ip);
    server = start_webserver();
    #ifdef  LOG_DBG
    if (writeDeviceLog(4))
          ESP_LOGI("MAIN", "AP SUCCESS..");
    #endif
}

//Metode to check Connection to New WiFi Network 
void connect_wifi(char* ssid,char* pass)
{ 
    if(mode==ACCESSPOINT_MODE)
    {
        ret=esp_wifi_disconnect();
        wifi_config_t wifi_config={0};
        wifi_config.sta.channel=0;
        strcpy((char*)wifi_config.sta.ssid, (const char*)ssid);
        strcpy((char*)wifi_config.sta.password, (const char*)pass);
        wifi_config.sta.scan_method = WIFI_FAST_SCAN;
        wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
        wifi_config.sta.threshold.rssi = -100;
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
        ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));      
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        vTaskDelay(DELAY_500_MS);
        ret=esp_wifi_connect();
        ESP_LOGW("WiFi_CONNECT","From Ap Connect to SSID:%s ", wifi_config.sta.ssid);
    }
    else 
    {
        mode=STATION_2_STA_MODE;
        ret=esp_wifi_stop();
        wifi_config_t wifi_config={0};
        wifi_config.sta.channel=0;
        strcpy((char*)wifi_config.sta.ssid, (const char*)ssid);
        strcpy((char*)wifi_config.sta.password, (const char*)pass);
        wifi_config.sta.scan_method = WIFI_FAST_SCAN;
        wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
        wifi_config.sta.threshold.rssi = -100; 
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        vTaskDelay(DELAY_500_MS);
        ESP_ERROR_CHECK(esp_wifi_start());
        ESP_LOGW("WiFi_CONNECT","From STA Try to connect: %s ...\n",ssid);      

    }
}

void scann(void)
{
    ESP_LOGW(TAG_WiFi,"Start scanning...|%d \n",heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    ret=esp_wifi_scan_start(&scan_config, true); 
    print_scan();
}

inline void get_mac(void)
{
    uint8_t mac[MAC_ADD_LEN];
    memset(mac,0,MAC_ADD_LEN*sizeof(char));
    esp_read_mac(mac,ESP_MAC_WIFI_SOFTAP);
    sprintf(MAC,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]-1);
}

void print_scan(void)
{
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    if(ap_count >DEFAULT_SCAN_LIST_SIZE) ap_count=DEFAULT_SCAN_LIST_SIZE;
    APnum=ap_count;

    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) 
    {
        // ESP_LOGI("TAG", "SSID \t\t%s", ap_info[i].ssid);
        // ESP_LOGI("TAG", "RSSI \t\t%d", ap_info[i].rssi);
        // ESP_LOGI("TAG", "Channel \t\t%d\n", ap_info[i].primary);
        wifi_Ap_list[i].rssi=ap_info[i].rssi;
        bzero(wifi_Ap_list[i].ssid,33);
        sprintf(wifi_Ap_list[i].ssid,"%s",ap_info[i].ssid);
    }
    xEventGroupSetBits(connection_flag,CONNECTED_BIT_scanRes);
}

// Method for Change Mode from AP to STA
void start_sta(void)
{
   if(get_str_key(nvsHandle,STA_SSID,st_SSID)==0) 
   {
      ESP_LOGE("Wifi MAIN","NO SSID FOR STATION Mode, Change to MIXED Mode and Restart");
      set_nvs_int_value(nvsHandle,WiFi_MODE,WIFI_MODE_MIXED);
      fflush(stdout);
      esp_restart();
   }
  if(get_str_key(nvsHandle,STA_PASS,st_pass)==0) 
  {
      ESP_LOGE("Wifi MAIN","NO PASSWord FOR STATION Mode, Change to MIXED Mode and Restart");
      set_nvs_int_value(nvsHandle,WiFi_MODE,WIFI_MODE_MIXED);
      fflush(stdout);
      esp_restart();
  }
  wifi_sta_start();  
}

//Access Point Timer Method 
void startup_mode( TimerHandle_t xTimer )
{
  bool isExist=true;
  if(get_str_key(nvsHandle,STA_SSID,st_SSID)==0) 
  {
    isExist=false ;
  }
  if(get_str_key(nvsHandle,STA_PASS,st_pass)==0) 
  {
    isExist=false ;
  }
  if(isExist)
  {
      stop_webserver(server);
      ESP_ERROR_CHECK(esp_wifi_stop());
      start_sta();
      ESP_LOGI("WiFi_Timer","********** Timer STA Mode **********"); 
  }
  else
  {
      ESP_LOGI("WiFi_Timer","NO SSID OR Password has been set for WiFi Station Mode for station mode, Stay On WiFi Access Mode");
  } 
     
}
