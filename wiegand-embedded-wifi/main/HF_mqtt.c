#include "HF_mqtt.h"


static double get_temperature();


void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE("MQTT", "Last error %s: 0x%x", message, error_code);
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;

    ESP_LOGD("MQTT", "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
    {
        mqttState=true;
        ESP_LOGI("MQTT", "MQTT_EVENT_CONNECTED");

        char sub_topic[48];
        memset(sub_topic,0,48*sizeof(char));
        sprintf(sub_topic,OUT1TPOIC,MAC);
        
        msg_id = esp_mqtt_client_subscribe(client, sub_topic, 1);
        ESP_LOGI("MQTT", "sent subscribe successful,{%s} msg_id=%d",sub_topic, msg_id);

        memset(sub_topic,0,48*sizeof(char));
        sprintf(sub_topic,OUT2TPOIC,MAC);
        msg_id = esp_mqtt_client_subscribe(client, sub_topic, 1);
        ESP_LOGI("MQTT", "sent subscribe successful,{%s}  msg_id=%d",sub_topic, msg_id);

        memset(sub_topic1,0,48*sizeof(char));
        memset(sub_topic2,0,48*sizeof(char));
        sprintf(sub_topic1, OUT1TPOIC ,MAC);
        sprintf(sub_topic2, OUT2TPOIC ,MAC);

        if(eTaskGetState(task_mqttBrodcast)==eSuspended){vTaskResume(task_mqttBrodcast);}

        break;
    }
    case MQTT_EVENT_DISCONNECTED:
        mqttState=false;
        if(eTaskGetState(task_mqttBrodcast)==eRunning) {vTaskSuspend(task_mqttBrodcast);}
        ESP_LOGI("MQTT", "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI("MQTT", "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI("MQTT", "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI("MQTT", "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
    {
        // if(event->data[0]==NULL) return;
        ESP_LOGI("MQTT", "MQTT_EVENT_DATA");
        // ESP_LOGI("MQTT", "%s",event->data);
        // ESP_LOGI("MQTT", "%d",(*event).data_len);
        // ESP_LOGI("MQTT", "%d",event->data_len);

        if(event->data_len !=0)
        {
            if(strncmp(event->topic, sub_topic1,event->topic_len)==0)
            {
                if(event->data[0]==OPEN)
                    OpenDoor_R1(); 
                else if( event->data[0]==CLOSE )
                    R1_CloseDoor();
            }
            if(strncmp(event->topic, sub_topic2,event->topic_len)==0)
            {
                if(event->data[0]== OPEN)
                    OpenDoor_R2();
                else if( event->data[0]==CLOSE)
                    R2_CloseDoor();
            }
        }
        break;
    }
    case MQTT_EVENT_ERROR:
        ESP_LOGI("MQTT", "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI("MQTT", "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg={
        .uri=MQTT_server,
        .username= MQTT_name,
        .password=MQTT_pass,
        .port=MQTT_port,
        .transport=MQTT_TRANSPORT_OVER_TCP,
    };
    
    // strcpy((char*) mqtt_cfg.uri,(const char *)  MQTT_server);
    // strcpy((char*) mqtt_cfg.username,(const char *)  MQTT_name);
    // strcpy((char*) mqtt_cfg.password,(const char *)  MQTT_pass);

    printf("Connect to %s\n",mqtt_cfg.uri);
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    int ret= esp_mqtt_client_start(client);
    memset(topic,0,40*sizeof(char));
    sprintf(topic,PUBTPOIC,MAC);  
    if (ret==ESP_OK ) ESP_LOGW("MQTT","STARTING MQTT CLIENT ");

}


void publish_QC(char* code , int reader)
{

   

    cJSON *QRFreame=cJSON_CreateObject();
    if (QRFreame == NULL){ESP_LOGE("MQTT","Failed to CREATE QRFreame OBJECT");   goto end;}

    cJSON *ID_item = cJSON_CreateString(MAC);
    if (ID_item == NULL){ESP_LOGE("MQTT","Failed to CREATE ID  OBJECT");  goto end;}

    cJSON *reader_item = cJSON_CreateNumber(reader);
    if (reader_item == NULL){ESP_LOGE("MQTT","Failed to CREATE reader OBJECT"); goto end;}

    cJSON *code_item = cJSON_CreateString(code);
    if (code_item == NULL) {ESP_LOGE("MQTT","Failed to CREATE Code OBJECT");   goto end;}

    cJSON *type_item=cJSON_CreateNumber(R_type);  
    if (type_item == NULL) {ESP_LOGE("MQTT","type_item to CREATE Id  OBJECT");  goto end; }
    cJSON *time_item=NULL;
    if(gettimeofday(&tv,NULL)==0)
    {
        time_item=cJSON_CreateNumber(tv.tv_sec);    
    }
    else 
    {
        time_item=cJSON_CreateNumber(tv.tv_sec);
    }
    if (time_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE time_item"); goto end;}
    cJSON_AddItemToObject(QRFreame,"gm",ID_item    );
    cJSON_AddItemToObject(QRFreame,"r" ,reader_item);
    cJSON_AddItemToObject(QRFreame,"ct",type_item  );
    cJSON_AddItemToObject(QRFreame,"v" ,code_item  );
    cJSON_AddItemToObject(QRFreame,"t" ,time_item  );

    char *content = cJSON_Print(QRFreame);
    reader = esp_mqtt_client_publish(client, topic , content , 0, 1, 0);
    free(content);
    // ESP_LOGI("MQTT", "sent publish successful, msg_id=%d", reader);
    goto end;
    end:
    cJSON_Delete(QRFreame);   
}


void mqtt_brodcast_handller (void *arg){

 ESP_LOGW("MQTT","Starting Task MQTT Brodcaster : %s ! ",pcTaskGetTaskName(NULL));
 int msg_id;
 char brodTopic[40];
 char devType[12];
 char bssid[23];
 wifi_ap_record_t apinfo;
 vTaskDelay(5000/portTICK_PERIOD_MS);

adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_MAX);  
bzero(devType,12);
switch(R_type){
        case 20: 
            sprintf(devType,"Barcode");
            break;
        case 21: 
            sprintf(devType,"UHFCard");
            break;
        case 22: 
            sprintf(devType,"FingerPrint");
            break;
        case 23: 
            sprintf(devType,"FaceId");
            break;
        case 24: 
            sprintf(devType,"Voice");
            break;
        default : 
            sprintf(devType,"Barcode");
                }
memset(bssid,0,23*sizeof(char));
memset(&apinfo, 0, sizeof(apinfo));  
memset(brodTopic,0,40*sizeof(char)); 
sprintf(brodTopic,BRODTPOIC,MAC);
while(1){
    if(mqttState){

                esp_wifi_sta_get_ap_info(&apinfo);
                msg_id=gettimeofday(&tv,NULL);
                sprintf(bssid,"%02X:%02X:%02X:%02X:%02X:%02X",apinfo.bssid[0], apinfo.bssid[1], apinfo.bssid[3], apinfo.bssid[4], apinfo.bssid[5], apinfo.bssid[6]);
                cJSON *root=cJSON_CreateObject();
                if (root == NULL){ESP_LOGE("MQTT","Failed to CREATE root OBJECT");    goto end;}

                cJSON *heap_item=cJSON_CreateNumber(heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
                if (heap_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE heap_item"); goto end;}
                
                cJSON *MAC_item = cJSON_CreateString(MAC);
                if (MAC_item == NULL){ESP_LOGE("MQTT","Failed to CREATE MAC_item");   goto end;}

                cJSON *IP_item = cJSON_CreateString(myIP);
                if (IP_item == NULL){ESP_LOGE("MQTT","Failed to CREATE IP_item");   goto end;}

                cJSON *SSID_item = cJSON_CreateString(st_SSID);
                if (SSID_item == NULL){ESP_LOGE("MQTT","Failed to CREATE SSID_item");   goto end;}

                cJSON *BSSID_item = cJSON_CreateString(bssid);
                if (BSSID_item == NULL){ESP_LOGE("MQTT","Failed to CREATE BSSID_item");   goto end;}

                cJSON *rssi_item=cJSON_CreateNumber(apinfo.rssi);
                if (rssi_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE rssi_item"); goto end;}

                cJSON *ver_item = cJSON_CreateString(Firmware);
                if (ver_item == NULL){ESP_LOGE("MQTT","Failed to CREATE ver_item");   goto end;}

                cJSON *IDFver_item = cJSON_CreateString(esp_get_idf_version());
                if (IDFver_item == NULL){ESP_LOGE("MQTT","Failed to CREATE IDFver_item");   goto end;}

                cJSON *volt_item=cJSON_CreateNumber(adc1_get_raw(ADC1_CHANNEL_6));
                if (volt_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE volt_item"); goto end;}

                cJSON *cpuTemp_item=cJSON_CreateNumber(get_temperature());
                if (cpuTemp_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE cpuTemp_item"); goto end;}

                cJSON *time_item=cJSON_CreateNumber(tv.tv_sec);
                if (time_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE time_item"); goto end;}

                cJSON *uptime_item=cJSON_CreateNumber(tv.tv_sec-upTime);
                if (uptime_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE uptime_item"); goto end;}

                cJSON *name_item = cJSON_CreateString(ap_ssid);
                if (name_item == NULL){ESP_LOGE("MQTT","Failed to CREATE name_item");   goto end;}

                cJSON *type_item = cJSON_CreateString(devType);
                if (type_item == NULL){ESP_LOGE("MQTT","Failed to CREATE type_item");   goto end;}

                cJSON *latitude_item = cJSON_CreateString(latitude);
                if (latitude_item == NULL){ESP_LOGE("MQTT","Failed to CREATE latitude_item");   goto end;}

                cJSON *longitude_item = cJSON_CreateString(longitude);
                if (longitude_item == NULL){ESP_LOGE("MQTT","Failed to CREATE longitude_item");   goto end;}

                cJSON *devicetype_item=cJSON_CreateNumber(1);
                if (devicetype_item == NULL){ ESP_LOGE("MQTT","Faild  to CREATE devicetype_item"); goto end;}


                cJSON_AddItemToObject(root,"heap"         ,heap_item     );
                cJSON_AddItemToObject(root,"mac"          ,MAC_item      );
                cJSON_AddItemToObject(root,"ip"           ,IP_item       );
                cJSON_AddItemToObject(root,"stationSSID"  ,SSID_item     );
                cJSON_AddItemToObject(root,"stationBSSID" ,BSSID_item    );             
                cJSON_AddItemToObject(root,"stationSignal",rssi_item     );
                cJSON_AddItemToObject(root,"version"      ,ver_item      );
                cJSON_AddItemToObject(root,"idfVersion"   ,IDFver_item   );
                cJSON_AddItemToObject(root,"volt"         ,volt_item     );
                cJSON_AddItemToObject(root,"cpuTemp"      ,cpuTemp_item  );
                cJSON_AddItemToObject(root,"time"         ,time_item     );
                cJSON_AddItemToObject(root,"uptime"       ,uptime_item   );
                cJSON_AddItemToObject(root,"name"         ,name_item     );
                cJSON_AddItemToObject(root,"type"         ,type_item     );
                cJSON_AddItemToObject(root,"latitude"     ,latitude_item );
                cJSON_AddItemToObject(root,"longitude"    ,longitude_item);
                cJSON_AddItemToObject(root,"deviceType"   ,devicetype_item);
            
                char *content = cJSON_Print(root);
                msg_id = esp_mqtt_client_publish(client, brodTopic , content , 0, 1, 0);
                free(content);
                ESP_LOGI("MQTT", "publish Broadcast successful, msg_id=%d\n", msg_id);
                goto end;
                end:
                cJSON_Delete(root);            
    }      
    ESP_LOGI("MQTT","Task Stack (bytes):[%d] ",uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI("MQTT","Heap (bytes):[%d] ",xPortGetFreeHeapSize());
 
    vTaskDelay(120000/portTICK_PERIOD_MS) ;

   }
}

static double get_temperature(void){

    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT2_REG, SENS_FORCE_XPD_SAR, 3, SENS_FORCE_XPD_SAR_S);
    SET_PERI_REG_BITS(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_CLK_DIV, 10, SENS_TSENS_CLK_DIV_S);
    CLEAR_PERI_REG_MASK(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_POWER_UP);
    CLEAR_PERI_REG_MASK(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_DUMP_OUT);
    SET_PERI_REG_MASK(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_POWER_UP_FORCE);
    SET_PERI_REG_MASK(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_POWER_UP);
    ets_delay_us(100);
    SET_PERI_REG_MASK(SENS_SAR_TSENS_CTRL_REG, SENS_TSENS_DUMP_OUT);
    ets_delay_us(5);
    int temp_farenheit = GET_PERI_REG_BITS2(SENS_SAR_SLAVE_ADDR3_REG, SENS_TSENS_OUT, SENS_TSENS_OUT_S);
    double temp_celsius = (temp_farenheit - 32) / 1.8;
    return temp_celsius;
}

// bakelor/pr14/prod/+/ 