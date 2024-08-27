#include "HF_nvs.h"

int get_str_key(nvs_handle_t handle, char* parameter,char* output)
{
	size_t string_size;
	esp_err_t err = nvs_get_str(handle, parameter, NULL, &string_size);
	if(err != ESP_OK) 
	{
	   	return 0;
	}
	char* value = (char*) malloc(string_size);
	err = nvs_get_str(handle, parameter, value, &string_size);
	if(err != ESP_OK) 
	{
		if(err == ESP_ERR_NVS_NOT_FOUND) 
			ESP_LOGE("NVS","Key %s not found", parameter);
		return 0;
	}
	strcpy(output,value);

    return 1;    
}

int add_str_key(nvs_handle_t handle, char* parameter1, char* parameter2 )
{

	if(!parameter1 || !parameter2)
	{
		ESP_LOGE("NVS"," No key or value provided!");
		return 0;
	}
    esp_err_t err = nvs_set_str(handle, parameter1, parameter2);
	if(err != ESP_OK) 
	{
		ESP_LOGE("NVS","Error in nvs_set_str! (%04X)", err);
		return 0;
	}
	err = nvs_commit(handle);
	if(err != ESP_OK) 
	{
		ESP_LOGE("NVS","Error in commit! (%04X)", err);
		return 0;
	}
	ESP_LOGI("NVS","Value %s stored in NVS with key %s", parameter2, parameter1);
    return 1;
}

int init_nvs_hf(nvs_handle_t* handle)
{
	esp_err_t err = nvs_flash_init();
	if(err != ESP_OK) 
	{
		ESP_LOGE("NVS","FATAL ERROR: Unable to init NVS");
		return false;
	}
	
	err = nvs_open("storage", NVS_READWRITE, handle);
    if (err != ESP_OK) {				
		ESP_LOGE("NVS","FATAL ERROR: Unable to open NVS");
		return false;		 
	}
	ESP_LOGI("NVS","NVS open OK"); 
	return true;  
} 

int get_nvs_int_value(nvs_handle_t nvsHandle,char* paramter)
{
	int32_t value = 0;
    esp_err_t err = nvs_get_i32(nvsHandle, paramter, &value);
		if(err != ESP_OK) 
		{
			if(err == ESP_ERR_NVS_NOT_FOUND) 
			{
				ESP_LOGW("NVS_GetInt","Key %s not found", paramter);
			}
			else
			{
				ESP_LOGE("NVS_GetInt","Error in nvs_get_i32! (%04X)", err);
			} 
			return 0;
		}
	return value;
 }
 
void set_nvs_int_value(nvs_handle_t nvsHandle,char* paramter,int value)
{
	esp_err_t  err = nvs_set_i32(nvsHandle, paramter, value);
	if(err != ESP_OK) 
	{
		ESP_LOGE("NVS","Error in nvs_set_i32! (%04X)", err);
		return;
	}
	err = nvs_commit(nvsHandle);
	if(err != ESP_OK)
	{
		ESP_LOGE("NVS","Error in commit! (%04X)", err);
		return;
	}

  }
	  
bool login_check(nvs_handle_t nvsHandle,char * loginId, char* password)
{
	char login_name[25];
	char login_pass[16];
	bzero(login_pass,16);
	bzero(login_name,25);

  	if(get_str_key(nvsHandle,LOGINID,login_name)==0) 
	{
    	ESP_LOGW("NVS","NO LOGIN NAME FOR LOGIN");
  	}
    if(get_str_key(nvsHandle,LOGINPASS,login_pass)==0)
	{
      	ESP_LOGW("NVS","NO LOGIN PASSWORD FOR STATION");
  	}
	if ((strcmp(loginId,login_name)==0)&&(strcmp(password,login_pass)==0))
		return true;
	else 
		return false; 
}
    
//Rest all NVS Values           
void reset_user_values(nvs_handle_t *nvsHandle)
{
    ESP_ERROR_CHECK(nvs_flash_erase());
		vTaskDelay(100 / portTICK_PERIOD_MS);
    if (init_nvs_hf(nvsHandle))
    {
        xEventGroupSetBits(connection_flag,CONNECTED_BIT_RESET);
        ESP_LOGE("NVS","Flash Ereased");
    }
    else 
    {
      esp_restart();
    }
}

void get_settings(nvs_handle_t *nvsHandle)
{

	if(get_nvs_int_value(*nvsHandle,R1_MODE))        
	{
		R1_call=get_nvs_int_value(*nvsHandle,R1_MODE); 
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,R1_MODE,R1_call);
	}
	
	if(get_nvs_int_value(*nvsHandle,R2_MODE))
    {
		R2_call=get_nvs_int_value(*nvsHandle,R2_MODE);
	}       
	else 
	{
		set_nvs_int_value(*nvsHandle,R2_MODE,R2_call);
	}

	if(get_nvs_int_value(*nvsHandle,G1_OPEN_TIME))
	{
		Out1_time=get_nvs_int_value(*nvsHandle,G1_OPEN_TIME);
	}  
	else 
	{
		set_nvs_int_value(*nvsHandle,G1_OPEN_TIME,Out1_time);
	}

	if(get_nvs_int_value(*nvsHandle,G2_OPEN_TIME))   
	{
		Out2_time=get_nvs_int_value(*nvsHandle,G2_OPEN_TIME);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,G2_OPEN_TIME,Out2_time);
	}

	if(get_nvs_int_value(*nvsHandle,READER_TYPE))   
	{
		R_type=get_nvs_int_value(*nvsHandle,READER_TYPE);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,READER_TYPE,R_type);
	}

	if(get_nvs_int_value(*nvsHandle,WiFi_MODE))
	{
		WiFi_mode=get_nvs_int_value(*nvsHandle,WiFi_MODE);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,WiFi_MODE,WiFi_mode);
	}

	if(get_nvs_int_value(*nvsHandle,USER_COUNT))     
	{
	USERCOUNT= get_nvs_int_value(*nvsHandle,USER_COUNT);
	USERCOUNT= get_users_count(); 
	}
	else 
	{ 
	USERCOUNT= get_users_count(); 
	userCounter=USERCOUNT; 
	set_nvs_int_value(*nvsHandle,USER_COUNTER,userCounter); 
	set_nvs_int_value(*nvsHandle,USER_COUNT,USERCOUNT);  
	ESP_LOGI("NVS","USER_COUNT  RESET  SUCCESSFULLY |%d",USERCOUNT);
	}     
	
	if(get_nvs_int_value(*nvsHandle,CARD_TYPE))   
	{
		card_type= get_nvs_int_value(*nvsHandle,CARD_TYPE);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,CARD_TYPE,card_type);
	}

	if(get_nvs_int_value(*nvsHandle,READER_MODE))   
	{
		isStandalone=get_nvs_int_value(*nvsHandle,READER_MODE);
	} 
	else 
	{
		set_nvs_int_value(*nvsHandle,READER_MODE,isStandalone);
	}

	if(get_nvs_int_value(*nvsHandle,LOG_MODE))      
	{
		mod_log=get_nvs_int_value(*nvsHandle,LOG_MODE);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,LOG_MODE,mod_log);
	}

	if(get_nvs_int_value(*nvsHandle,PORT))          
	{
		port=get_nvs_int_value(*nvsHandle,PORT);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,PORT,port);
	}

	if(get_nvs_int_value(*nvsHandle,WAITTIME))      
	{
		wait_time=get_nvs_int_value(*nvsHandle,WAITTIME);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,WAITTIME,wait_time);
	}
		
	if(get_nvs_int_value(*nvsHandle,NTPSTATE))   
	{
		ntpState= get_nvs_int_value(*nvsHandle,NTPSTATE); 
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,NTPSTATE,ntpState); 
	} 

	if(get_nvs_int_value(*nvsHandle,NTPRATE))   
	{
		ntpRate = get_nvs_int_value(*nvsHandle,NTPRATE); 
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,NTPRATE,ntpRate); 
	} 

	if(get_nvs_int_value(*nvsHandle,MQTTPORT))     
	{
		MQTT_port=get_nvs_int_value(*nvsHandle,MQTTPORT); 
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,MQTTPORT,MQTT_port);
	}

	if(get_nvs_int_value(*nvsHandle,MQTTSTATE))    
	{
		ismqtt=get_nvs_int_value(*nvsHandle,MQTTSTATE);
	}
	else 
	{
		set_nvs_int_value(*nvsHandle,MQTTSTATE,ismqtt);
	}

	if(get_str_key(*nvsHandle,LOGINID,login_name)==0)  
	{
		if(add_str_key(*nvsHandle, LOGINID, login_name))
		{
			ESP_LOGI("NVS","LOGIN PASS SAVED SUCCESSFULLY ");
		}
	}  
	
	if(get_str_key(*nvsHandle,LOGINPASS,login_pass)==0) 
	{
		if(add_str_key(*nvsHandle, LOGINPASS, login_pass)) 
		{
			ESP_LOGI("NVS","LOGIN PASS SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,FIRMWARE,Firmware)==0) 
	{
		if(add_str_key(*nvsHandle, FIRMWARE, Firmware)) 
		{
			ESP_LOGI("NVS","FIRMWARE VER. SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,MQTTSERVER,MQTT_server)==0) 
	{
		if(add_str_key(*nvsHandle, MQTTSERVER, MQTT_server)) 
		{
			ESP_LOGI("NVS","MQTTSERVER SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,MQTTUSER,MQTT_name)==0) 
	{
		if(add_str_key(*nvsHandle, MQTTUSER, MQTT_name)) 
		{
			ESP_LOGI("NVS","MQTTUSER SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,MQTTPASS,MQTT_pass)==0) 
	{
		if(add_str_key(*nvsHandle, MQTTPASS, MQTT_pass)) 
		{
			ESP_LOGI("NVS","MQTTPASS SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,AP_IP,ap_ip)==0) 
	{
		if(add_str_key(*nvsHandle, AP_IP,ap_ip)) 
		{
			ESP_LOGI("NVS","ACCESS POINT IP SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle,AP_NETMASK,netmask)==0) 
	{
		if(add_str_key(*nvsHandle,AP_NETMASK,netmask))
		{
			ESP_LOGW("NVS","NETMASK  SAVED SUCCESSFULLY");
		} 
	}

	if(get_str_key(*nvsHandle,AP_GATEWAY,gateway)==0) 
	{
		if(add_str_key(*nvsHandle,AP_GATEWAY,gateway)) 
		{
			ESP_LOGW("NVS","AP_GATEWAY SAVED SUCCESSFULLY");
		}
	}
		
	if(get_str_key(*nvsHandle, LONGITUDE, longitude)==0)  
	{
		if(add_str_key(*nvsHandle, LONGITUDE, longitude)) 
		{
			ESP_LOGI("NVS","LONGITUDE SAVED SUCCESSFULLY ");
		}
	}

	if(get_str_key(*nvsHandle, LATITUDE, latitude)==0)  
	{
		if(add_str_key(*nvsHandle, LATITUDE, latitude)) 
		{
			ESP_LOGI("NVS","LATITUDE SAVED SUCCESSFULLY ");
		}
	}
	
	if(get_str_key(*nvsHandle,NTPADDRS, ntpAddress)==0)
	{
		if(add_str_key(*nvsHandle, NTPADDRS, ntpAddress)) 
		{
			ESP_LOGI("NVS","NTP Address SUCCESSFULLY ");
		}
	} 

	if(get_str_key(*nvsHandle,AP_PASSWORD,ap_pass)==0) 
	{
		if(add_str_key(*nvsHandle,AP_PASSWORD,ap_pass ))
		{
			ESP_LOGW("AP_WiFi","NO PASS FOR AP, USE DEFUALT %s",ap_pass);
		} 
		
	}
	
	if(get_str_key(*nvsHandle,AP_SSID,ap_ssid)==0)
	{ 
		uint8_t mac[6];
		memset(mac,0,6*sizeof(char));
		esp_read_mac(mac,ESP_MAC_WIFI_SOFTAP);
		bzero (ap_ssid,25);
		sprintf(ap_ssid,"Wiegand %02x%02x%02x",mac[3],mac[4],mac[5]-1);
		if(add_str_key(*nvsHandle,AP_SSID, ap_ssid )) 
		{
			ESP_LOGW("AP_WiFi","NO SSID  FOR AP, USE DEFUALT %s",ap_ssid); 
		}			
	} 

	recordCount= get_total_logs(&f_logCount);

	if(get_nvs_int_value(*nvsHandle,USER_COUNTER))   
	{
		userCounter= get_nvs_int_value(*nvsHandle,USER_COUNTER);
	}

	ESP_LOGE("MAIN","USER COUNT:%d ",USERCOUNT);
	ESP_LOGE("MAIN","USER COUNTER:%d ",userCounter);
	ESP_LOGE("NVS_","LOG FILES COUNT:%d",f_logCount);        
	ESP_LOGE("NVS_Getlogin","LoginID:%s  Password:*******",login_name);
	ESP_LOGE("NVS_","Record Count %d",recordCount);
	ESP_LOGE("NVS_","Card Type %d",card_type);  

	xSemaphoreGive(mutex);
}     

