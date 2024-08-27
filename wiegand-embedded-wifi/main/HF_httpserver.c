#include "HF_httpserver.h"


static const char *HTTP_TAG = "HTTP";

static esp_err_t style_handler(httpd_req_t *req);

static esp_err_t runtimeJS_handler(httpd_req_t *req);

static esp_err_t polyfillsJS_handler(httpd_req_t *req);

static esp_err_t mainJS_handler(httpd_req_t *req);

static esp_err_t logo_handler(httpd_req_t *req);

static esp_err_t index_get_handler(httpd_req_t *req);

static esp_err_t nodata_handler(httpd_req_t *req);

static esp_err_t del_all_users(httpd_req_t* req);

static esp_err_t login_handler(httpd_req_t *req);

static esp_err_t set_login_handler(httpd_req_t *req);

static esp_err_t set_timeDate_handler(httpd_req_t *req);

static esp_err_t get_timeDate_handler(httpd_req_t *req);

static esp_err_t updateUser_post_handler(httpd_req_t *req);

static esp_err_t adduser_post_handler(httpd_req_t *req);

static esp_err_t delusers_post_handler(httpd_req_t *req);

static esp_err_t log_handler(httpd_req_t *req);

static esp_err_t del_log_handler(httpd_req_t *req);

static esp_err_t user_list_handler(httpd_req_t *req);

static esp_err_t sta_post_handler(httpd_req_t *req);

static esp_err_t uplodlist_post_handler(httpd_req_t *req);

static esp_err_t wifiscan_handler(httpd_req_t *req);

static esp_err_t wificonnect_handler(httpd_req_t *req);

static esp_err_t apSettings_post_handler(httpd_req_t *req);

static esp_err_t staSettings_post_handler(httpd_req_t *req);

static esp_err_t apSettings_get_handler(httpd_req_t *req);

static esp_err_t gate_settings_handler(httpd_req_t *req);

static esp_err_t get_gate_settings_handler(httpd_req_t *req);

static esp_err_t get_lastdenied_handler(httpd_req_t *req);

static esp_err_t setschedule_handler (httpd_req_t *req);

#ifdef  LOG_DBG
static esp_err_t del_device_log_get_handler (httpd_req_t *req);

static esp_err_t device_log_get_handler (httpd_req_t *req);

#endif

static esp_err_t getschedule_handler (httpd_req_t *req);

static esp_err_t reset_post_handler (httpd_req_t *req);

static esp_err_t getMQTT_handler (httpd_req_t *req);

static esp_err_t setMQTT_handler (httpd_req_t *req);

static esp_err_t multiusers_post_handler(httpd_req_t *req);

static esp_err_t add_multi_users(cJSON *root,uint8_t * status , int count );

static esp_err_t delete_multi_users(cJSON *root,uint8_t * status , int count );

static esp_err_t update_multi_users(cJSON *root,uint8_t * status , int count );

static esp_err_t add_multi_users_lite(cJSON *root,uint8_t * status , int count ); 


static esp_err_t style_handler(httpd_req_t *req)
{
   extern const uint8_t style_css_start[] asm("_binary_styles_css_gz_start");
   extern const uint8_t style_css_end[] asm("_binary_styles_css_gz_end"); 
   httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
   httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
   httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
   httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
   httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
   httpd_resp_set_type(req, "text/css");
   httpd_resp_send(req, (const char *)style_css_start, (style_css_end-1) - style_css_start);
   return ESP_OK;
}

static esp_err_t runtimeJS_handler(httpd_req_t *req)
{
    extern const uint8_t runtime_js_start[] asm("_binary_runtime_js_gz_start");
    extern const uint8_t runtime_js_end[] asm("_binary_runtime_js_gz_end");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)runtime_js_start, (runtime_js_end-1) - runtime_js_start);
    return ESP_OK;
}

static esp_err_t polyfillsJS_handler(httpd_req_t *req)
{
    
    extern const uint8_t polyfills_js_start[] asm("_binary_polyfills_js_gz_start");
    extern const uint8_t polyfills_js_end[] asm("_binary_polyfills_js_gz_end");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)polyfills_js_start, (polyfills_js_end-1) - polyfills_js_start);
    return ESP_OK;
}

static esp_err_t mainJS_handler(httpd_req_t *req)
{
    extern const uint8_t main_js_start[] asm("_binary_main_js_gz_start");
    extern const uint8_t main_js_end[] asm("_binary_main_js_gz_end");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)main_js_start, (main_js_end-1) - main_js_start);
    return ESP_OK;
}
  
static esp_err_t logo_handler(httpd_req_t *req)
{
    extern const uint8_t logo_png_start[] asm("_binary_logo_png_start");
    extern const uint8_t logo_png_end[] asm("_binary_logo_png_end");
    httpd_resp_send(req, (const char *)logo_png_start, (logo_png_end-1) - logo_png_start);
    return ESP_OK;
}

static esp_err_t index_get_handler(httpd_req_t *req)
{   
    extern const uint8_t index_html_start[] asm("_binary_index_html_start");
    extern const uint8_t index_html_end[]   asm("_binary_index_html_end");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, (index_html_end-1) - index_html_start);
    return ESP_OK;
}
 
static esp_err_t nodata_handler(httpd_req_t *req){
    extern const uint8_t nodata_png_start[] asm("_binary_nodata_png_start");
    extern const uint8_t nodata_png_end[] asm("_binary_nodata_png_end");
    httpd_resp_send(req, (const char *)nodata_png_start, (nodata_png_end-1) - nodata_png_start);
    return ESP_OK;
}

static esp_err_t del_all_users(httpd_req_t* req)
{
    int count=0,status=0;
    struct dirent *entry;
    DIR *dir = opendir("/spiffs"); 
    if (dir == NULL) 
    {
        httpd_resp_sendstr(req,"false");
        return false;
    }
    while ((entry = readdir(dir)) != NULL) 
    {
        char name[FILE_NAME_LENGTH];
        bzero (name,FILE_NAME_LENGTH);
        strncpy(name,entry->d_name,CARD_ID_LENGTH);
        if ((strncmp(entry->d_name,"log",COMPAER_LEN_3)==OK_RESULT)||((strncmp(entry->d_name,"sch",COMPAER_LEN_3)==OK_RESULT)))
        {
            continue;
        } 
        if(USERCOUNT>0)
        {
            int err=del_user(name);
            if(err==1)
            {
                ESP_LOGW(HTTP_TAG, "User Removed ");
                --USERCOUNT;                
            }
            else
            {
                status++;
            } 
        }
       count++;
    }

    closedir (dir);
    ESP_LOGI(HTTP_TAG,"Deleted user count %d",count);
    closedir(dir);
    // set_nvs_int_value(nvsHandle,USER_COUNT,USERCOUNT);

    if (status>0)
    {
        httpd_resp_sendstr(req, "false");
        set_nvs_int_value(nvsHandle,USER_COUNT,USERCOUNT);
    }
    else 
    {
        set_nvs_int_value(nvsHandle,USER_COUNTER,0); 
        set_nvs_int_value(nvsHandle,USER_COUNT,0);
        userCounter=0;
        httpd_resp_sendstr(req, "true");
    }
   return ESP_OK;
}

static esp_err_t login_handler(httpd_req_t *req)
{
    char buf[128];
    memset(buf,0,128*sizeof(char));
    int ret=0;
    int remaining = req->content_len;
    ESP_LOGI(HTTP_TAG,"Received len %d",remaining);
    if ((ret = httpd_req_recv(req, buf,MIN(remaining, sizeof(buf)))) <= OK_RESULT) 
    {
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }
    ESP_LOGI(HTTP_TAG, "RECEIVED DATA %d:%s", ret, buf);
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 

    char loginId[25];
    char password[17];
    memset(loginId,0,25*sizeof(char));
    memset(password,0,17*sizeof(char));

    cJSON *root = cJSON_Parse(buf);
    if(root == NULL){
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }

    cJSON *loginId_item=cJSON_GetObjectItemCaseSensitive(root,"LoginId");
    if (!cJSON_IsString(loginId_item)||(loginId_item->valuestring==NULL))
    {
        ESP_LOGE(HTTP_TAG,"LoginId is invalid"); httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    cJSON *password_item= cJSON_GetObjectItemCaseSensitive(root,"Passwrd");
    if(!cJSON_IsString(password_item)||(password_item->valuestring==NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"Password is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    strcpy(loginId,loginId_item->valuestring);
    strcpy(password,password_item->valuestring);

    ESP_LOGI(HTTP_TAG,"Received LOGIN ID:%s;Password:%s",loginId,password);

    if (login_check(nvsHandle,loginId,password)){
            httpd_resp_sendstr(req, "true");
             ESP_LOGI(HTTP_TAG,"Login successfully ");
             goto end;}
    else {
            httpd_resp_sendstr(req, "false");
            ESP_LOGI(HTTP_TAG,"Login Faild");
            goto end;}
    goto end;

    end:
        cJSON_Delete(root);
        return ESP_OK;
}

static esp_err_t set_login_handler(httpd_req_t *req)
{
    char buf[BUFFER_128_B_LEN];
    memset(buf,0,BUFFER_128_B_LEN*sizeof(char));
    int ret=0;
    int len = req->content_len;
    ESP_LOGI(HTTP_TAG,"Received len %d ",len);
    if ((ret = httpd_req_recv(req, buf,len)) <= 0) 
    {
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 

    char loginId[17];
    char password[17];
    char temp[17];

    memset(loginId,0,17*sizeof(char));
    memset(password,0,17*sizeof(char));
    memset(temp,0,17*sizeof(char));

    cJSON *root = cJSON_Parse(buf);
    if(root == NULL)
    {
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }

    cJSON *loginId_item=cJSON_GetObjectItemCaseSensitive(root,"Oldpassword");
    if (!cJSON_IsString(loginId_item)||(loginId_item->valuestring==NULL))
    {
        ESP_LOGE(HTTP_TAG,"LoginId is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    cJSON *password_item= cJSON_GetObjectItemCaseSensitive(root,"Passwrd");
    if(!cJSON_IsString(password_item)||(password_item->valuestring==NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"Password is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    strcpy(loginId,loginId_item->valuestring);
    strcpy(password,password_item->valuestring);

    ESP_LOGI(HTTP_TAG,"old password ID:%s;Password:%s",loginId,password);

    if(get_str_key(nvsHandle,LOGINPASS,temp)==OK_RESULT) 
    {
        ESP_LOGE(HTTP_TAG,"LOGINPASS is invalid"); 
        httpd_resp_sendstr(req,"false,LOGINPASS is invalid"); 
        goto end;     	
  	}
    else
    {
        if (strcmp(loginId,temp)==OK_RESULT)
        {
            if(add_str_key(nvsHandle, LOGINPASS, password)) 
            {
                ESP_LOGI("NVS","LOGIN INFO SAVED SUCCESSFULLY ");  
                httpd_resp_sendstr(req, "true, Successfully saved");              
             }
            else 
            {
                httpd_resp_sendstr(req, "false,Faild to set");
                ESP_LOGI(HTTP_TAG,"Login info Faild to set ");
            }
        }
        else
        {
            httpd_resp_sendstr(req, "false,,Old password is wrong");
            ESP_LOGI(HTTP_TAG,"Old Password is wrong ");
        }
    }
    goto end;
    end:
        cJSON_Delete(root);
        return ESP_OK;
}

static esp_err_t set_timeDate_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200);
    char buffer[TIME_REQ_BUFFER];
    memset(buffer,VAL_0,TIME_REQ_BUFFER*sizeof(char));
    int ret=VAL_0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= OK_RESULT) 
    {
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }

    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL)
    {
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }

    cJSON *year_item=cJSON_GetObjectItemCaseSensitive(root,"year");
    if (year_item == NULL)
    {
        ESP_LOGE(HTTP_TAG,"Year is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }
    cJSON *month_item= cJSON_GetObjectItemCaseSensitive(root,"month");
    if(month_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"Minute is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }
    cJSON *day_item= cJSON_GetObjectItemCaseSensitive(root,"day");
    if(day_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"day is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    cJSON *wday_item= cJSON_GetObjectItemCaseSensitive(root,"wday");
    if(wday_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"wday is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end; 
    }

    cJSON *hour_item= cJSON_GetObjectItemCaseSensitive(root,"hour");
    if(hour_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"Hour is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    cJSON *minute_item= cJSON_GetObjectItemCaseSensitive(root,"minute");
    if(minute_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"Minute is invalid"); 
        httpd_resp_sendstr(req,"false"); 
        goto end;
    }

    cJSON *second_item= cJSON_GetObjectItemCaseSensitive(root,"second");
    if(second_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"Second is invalid"); 
        httpd_resp_sendstr(req,"false");
        goto end;
    }

    cJSON *ntp_item= cJSON_GetObjectItemCaseSensitive(root,"ntp");
    if(ntp_item == NULL) 
    {
        ESP_LOGE(HTTP_TAG,"NTP STATE is invalid"); 
        httpd_resp_sendstr(req,"false");
        goto end;
    }
    
    int isNTP=ntp_item->valueint;
    if ((isNTP<0)||(isNTP>1)) 
    {
        ESP_LOGE(HTTP_TAG,"NTP Update Rate value is invalid");  
        goto end;
    }
    else
    {
        ntpState = isNTP;
        set_nvs_int_value(nvsHandle,NTPSTATE,isNTP);
        if (isNTP)
        {
            cJSON *address_item= cJSON_GetObjectItemCaseSensitive(root,"ip");
            if(address_item == NULL) 
            {
                ESP_LOGE(HTTP_TAG,"NTP Address is invalid"); 
                httpd_resp_sendstr(req,"false");
                goto end;
            }
            
            cJSON *rate_item= cJSON_GetObjectItemCaseSensitive(root,"rate");
            if(rate_item == NULL) 
            {
                ESP_LOGE(HTTP_TAG,"NTP Update Rate is invalid"); 
                httpd_resp_sendstr(req,"false");
                goto end;
            }
            

            int rate=rate_item->valueint;
            if ((rate<1)||(rate>43200))   //24 hours 
            {
                ESP_LOGE(HTTP_TAG,"NTP Update Rate value is invalid");  
                httpd_resp_sendstr(req,"false");
                goto end;
            }


            char Adress[NTP_ADDRESS_LEN];
            memset(Adress,0,NTP_ADDRESS_LEN);
            strcpy(Adress,address_item->valuestring);
            strcpy(ntpAddress,Adress);

            if(!(add_str_key(nvsHandle, NTPADDRS, Adress))) 
            {
                ESP_LOGE(HTTP_TAG,"NTP Address Not Saved"); 
            }
            ntpRate  = rate;
            set_nvs_int_value(nvsHandle,NTPRATE,rate);

            if(get_str_key(nvsHandle,NTPADDRS, Adress)==0)
            {
                if(add_str_key(nvsHandle, NTPADDRS, Adress)) 
                {
                    ESP_LOGI("NVS","NTP Address SUCCESSFULLY ");
                }
            } 
        }
    }
    
    if(ntpState)
    {
        ntp_service_init();
    }
    else 
    {
        ntp_stop_RQ();
    }
    
    int year=year_item->valueint;
    if ((year<2021)||(year>2050)) 
    {
        ESP_LOGE(HTTP_TAG,"YEAR value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }
    int mon =month_item->valueint;
    if ((mon<1)||(mon>12)) 
    {
        ESP_LOGE(HTTP_TAG,"MONTH value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }
    int day=day_item->valueint;
    if ((day<1)||(day>31)) 
    {
        ESP_LOGE(HTTP_TAG,"DAY value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }

    int hour=hour_item->valueint;
    if ((hour<0)||(hour>23)) 
    {
        ESP_LOGE(HTTP_TAG,"HOUR value is invalid");
        httpd_resp_sendstr(req,"false");  
        goto end;
    }

    int minute=minute_item->valueint;
    if ((minute<0)||(minute>60)) 
    {
        ESP_LOGE(HTTP_TAG,"MINUTE value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }

    int sec=second_item->valueint;
    if ((sec<0)||(sec>60)) 
    {
        ESP_LOGE(HTTP_TAG,"SECOND value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }

    int wday=wday_item->valueint;
    if ((wday<1)||(wday>7)) 
    {
        ESP_LOGE(HTTP_TAG,"wday value is invalid");  
        httpd_resp_sendstr(req,"false");
        goto end;
    }
    // ESP_LOGI(HTTP_TAG,"Received:%d-%d-%d %d:%d:%d",year,mon,day,hour,minute,sec);

    struct tm time = 
    {
            .tm_year = year,
            .tm_mon  = mon,             // 0-based
            .tm_mday = day,
            .tm_hour = hour,
            .tm_min  = minute,
            .tm_sec  = sec,
            .tm_wday = wday,
    };
    
    if (ds3231_set_time(&dev, &time) != ESP_OK) 
    { 
        httpd_resp_sendstr(req, "false");
        ESP_LOGI(HTTP_TAG,"Could not set time");
    }
    else 
    { 
        httpd_resp_sendstr(req, "true");
        ESP_LOGI(HTTP_TAG," set Time : %04d-%02d-%02d %02d:%02d:%02d ",year,mon,day,hour,minute,sec);
    }

    goto end;
    end:
        cJSON_Delete(root);
        return ESP_OK;
}

static esp_err_t get_timeDate_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200);
    struct tm rtc_info;    

    if (ds3231_get_time(&dev, &rtc_info) != ESP_OK) 
    {
        ESP_LOGE("DS3231", "Could not get time.");
        httpd_resp_sendstr(req,"false");
        return 0;
    }
     
    cJSON *timeDate=cJSON_CreateObject();
    if (timeDate == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE timeDate OBJECT");
        goto end;
    }
    cJSON *year=cJSON_CreateNumber(rtc_info.tm_year);
    if (year == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE year  OBJECT");
        goto end;
    }
    cJSON *month=cJSON_CreateNumber(rtc_info.tm_mon );
    if (month == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE month  OBJECT");
        goto end;
    }
    cJSON *day=cJSON_CreateNumber(rtc_info.tm_mday);
    if (day == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE day  OBJECT");
        goto end;
    }
  
    cJSON *wday=cJSON_CreateNumber(rtc_info.tm_wday);
    if (wday == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE wday  OBJECT");
        goto end;
    }
    cJSON *hour=cJSON_CreateNumber(rtc_info.tm_hour);
    if (hour == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE hour  OBJECT");
        goto end;
    }

    cJSON *minute=cJSON_CreateNumber(rtc_info.tm_min);
    if (minute == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE minute  OBJECT");
        goto end;
    }

    cJSON *second=cJSON_CreateNumber(rtc_info.tm_sec);
    if (second == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE second  OBJECT");
        goto end;
    }


    cJSON *rate_item=cJSON_CreateNumber(ntpRate);
    if (rate_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Update Rate  OBJECT");
        goto end;
    }
    
    cJSON *ntp_item=cJSON_CreateNumber(ntpState);
    if (ntp_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE NTP State  OBJECT");
        goto end;
    }
    cJSON *ip_item=cJSON_CreateString(ntpAddress);
    if (ip_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE NTP Address OBJECT");
        goto end;
    }


 
 
    cJSON_AddItemToObject(timeDate,"ntp",ntp_item);
    cJSON_AddItemToObject(timeDate,"rate",rate_item);
    cJSON_AddItemToObject(timeDate,"ip",ip_item);


    cJSON_AddItemToObject(timeDate,"year",year);
    cJSON_AddItemToObject(timeDate,"month",month);
    cJSON_AddItemToObject(timeDate,"day",day);
    cJSON_AddItemToObject(timeDate,"wday",wday);
    cJSON_AddItemToObject(timeDate,"hour",hour);
    cJSON_AddItemToObject(timeDate,"minute",minute);
    cJSON_AddItemToObject(timeDate,"second",second);


    char *content = cJSON_Print(timeDate);
    httpd_resp_sendstr(req, content);
    free(content);
    goto end;
    end:
    cJSON_Delete(timeDate);

    return ESP_OK;
}

static esp_err_t updateUser_post_handler(httpd_req_t *req)
{
    char buf[256];
    memset(buf,0,256*sizeof(char));
    int ret=0;
     ESP_LOGE(HTTP_TAG,"erceived len %d",req->content_len);
    if ((ret = httpd_req_recv(req, buf,sizeof(buf))) <= OK_RESULT) 
    {
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }

    char id[7];
    char name[21];
    char lastname[21];
    char cardId[9];
    char gender='0';
    uint8_t profile=0;

    char phone[16];
    char gender_val[2];

    memset(id,0,7*sizeof(char));
    memset(name,0,21*sizeof(char));
    memset(lastname,0,21*sizeof(char));
    memset(cardId,0,9*sizeof(char));
    memset(phone,0,16*sizeof(char));

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Root is Empty ");
        goto end;
    }
    cJSON *id_item = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (!cJSON_IsString(id_item) || (id_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"id value is invalid");  
        httpd_resp_sendstr(req, "false,id value is invalid"); 
        goto end;
    }
    cJSON *pname_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(pname_item) || (pname_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Name value is invalid");  
        httpd_resp_sendstr(req, "false,Name value is invalid"); 
        goto end;
    }
    cJSON *plastname_item = cJSON_GetObjectItemCaseSensitive(root, "lastname");
    if (!cJSON_IsString(plastname_item) || (plastname_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Last Name value is invalid");  
        httpd_resp_sendstr(req, "false,Last Name value is invalid"); 
        goto end;
    }
    cJSON *pphone_item = cJSON_GetObjectItemCaseSensitive(root, "phone");
    if (!cJSON_IsString(pphone_item) || (pphone_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Phone value is invalid");  
        httpd_resp_sendstr(req, "false,Phone value is invalid"); 
        goto end;
    }
    cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(root, "cardID");
    if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"CardID value is invalid");  
        httpd_resp_sendstr(req, "false,CardID value is invalid"); 
        goto end;
    }
    cJSON *pgender_item = cJSON_GetObjectItemCaseSensitive(root, "gender");
    if (!cJSON_IsString(pgender_item) || (pgender_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Gender value is invalid");  
        httpd_resp_sendstr(req, "false,Gender value is invalid"); 
        goto end;
    }
    cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(root, "profileId");
    if (profile_item== NULL) 
    { 
        ESP_LOGE(HTTP_TAG,"profile value is invalid");  
        httpd_resp_sendstr(req, "false,profileId value is invalid"); 
        goto end;
    }


    strcpy(id,id_item->valuestring);
  	strcpy(name, pname_item->valuestring);
    strcpy(lastname, plastname_item->valuestring);
    strcpy(phone, pphone_item->valuestring);
    strcpy(cardId, pcardId_item->valuestring);
    strcpy(gender_val,pgender_item->valuestring);
    
    gender=gender_val[0]; 
    profile=profile_item->valueint;

    ESP_LOGI(HTTP_TAG,"Reseived Data:%s;%s;%s;%s;%s;%c %d",id,name,lastname,cardId,phone,gender ,profile);

    int err=update_user(id,profile,name,lastname,cardId,phone,gender);
    if(err==1){
                httpd_resp_sendstr(req, "true");
                ESP_LOGI(HTTP_TAG,"%s %s update successfully",name,lastname);
                }
    else { 
            httpd_resp_sendstr(req, "false,faild to update "); 
            ESP_LOGE(HTTP_TAG,"faild to update");
    }
    goto end;
    end:
        cJSON_Delete(root);
    
 return ESP_OK;
}

static esp_err_t adduser_post_handler(httpd_req_t *req)
{
    char buf[BUFFER_256_B_LEN];
    memset(buf,0,BUFFER_256_B_LEN*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buf,sizeof(buf))) <= 0) 
    {
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }
    char name[FIRST_NAME_LENGTH];
    char lastname[LAST_NAME_LENGTH];
    char cardId[CARD_ID_BUF_LENGTH];
    char gender='0';
    uint8_t profile=0;
    char phone[PHONE_BUF_LENGTH];
    char gender_val[2];

    memset(name,0,FIRST_NAME_LENGTH*sizeof(char));
    memset(lastname,0,LAST_NAME_LENGTH*sizeof(char));
    memset(cardId,0,CARD_ID_BUF_LENGTH*sizeof(char));
    memset(phone,0,PHONE_BUF_LENGTH*sizeof(char));

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL)
    {
            httpd_resp_sendstr(req, "false");
             ESP_LOGE(HTTP_TAG,"Root is Empty");
            goto end;
    }

    cJSON *pname_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(pname_item) || (pname_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Name value is invalid");  
        httpd_resp_sendstr(req, "false,Name value is invalid"); 
        goto end;
    }
    cJSON *plastname_item = cJSON_GetObjectItemCaseSensitive(root, "lastname");
    if (!cJSON_IsString(plastname_item) || (plastname_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Last Name value is invalid");  
        httpd_resp_sendstr(req, "false,Last Name value is invalid"); 
        goto end;
    }
    cJSON *pphone_item = cJSON_GetObjectItemCaseSensitive(root, "phone");
    if (!cJSON_IsString(pphone_item) || (pphone_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Phone value is invalid");  
        httpd_resp_sendstr(req, "false,Phone value is invalid"); 
        goto end;
    }
    cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(root, "cardID");
    if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"CardID value is invalid");  
        httpd_resp_sendstr(req, "false,CardID value is invalid"); 
        goto end;
    }
    cJSON *pgender_item = cJSON_GetObjectItemCaseSensitive(root, "gender");
    if (!cJSON_IsString(pgender_item) || (pgender_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"Gender value is invalid");  
        httpd_resp_sendstr(req, "false,Gender value is invalid"); 
        goto end;
    }

    cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(root, "profileId");
    if (profile_item == NULL) 
    { 
        ESP_LOGE(HTTP_TAG,"profile value is invalid");  
        httpd_resp_sendstr(req, "false,profileID value is invalid"); 
        goto end;
    }
  	strcpy(name, pname_item->valuestring);
    strcpy(lastname, plastname_item->valuestring);
    strcpy(phone, pphone_item->valuestring);
    strcpy(cardId, pcardId_item->valuestring);

    if(strlen(cardId)!=CARD_ID_LEN) 
    { 
        ESP_LOGE(HTTP_TAG,"CardId format is invalid ");  
        httpd_resp_sendstr(req, "false"); 
        goto end; 
    }

    strcpy(gender_val,pgender_item->valuestring);
    gender=gender_val[0]; 
    profile=profile_item->valueint;

    ESP_LOGI(HTTP_TAG,"Reseived Data:%s;%s;%s;%s;%c %d",name,lastname,cardId,phone,gender,profile);

    if (USERCOUNT<MAX_USER_COUNT)
    {
            int err=add_user(++userCounter,profile,name,lastname,cardId,phone,gender);
            if(err==1)
            {
                ESP_LOGI(HTTP_TAG, "user added.. ");
                set_nvs_int_value(nvsHandle,USER_COUNT,++USERCOUNT);
                set_nvs_int_value(nvsHandle,USER_COUNTER,userCounter); 
                httpd_resp_sendstr(req, "true");
            }
            else if(err==-1)
            {                     
                httpd_resp_sendstr(req, "false,User is already exist");
                ESP_LOGE(HTTP_TAG,"User is already exist ");
                userCounter--;
            }    
            else
            {  
                userCounter--;
                httpd_resp_sendstr(req, "false,Failed to add user");
                ESP_LOGE(HTTP_TAG,"Failed to add user ");     
            }
           if(userCounter>MAX_USERCOUNTER ) 
           {
               userCounter=0; 
           }    
    }
    else 
    { 
            httpd_resp_sendstr(req, "false,Max Cap: 1000 User. You Canot add more"); 
            ESP_LOGE(HTTP_TAG,"Max Cap: 1000 User. You Canot add more ");
    }
    goto end;
    end:
        cJSON_Delete(root);
    
 return ESP_OK;
}

static esp_err_t delusers_post_handler(httpd_req_t *req)
{
    char buffer[BUFFER_128_B_LEN];
    bzero(buffer,BUFFER_128_B_LEN);
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer,sizeof(buffer))) <= 0) {httpd_resp_sendstr(req,"false"); return ESP_FAIL;}
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 
 
     cJSON *list=cJSON_Parse(buffer);
    if (list ==NULL){
         ESP_LOGE(HTTP_TAG,"Empty Root List \n");
        httpd_resp_sendstr(req, "false");
        goto end;
    }

    char name[12];
    memset(name,0,12*sizeof(char));
    cJSON *user_item = cJSON_GetObjectItemCaseSensitive(list, "users");
    if (!cJSON_IsString(user_item) || (user_item->valuestring == NULL))
    {
        ESP_LOGE(HTTP_TAG,"user value is invalid");  
        httpd_resp_sendstr(req, "false,user value is invalid"); 
        goto end;
    }
    strcpy(name, user_item->valuestring);
    ESP_LOGI(HTTP_TAG,"User:%s",name);
    if (del_user(name)==1) 
    { 
        set_nvs_int_value(nvsHandle,USER_COUNT,--USERCOUNT); 
        httpd_resp_sendstr(req,"true"); 
        ESP_LOGW(HTTP_TAG,"user:%s: has been removed",name); 
    } 
    else httpd_resp_sendstr(req,"false");

    goto end;

    end:
    cJSON_Delete(list);

    return ESP_OK;
}

static esp_err_t log_handler(httpd_req_t *req)
{
    FILE *file;
    char buffer[BUFFER_LENGTH_64];
    char line[LINE_LENGTH_60];
    char val[LINE_LENGTH_64];
    char path[FILE_PATH_LENGTH];
    bool fstate=true;
    unsigned int count=0;   
    int ret=0;
    char start[LOG_DATE_LENGTH];  
    char end[LOG_DATE_LENGTH];
    esp_err_t err_code=ESP_OK;

    

    bzero(buffer,BUFFER_LENGTH_64);
    bzero(path,FILE_PATH_LENGTH);

    int len = req->content_len;
    if ((ret = httpd_req_recv(req, buffer,MIN(len, sizeof(buffer)))) <= 0) 
    {
        return ESP_FAIL;
    }

    memset(start,0,LOG_DATE_LENGTH*sizeof(char));
    memset(end,0,LOG_DATE_LENGTH*sizeof(char));

    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL)
    {
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        fstate=false;
        goto end;  
    }
    
    cJSON *start_item = cJSON_GetObjectItemCaseSensitive(root, "start");
    if (!cJSON_IsString(start_item) || (start_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"Start value is invalid");  
        fstate=false;
        goto end;
    }

	cJSON *end_item = cJSON_GetObjectItemCaseSensitive(root, "end");
    if (!cJSON_IsString(end_item) || (end_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"_End is invalid");  
        fstate=false;
        goto end;
    }
     
	strcpy(start, start_item->valuestring);
	strcpy(end, end_item->valuestring);
    
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200); 
    httpd_resp_set_type(req,"application/json");

    struct dirent *entry;
    char newest[LOG_NAME_LENGTH];
    char oldest[LOG_NAME_LENGTH];
    char name[LOG_NAME_LENGTH];

    bzero(newest,LOG_NAME_LENGTH);
    bzero(oldest,LOG_NAME_LENGTH);
    sprintf(newest,"log%s",start);
    sprintf(oldest,"log%s",end);

    DIR *dir = opendir("/spiffs");   
    if (dir == NULL)
    {
        ESP_LOGE("HTTP_LOG","UNABLE TO OPEN DIR");
        fstate=false;
        goto end;
    }
    ESP_LOGW("HTTP_LOG","START FETCHING " );
        
    while ((entry = readdir(dir)) != NULL)
    {

        if (strncmp(entry->d_name,"log",COMPAER_LEN_3)==0)
        {       
               
                if((strncmp(entry->d_name,newest,LOG_NAME_LENGTH)>=0)&&(strncmp(entry->d_name,oldest,LOG_NAME_LENGTH)<=0))
                {
                    if (count ==0) 
                    {
                        err_code=httpd_resp_send_chunk(req, "{\"logs\":[",strlen("{\"logs\":["));
                        if(err_code!=ESP_OK)
                        {
                            break;
                        }
                    }
                    fstate=true;
                    
                    bzero (name,LOG_NAME_LENGTH);
                    strncpy(name,entry->d_name,LOG_NAME_LENGTH);
                    sprintf(path,"/spiffs/%s",name);
                    file = fopen(path, "r");
                    if (file == NULL) 
                    { 
                        ESP_LOGE("HTTP_LOG", "Failed to open file");  
                        continue;
                    }
                    ESP_LOGE("HTTP_LOG","Start sending %s",entry->d_name);
                    do{                          
                            bzero(line,LINE_LENGTH_60);
                            bzero(val,LINE_LENGTH_64);
                            fgets(line, sizeof(line), file);
                            if(feof(file)) break;
                            int i=0;
                            do
                            {                                            // Remove  \n  from line content 
                                i++;
                            }while(line[i]!='\n');
                            line[i]=0;
                            if (count ==0)
                            {
                                sprintf(val,"\"%s\"",line); 
                            }
                            else 
                            {
                                sprintf(val,",\"%s\"",line);
                            }
                            err_code=httpd_resp_send_chunk(req, val,strlen(val));
                            if(err_code!=ESP_OK)
                            {
                                break;
                            }
                            count++;
                    }while(!feof(file));
                    fclose(file);                     
                }
            }   
                
        }
    closedir (dir);
    httpd_resp_send_chunk(req, "]}",strlen("]}"));       
    httpd_resp_send_chunk(req,NULL,0);
    ESP_LOGW("HTTP_LOG","Log Count %d",count);    
    goto end;
    end:  
        cJSON_Delete(root);
    if (fstate==false)
    {
        httpd_resp_sendstr(req, "false");
    }

    return ESP_OK;
}

static esp_err_t del_log_handler(httpd_req_t *req)
{
    char buffer[LINE_LENGTH_64];
    unsigned int count=0;   
    int ret=0;
    char start[LOG_DATE_LENGTH];  //year-month-day
    char end[LOG_DATE_LENGTH];

    bzero(buffer,LINE_LENGTH_64);

    int len = req->content_len;
    if ((ret = httpd_req_recv(req, buffer,MIN(len, sizeof(buffer)))) <= 0) 
    {
        return ESP_FAIL;
    }
    memset(start,0,LOG_DATE_LENGTH*sizeof(char));
    memset(end,0,LOG_DATE_LENGTH*sizeof(char));

    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL)
    {
        ESP_LOGE(HTTP_TAG,"Root is Empty ");
        goto end;  
    }
    
    cJSON *start_item = cJSON_GetObjectItemCaseSensitive(root, "start");
    if (!cJSON_IsString(start_item) || (start_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"Start value is invalid "); 
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
	cJSON *end_item = cJSON_GetObjectItemCaseSensitive(root, "end");
    if (!cJSON_IsString(end_item) || (end_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"_End is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
     
	strcpy(start, start_item->valuestring);
	strcpy(end, end_item->valuestring); 
    
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200); 
    httpd_resp_set_type(req,"application/json");


    struct dirent *entry;
    char newest[FILE_NAME_LENGTH];
    char oldest[FILE_NAME_LENGTH];
    char name[FILE_NAME_LENGTH];

    bzero(newest,FILE_NAME_LENGTH);
    bzero(oldest,FILE_NAME_LENGTH);
    sprintf(newest,"log%s",start);
    sprintf(oldest,"log%s",end);

    DIR *dir = opendir("/spiffs");   
    if (dir == NULL) 
    {
    ESP_LOGE("HTTP_LOG","UNABLE TO OPEN DIR");
    goto end;
    }
    ESP_LOGW("HTTP_LOG","START FETCHING " );  

    while ((entry = readdir(dir)) != NULL) {

        if (strncmp(entry->d_name,"log",COMPAER_LEN_3)==OK_RESULT)
        {       
               
                if((strncmp(entry->d_name,newest,FILE_NAME_LENGTH)>=0)&&(strncmp(entry->d_name,oldest,FILE_NAME_LENGTH)<=OK_RESULT))
                {
                    bzero (name,FILE_NAME_LENGTH);
                    strncpy(name,entry->d_name,FILE_NAME_LENGTH);
                    ESP_LOGW("HTTP_LOG","Removing %s",name);
                    if(del_log(name)) count++;
                }
            }   
                
        }
    closedir (dir);
    ESP_LOGE("HTTP","File Count %d",count); 

    goto end;
    end:
        cJSON_Delete(root);

    if (count==0) 
    {
        httpd_resp_sendstr(req,"false,No logs to delete");
        ESP_LOGE(HTTP_TAG,"File is not exists ");
    }
    else   
    {    
        httpd_resp_sendstr(req,"true");
    }  
    return ESP_OK;
}

static esp_err_t delall_log_handler(httpd_req_t *req) 
{
  
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req,"Access-Control-Max-Age", "10000");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "*");
    httpd_resp_set_status(req, HTTPD_200); 

    if (clear_logs()==false) 
    {
        httpd_resp_sendstr(req, "false,No logs to delete");
        ESP_LOGE(HTTP_TAG,"Failed to clear ");
    }
    else   
    {    
        httpd_resp_sendstr(req,"true");
        ESP_LOGE(HTTP_TAG,"All Logs have been removed ");
    }  
    return ESP_OK;
}

static esp_err_t user_list_handler(httpd_req_t *req)
{

    FILE *file;
    int count=0;
    bool fstate=false;
    char line[USER_LINE_LENGTH];
    char val [USER_DATA_LENGTH];
    struct dirent *entry;
    esp_err_t err_code=ESP_OK;
    DIR *dir = opendir("/spiffs");   
    if (dir == NULL) 
    {
        httpd_resp_sendstr(req,"false");
        return false;
     }
    
   httpd_resp_set_status(req, HTTPD_200); 
   while ((entry = readdir(dir)) != NULL) 
   {
        char path[FILE_PATH_LENGTH]={0};
        bzero(path,FILE_PATH_LENGTH);
        bzero(line,USER_LINE_LENGTH);
        bzero(val,USER_DATA_LENGTH);
        char name[USER_NAME_LENGTH];
        bzero (name,USER_NAME_LENGTH);
        strncpy(name,entry->d_name,CARD_ID_LENGTH);
        if ((strncmp(entry->d_name,"log",COMPAER_LEN_3)==OK_RESULT)||((strncmp(entry->d_name,"sch",3)==OK_RESULT))) 
        {
            continue;
        }
        if(count == 0) 
        {
            err_code=httpd_resp_send_chunk(req, "{\"users\":[",strlen("{\"users\":["));
            if(err_code!=ESP_OK)
            {
                break;
            }
        }

        fstate=true;
        memset(path,0,FILE_PATH_LENGTH*sizeof(char));
        sprintf(path,"/spiffs/%s",name);        
        file = fopen(path, "r");
        if (file == NULL) 
        { 
            ESP_LOGE(HTTP_TAG, "Failed to open file: %s",path); continue;
        } 
        fgets(line,sizeof(line),file);
        if(count == 0) 
        { 
            sprintf(val,"\"%s\"",line);
        }
        else 
        {
            sprintf(val,",\"%s\"",line);
        }
        err_code = httpd_resp_send_chunk(req, val,strlen(val));
        if(err_code!=ESP_OK)
        {
            break;
        }
        fclose(file);
       count++;
   }
   ESP_LOGI(HTTP_TAG,"USER LIST HAS  BEEN SENT.user count %d",count);
   closedir(dir);

   if (!fstate) 
   {
        httpd_resp_send_chunk(req, "false",strlen("false"));
        ESP_LOGE(HTTP_TAG,"File is not exists");
    }
   else   
   { 
    if(err_code==ESP_OK)
    {
        httpd_resp_send_chunk(req, "]}",strlen("]}"));
    }   
   }  
    httpd_resp_send_chunk(req,NULL,0);
    vTaskDelay(10/portTICK_PERIOD_MS);
    return ESP_OK;
}

static esp_err_t sta_post_handler(httpd_req_t *req){

    char buffer[128];
    bzero(buffer,128);
    int ret=0;
    int len = req->content_len;
    if ((ret = httpd_req_recv(req, buffer,len)) <= 0)
    {
        return ESP_FAIL;
    }
    // ESP_LOGI(HTTP_TAG, "RECEIVED DATA %d:%s", ret, buffer);

    char name[24];
    char password[16];
    memset(name,0,24*sizeof(char));
    memset(password,0,16*sizeof(char));

    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL){
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Root is Empty  \n");
            goto end;  
    }
    
    cJSON *ssid_item = cJSON_GetObjectItemCaseSensitive(root, "SSID");
    if (!cJSON_IsString(ssid_item) || (ssid_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"SSID value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}
	cJSON *pass_item = cJSON_GetObjectItemCaseSensitive(root, "Password");
    if (!cJSON_IsString(pass_item) || (pass_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"Password value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}
     
	strcpy(name, ssid_item->valuestring);
	strcpy(password, pass_item->valuestring);

    uint8_t i;                      // redefine  space 
    for (i=0;i<strlen(name);i++)
        if(name[i]=='+') name[i]=' ';
    ESP_LOGI(HTTP_TAG, "%s:  %s", name , password);

    if(add_str_key(nvsHandle, STA_SSID, name ) && add_str_key(nvsHandle, STA_PASS, password)) {
            ESP_LOGI(HTTP_TAG, "WiFi info HAS BEEN SAVED");
            httpd_resp_sendstr(req, "true");}
    else{ httpd_resp_sendstr(req, "false");}        
    goto end;
    end:
        cJSON_Delete(root);
    vTaskDelay(10/portTICK_PERIOD_MS);
    return ESP_OK;
}

static esp_err_t uplodlist_post_handler(httpd_req_t *req)
{

    char buf[152];
    memset(buf,0,152*sizeof(char));
    int ret=0;
    //ESP_LOGI(HTTP_TAG,"erceived len %d |%d\n",remaining,heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    if ((ret = httpd_req_recv(req, buf,sizeof(buf))) <= 0) {return ESP_FAIL;}
    // ESP_LOGI(HTTP_TAG, "RECEIVED DATA %d:%s", ret, buf);
    httpd_resp_set_hdr(req,"Content-Type:", "text/html");
    httpd_resp_set_status(req, HTTPD_200); 

    char name[21];
    char lastname[21];
    char cardId[10];
    char gender='0';
    uint8_t profile=1;
    char phone[16];
    char gender_val[2];
    // char profile_val[2];

    memset(name,0,21*sizeof(char));
    memset(lastname,0,21*sizeof(char));
    memset(cardId,0,10*sizeof(char));
    memset(phone,0,16*sizeof(char));

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL){
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Root is Empty ");
            goto end;  
    }
    
    cJSON *pname_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(pname_item) || (pname_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"Name value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *plastname_item = cJSON_GetObjectItemCaseSensitive(root, "lastname");
    if (!cJSON_IsString(plastname_item) || (plastname_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"Last Name value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *pphone_item = cJSON_GetObjectItemCaseSensitive(root, "phone");
    if (!cJSON_IsString(pphone_item) || (pphone_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"Phone value is invalid  \n"); httpd_resp_sendstr(req, "false");  goto end;}
    cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(root, "cardId");
    if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"CardId value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *pgender_item = cJSON_GetObjectItemCaseSensitive(root, "gender");
    if (!cJSON_IsString(pgender_item) || (pgender_item->valuestring == NULL)) {ESP_LOGE(HTTP_TAG,"Gender value is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end;}

    cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(root, "profileId");
    if (profile_item == NULL) { ESP_LOGE(HTTP_TAG,"profile value is invalid");  httpd_resp_sendstr(req, "false,profileId value is invalid"); goto end;}

    profile=profile_item->valueint;

  	strcpy(name, pname_item->valuestring);
    strcpy(lastname, plastname_item->valuestring);
    strcpy(phone, pphone_item->valuestring);
    strcpy(cardId, pcardId_item->valuestring);
    if(strlen(cardId)!=8) { ESP_LOGE(HTTP_TAG,"CardId format is invalid  \n");  httpd_resp_sendstr(req, "false"); goto end; }
    strcpy(gender_val,pgender_item->valuestring);
    // strcpy(profile_val,profile_item->valuestring);
    
    if((gender=='1')||(gender=='0')) gender=gender_val[0]; 
    else  { ESP_LOGE(HTTP_TAG,"Gender value is invalid");  httpd_resp_sendstr(req, "false,Gender value is invalid"); goto end;}
    // profile=atoi(profile_val);

    // ESP_LOGI(HTTP_TAG,"Reseived Data:%d;%s;%s;%s;%s;%c|%d\n",profile,name,lastname,cardId,phone,gender,heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    if (USERCOUNT<MAX_USER_COUNT)
    {
        int err=add_user(++userCounter,profile,name,lastname,cardId,phone,gender);
           if(err==1)
           {
                        ESP_LOGE(HTTP_TAG, "User Added");
                        set_nvs_int_value(nvsHandle,USER_COUNT,++USERCOUNT);
                        set_nvs_int_value(nvsHandle,USER_COUNTER,userCounter); 
                        httpd_resp_sendstr(req, "true");
            }
           else if(err==-1)
           {                     
                        httpd_resp_sendstr(req, "false,User is already exist");
                        ESP_LOGW(HTTP_TAG,"User is already exist ");
                        userCounter--;

            }    
            else
            {  
                        userCounter--;
                        httpd_resp_sendstr(req, "false,Failed to add");
                        ESP_LOGE(HTTP_TAG,"Failed to added successfully ");     
            }
            
            if(userCounter>MAX_USERCOUNTER ) 
            {
               userCounter=0; 
            }      
    }
    else 
    { 
            httpd_resp_sendstr(req, "false,Max Cap: 1000 User. You Canot add more "); 
            ESP_LOGE(HTTP_TAG,"Max Cap: 1000 User. You Canot add more ");
    }
    goto end;
    end:
        cJSON_Delete(root);
    
    vTaskDelay(10/portTICK_PERIOD_MS);
        return ESP_OK;
}

static esp_err_t wifiscan_handler(httpd_req_t *req){
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    xEventGroupClearBits(connection_flag,CONNECTED_BIT_scanRes);
    scann();
    xEventGroupWaitBits(connection_flag,CONNECTED_BIT_scanRes,pdTRUE, pdFALSE,5000);
    uint16_t ap_num=APnum;
    printf("Total APs scanned = %d \n",ap_num);
    cJSON *wifi_list=cJSON_CreateObject();
    if (wifi_list == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE wifi_list OBJECT");
        goto end;
    }
    cJSON *ap_s=cJSON_CreateArray();
    cJSON *ap=NULL;
    cJSON *ssid=NULL;
    cJSON *rssi=NULL;
    cJSON *count=cJSON_CreateNumber(ap_num);
    if (count == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE count  OBJECT");
        goto end;
    }

    cJSON *connectedWifi = cJSON_CreateString(st_SSID);
    if (connectedWifi == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE connectedWifi  OBJECT");
       goto end;
    }

    char mode_val[13];
    bzero(mode_val,13);
    
    if (mode==0) sprintf(mode_val,"accesspoint");
    else sprintf(mode_val,"station");
    
    cJSON *mode_item = cJSON_CreateString(mode_val);
    if (mode_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Mode  OBJECT");
        goto end;
    }

    cJSON *ver_item = cJSON_CreateString(Firmware);
    if (ver_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Firmware  OBJECT");
        goto end;
    }

    cJSON *ID_item = cJSON_CreateString(MAC);
    if (ID_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Firmware  OBJECT");
        goto end;
    }

    cJSON_AddItemToObject(wifi_list,"deviceID",ID_item);
    cJSON_AddItemToObject(wifi_list,"version",ver_item);
    cJSON_AddItemToObject(wifi_list,"connected",connectedWifi);
    cJSON_AddItemToObject(wifi_list,"Mode",mode_item);
    cJSON_AddItemToObject(wifi_list,"count",count);
    cJSON_AddItemToObject(wifi_list,"APs",ap_s);
 

    for(int i=0;i<ap_num;i++){
        if(strlen(wifi_Ap_list[i].ssid)==0) continue;                      

        ap=cJSON_CreateObject();
        cJSON_AddItemToArray(ap_s, ap);   
        ssid=cJSON_CreateString( wifi_Ap_list[i].ssid);
        rssi=cJSON_CreateNumber(wifi_Ap_list[i].rssi);
        cJSON_AddItemToObject(ap, "SSID", ssid);   
        cJSON_AddItemToObject(ap,"RSSI",rssi);     
    }
    char *string = cJSON_Print(wifi_list);
    httpd_resp_sendstr(req, string);
    free(string);

    end:
    cJSON_Delete(wifi_list);
    
    return ESP_OK;
}

static esp_err_t wificonnect_handler(httpd_req_t *req){

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 
    char buffer[BUFFER_LENGTH_64];
    bzero(buffer,BUFFER_LENGTH_64);
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer,sizeof(buffer))) <= 0) 
    {
        httpd_resp_sendstr(req,"false"); 
        return ESP_FAIL;
    }

    char name[SSID_LEN];
    char password[PASS_LEN];
    memset(name,0,SSID_LEN*sizeof(char));
    memset(password,0,PASS_LEN*sizeof(char));
  
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Root is Empty ");
        goto end;  
    }
    
    cJSON *ssid_item = cJSON_GetObjectItemCaseSensitive(root, "SSID");
    if (!cJSON_IsString(ssid_item) || (ssid_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"SSID value is invalid ");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }

	cJSON *pass_item = cJSON_GetObjectItemCaseSensitive(root, "Password");
    if (!cJSON_IsString(pass_item) || (pass_item->valuestring == NULL)) 
    {
        ESP_LOGE(HTTP_TAG,"Password value is invalid ");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
     
	strcpy(name, ssid_item->valuestring);
	strcpy(password, pass_item->valuestring);

    if((strlen(password)<PASSWOR_MIN_LEN)||(strlen(password)>PASSWOR_MAX_LEN)) 
    {
        ESP_LOGE(HTTP_TAG,"Password value is invalid ");  
        httpd_resp_sendstr(req, "false,password Length "); 
        goto end; 
    }

    uint8_t i;                      // redefine  space 
    for (i=0;i<strlen(name);i++)
    {
        if(name[i]=='+') 
        {
            name[i]=' ';
        }
    }
    ESP_LOGI(HTTP_TAG, "%s:  %s", name , password);
    xEventGroupClearBits(wifi_connection_flag, CONNECTED_BIT);
    status=false;
    retry=0;
    connect_wifi(name,password);
    vTaskDelay(DELAY_1_SEC);

    xEventGroupWaitBits(wifi_connection_flag,CONNECTED_BIT, pdTRUE, pdFALSE, 10000 / portTICK_PERIOD_MS);  

    if (mode== ACCESSPOINT_MODE)
    {
        char state[6];  
        char msg[17];

        bzero(state,6);
        bzero(msg,17);

        sprintf(state,"false");
        sprintf(msg,"000.000.000.000");

        if(status) 
        {
            sprintf(state,"true"); 
            sprintf(msg,"%s",myIP);
        }
        cJSON *info=cJSON_CreateObject();
        if (info == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE info OBJECT");
            goto sub_end;
        }

        cJSON *status=cJSON_CreateString(state);
        if (status == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE state  OBJECT");
            goto sub_end;
        }

        cJSON *message=cJSON_CreateString(msg);
        if (message == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE msg  OBJECT");
            goto sub_end;
        }

        cJSON_AddItemToObject(info,"status",status);
        cJSON_AddItemToObject(info,"message",message);

        ESP_LOGE(HTTP_TAG, "status:%s message:%s ", state,msg); 
        char *content = cJSON_Print(info);
        httpd_resp_sendstr(req, content);
        free(content);
        goto sub_end;
        sub_end:
        cJSON_Delete(info);
    }
    
    if ((mode== ACCESSPOINT_MODE)||(status==true))             //  save New Wifi info 
   { 
       
       if(add_str_key(nvsHandle, STA_SSID, name ) && add_str_key(nvsHandle, STA_PASS, password))   
       {
            strcpy(st_SSID,name);
            strcpy(st_pass,password);       
            ESP_LOGI(HTTP_TAG, "SAVE WiFi INFO");
        }
       else 
       {
           ESP_LOGE(HTTP_TAG, "UNABLE TO SAVE Wifi INFO "); 
       }
   }
    
    if ((mode== STATION_2_STA_MODE)&&(status==false))           // Reconnect old Wifi 
    {
       ESP_LOGI(HTTP_TAG, "Reconeccet to: %s",st_SSID);
       connect_wifi(st_SSID,st_pass);
    }

    goto end;
     end:
        cJSON_Delete(root);
    return ESP_OK;
}

static esp_err_t apSettings_post_handler(httpd_req_t *req){
    
    char buffer[256];
    bzero(buffer,256);
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer,sizeof(buffer))) <= 0) {httpd_resp_sendstr(req,"false"); return ESP_FAIL;}
    // ESP_LOGI(HTTP_TAG, "RECEIVED DATA %d:%s", ret, buffer);

    char _appSSID[25];
    char _apPASS[17];
    char _apIP[16];
    char _apNETMASK[16];
    char _gateway[16];

    memset(_appSSID,0,16*sizeof(char));
    memset(_apPASS,0,12*sizeof(char));
    memset(_apIP,0,16*sizeof(char));
    memset(_apNETMASK,0,16*sizeof(char));
    memset(_gateway,0,16*sizeof(char));
    
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200); 

    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL){
            httpd_resp_sendstr(req, "false");
             ESP_LOGE(HTTP_TAG,"Root is Empty ");
            goto end;
        }

    cJSON *ssid_item = cJSON_GetObjectItemCaseSensitive(root, "ssid");
    if (!cJSON_IsString(ssid_item) || (ssid_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"SSID value is invalid ");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
    cJSON *pass_item = cJSON_GetObjectItemCaseSensitive(root, "password");
    if (!cJSON_IsString(pass_item) || (pass_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"PASSWORD value is invalid ");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
    cJSON *ip_item = cJSON_GetObjectItemCaseSensitive(root, "ipaddress");
    if (!cJSON_IsString(ip_item) || (ip_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"IP is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
    cJSON *netmask_item = cJSON_GetObjectItemCaseSensitive(root, "networkmask");
    if (!cJSON_IsString(netmask_item) || (netmask_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"NETMASK value is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
    cJSON *gateway_item = cJSON_GetObjectItemCaseSensitive(root, "gateway");
    if (!cJSON_IsString(gateway_item) || (gateway_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"GATEWAY value is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }

  	strcpy(_appSSID, ssid_item->valuestring);
    strcpy(_apPASS, pass_item->valuestring);
    strcpy(_apIP, ip_item->valuestring);
    strcpy(_apNETMASK, netmask_item->valuestring);
    strcpy(_gateway,gateway_item->valuestring);

    ESP_LOGI(HTTP_TAG,"Reseived Data:%s;%s;%s;%s",_appSSID,_apPASS,_apIP,_apNETMASK);
    if (strlen(_apPASS)<10) 
    {
      httpd_resp_sendstr(req, "false");
      ESP_LOGE(HTTP_TAG,"Password too short");
      goto end;
    }

    if(add_str_key(nvsHandle,AP_IP,_apIP)==0) 
    {
      httpd_resp_sendstr(req, "false");
      ESP_LOGE(HTTP_TAG,"Failed to add AP_IP");
      goto end;
    }
    if(add_str_key(nvsHandle,AP_NETMASK,_apNETMASK)==0) 
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to add AP_NETMASK");
        goto end;
    }
    if(add_str_key(nvsHandle,AP_SSID,_appSSID)==0) 
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to add _AP_SSID");
        goto end;
    }
    if(add_str_key(nvsHandle,AP_PASSWORD,_apPASS)==0) 
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to add AP_PASSWORD");
        goto end;       
    }
    if(add_str_key(nvsHandle,AP_GATEWAY,_gateway)==0) 
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to add AP_GATEWAY");
        goto end;       
    }
        
    httpd_resp_sendstr(req, "true");
    ESP_LOGE(HTTP_TAG,"ADDED SUCCESSFULLY");
    end:
        cJSON_Delete(root);    
    return ESP_OK;
}

static esp_err_t staSettings_post_handler(httpd_req_t *req)
{
    
    char buffer[256];
    bzero(buffer,256);
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer,sizeof(buffer))) <= 0) {httpd_resp_sendstr(req,"false"); return ESP_FAIL;}

    char _st_SSID[25];
    char _st_pass[17];

    memset(_st_SSID,0,25*sizeof(char));
    memset(_st_pass,0,17*sizeof(char));
    
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    // httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200); 

    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL)
    {
            httpd_resp_sendstr(req, "false");
             ESP_LOGE(HTTP_TAG,"Root is Empty ");
            goto end;
    }

    cJSON *ssid_item = cJSON_GetObjectItemCaseSensitive(root, "ssid");
    if (!cJSON_IsString(ssid_item) || (ssid_item->valuestring == NULL))
    { 
        ESP_LOGE(HTTP_TAG,"SSID value is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }
    cJSON *pass_item = cJSON_GetObjectItemCaseSensitive(root, "password");
    if (!cJSON_IsString(pass_item) || (pass_item->valuestring == NULL)) 
    { 
        ESP_LOGE(HTTP_TAG,"PASSWORD value is invalid");  
        httpd_resp_sendstr(req, "false"); 
        goto end;
    }


  	strcpy(_st_SSID, ssid_item->valuestring);
    strcpy(_st_pass, pass_item->valuestring);

    ESP_LOGI(HTTP_TAG,"Reseived Data:%s;%s",_st_SSID,_st_pass);

    if(add_str_key(nvsHandle, STA_SSID, _st_SSID ) && add_str_key(nvsHandle, STA_PASS, _st_pass)) 
    {
        ESP_LOGI(HTTP_TAG, "WiFi info HAS BEEN SAVED");
        httpd_resp_sendstr(req, "true");
    }
    else
    { 
        httpd_resp_sendstr(req, "false");
    }        

    end:
        cJSON_Delete(root);
    
    return ESP_OK;
}

static esp_err_t apSettings_get_handler(httpd_req_t *req){
    
    // printf("apSettings_get_handler\n");

    char _appSSID[25];
    char _apPASS[17];
    char _apIP[16];
    char _apNETMASK[16];
    char _gateway[16];
    bool state=false;

    memset(_appSSID,0,25*sizeof(char));
    memset(_apPASS,0,17*sizeof(char));
    memset(_apIP,0,16*sizeof(char));
    memset(_apNETMASK,0,16*sizeof(char));
    memset(_gateway,0,16*sizeof(char));

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    // httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200);

    if(get_str_key(nvsHandle,AP_IP,_apIP)==0) {
      ESP_LOGW("AP_WiFi","NO IP FOR AP, USE DEFUALT %s",ap_ip);
      strcpy(_apIP,ap_ip);
    }

    if(get_str_key(nvsHandle,AP_NETMASK,_apNETMASK)==0) {
      ESP_LOGW("AP_WiFi","NO NETMASK  FOR AP, USE DEFUALT %s",netmask);
      strcpy(_apNETMASK,netmask);
    }

    if(get_str_key(nvsHandle,AP_GATEWAY,_gateway)==0) {
      ESP_LOGW("AP_WiFi","NO AP_GATEWAY  FOR AP, USE DEFUALT %s",_gateway);
      strcpy(_gateway,gateway);
    }

    
    if(get_str_key(nvsHandle,AP_SSID,_appSSID)==0) {
            ESP_LOGW("AP_WiFi","NO AP_SSID FOR AP, USE DEFUALT %s",ap_ssid);
            strcpy(_appSSID,ap_ssid);
    }
   
    if(get_str_key(nvsHandle,AP_PASSWORD,_apPASS)==0) {
            ESP_LOGW("AP_WiFi","NO PASS  FOR AP, USE DEFUALT %s",ap_pass);
            strcpy(_apPASS,ap_pass);
    }
  
    cJSON *ap_settings=cJSON_CreateObject();
    if (ap_settings == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE AP_SETTING OBJECT");
        goto end;
    }

    cJSON *ssid_item = cJSON_CreateString(_appSSID);
    if (ssid_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE SSID OBJECT");
        goto end;
    }

    cJSON *password_item = cJSON_CreateString(_apPASS);
    if (password_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE PASSWORD OBJECT");
        goto end;
    }

    cJSON *ip_item = cJSON_CreateString(_apIP);
    if (ip_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE IP OBJECT");
        goto end;
    }

    cJSON *netmask_item = cJSON_CreateString(_apNETMASK);
    if (netmask_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE NETMASK OBJECT");
        goto end;
    }

    cJSON *gateway_item = cJSON_CreateString(_gateway);
    if (gateway_item == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE GATEWAY OBJECT");
        goto end;
    }

    cJSON_AddItemToObject(ap_settings,"ipaddress",ip_item);
    cJSON_AddItemToObject(ap_settings,"gateway",gateway_item);
    cJSON_AddItemToObject(ap_settings,"ssid",ssid_item);
    cJSON_AddItemToObject(ap_settings,"password",password_item);
    cJSON_AddItemToObject(ap_settings,"networkmask",netmask_item);

    char *content = cJSON_Print(ap_settings);
    state=true;
    httpd_resp_sendstr(req, content);
    goto end;
    end:
    cJSON_Delete(ap_settings);
    if (state) free(content);

    return ESP_OK;
}

static esp_err_t gate_settings_handler(httpd_req_t *req){

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200);

    char buffer[256];
    memset(buffer,0,128*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= 0) {httpd_resp_sendstr(req, "false"); return ESP_FAIL;}
    // ESP_LOGI(HTTP_TAG,"Received len %d   content:%s",req->content_len,buffer);
   
    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL){
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }

    char R1_val[2];
    char R2_val[2];

    char R1_time[3];
    char R2_time[3];
    char _type[3];
    
    char _waittime[3];


    char _Rmode[2];
    char _logMod[2];
    char _port[6];

    bzero(_Rmode,2);
    bzero(_logMod,2);
    bzero(_waittime,3);

    bzero(_type,3);
    bzero(R1_time,3);
    bzero(R2_time,3);
    bzero(R1_val,2);
    bzero(R2_val,2);
    bzero(_port,6);



    cJSON *_mode = cJSON_GetObjectItemCaseSensitive(root, "mode");
    if (!cJSON_IsString(_mode) || (_mode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"Mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *_logMode = cJSON_GetObjectItemCaseSensitive(root, "log_mode");
    if (!cJSON_IsString(_logMode) || (_logMode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"log_mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}


    cJSON *R1_mode = cJSON_GetObjectItemCaseSensitive(root, "r1");
    if (!cJSON_IsString(R1_mode) || (R1_mode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"R1_mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *R2_mode = cJSON_GetObjectItemCaseSensitive(root, "r2");
    if (!cJSON_IsString(R2_mode) || (R2_mode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"R2_mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}
    
    cJSON *holdtime1_mode = cJSON_GetObjectItemCaseSensitive(root, "holdtime1");
    if (!cJSON_IsString(holdtime1_mode) || (holdtime1_mode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"holdtime1_mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}
    cJSON *holdtime2_mode = cJSON_GetObjectItemCaseSensitive(root, "holdtime2");
    if (!cJSON_IsString(holdtime2_mode) || (holdtime2_mode->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"holdtime2_mode value is invalid  ");  httpd_resp_sendstr(req, "false"); goto end;}

    cJSON *type_item = cJSON_GetObjectItemCaseSensitive(root, "bits");
    if (!cJSON_IsString(type_item) || (type_item->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"CardType value is invalid");  httpd_resp_sendstr(req, "false,CardType value is invalid"); goto end;}

    cJSON *_portVal = cJSON_GetObjectItemCaseSensitive(root, "port");
    if (!cJSON_IsString(_portVal) || (_portVal->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"port value is invalid");  httpd_resp_sendstr(req, "false,Port value is invalid"); goto end;}

    cJSON *_waitVal = cJSON_GetObjectItemCaseSensitive(root, "wait");
    if (!cJSON_IsString(_waitVal) || (_waitVal->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"_waitVal value is invalid");  httpd_resp_sendstr(req, "false,WaitVal value is invalid"); goto end;}

    cJSON *_R_type = cJSON_GetObjectItemCaseSensitive(root, "type");
    if (!cJSON_IsNumber(_R_type)) {httpd_resp_sendstr(req,"false"); ESP_LOGE(HTTP_TAG,"_R_type values is not number"); httpd_resp_sendstr(req, "false,WaitVal value is invalid");goto end;}

    cJSON *lon_item = cJSON_GetObjectItemCaseSensitive(root, "lng");
    if (!cJSON_IsString(lon_item) || (lon_item->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"lon_item value is invalid");  httpd_resp_sendstr(req, "false,Longitude value is invalid"); goto end;}

    cJSON *lat_item = cJSON_GetObjectItemCaseSensitive(root, "lat");
    if (!cJSON_IsString(lat_item) || (lat_item->valuestring == NULL)) { ESP_LOGE(HTTP_TAG,"port value is invalid");  httpd_resp_sendstr(req, "false,Latitude value is invalid"); goto end;}
    
    bzero(longitude,21);
    bzero(latitude,21);
    strcpy(latitude, lat_item->valuestring);
    strcpy(longitude, lon_item->valuestring);
    
  	strcpy(_waittime, _waitVal->valuestring);
  	strcpy(R1_val, R1_mode->valuestring);
    strcpy(R2_val, R2_mode->valuestring);
    strcpy(R2_time, holdtime2_mode->valuestring);
    strcpy(R1_time, holdtime1_mode->valuestring);
    strcpy(_type, type_item->valuestring);
    strcpy(_Rmode, _mode->valuestring);
    strcpy(_logMod, _logMode->valuestring);
    strcpy(_port, _portVal->valuestring);
    R_type=_R_type->valueint;
    
 //  R_type  READER_TYPE

    int val=0;
    val=atoi(_waittime);
    if (val ==0 ) wait_time=5; 
    else if (val > 60 ) wait_time=60; 
    else wait_time=val;

    val=0;
    val=atoi(_type);
    
    R1_call=atoi(R1_val);
    R2_call=atoi(R2_val);

    Out1_time=atoi(R1_time);
    Out2_time=atoi(R2_time);

    isStandalone=atoi(_Rmode);
    mod_log=atoi(_logMod);
    if ((atoi(_port)>=0)&&(atoi(_port)<= 65535)&&(atoi(_port)!=80)) port=atoi(_port);
    else  { ESP_LOGE(HTTP_TAG,"port value is invalid_2");  httpd_resp_sendstr(req, "false,PORT value is invalid"); goto end;}

    if(val==1) card_type=CARD_32_BIT;
    else if(val==2) card_type=CARD_34_BIT;
    else if(val==3) card_type=CARD_26_BIT;

    
    set_nvs_int_value(nvsHandle,CARD_TYPE,card_type);
   
    set_nvs_int_value(nvsHandle,R1_MODE,R1_call);
    set_nvs_int_value(nvsHandle,R2_MODE,R2_call);
    
    set_nvs_int_value(nvsHandle,G1_OPEN_TIME,Out1_time);
    set_nvs_int_value(nvsHandle,G2_OPEN_TIME,Out2_time);

    set_nvs_int_value(nvsHandle,READER_MODE,isStandalone); 
    set_nvs_int_value(nvsHandle,LOG_MODE,mod_log); 

    set_nvs_int_value(nvsHandle,PORT,port); 
    set_nvs_int_value(nvsHandle,WAITTIME,wait_time);
    set_nvs_int_value(nvsHandle,READER_TYPE,R_type); 
    
    if(add_str_key(nvsHandle, LONGITUDE, longitude)) val=0;
    if(add_str_key(nvsHandle, LATITUDE, latitude)) val=0;
    
    if(xTimerChangePeriod(tmr_R1_Relay, (Out1_time *1000)/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)    ESP_LOGI(HTTP_TAG,"Gate1 Openning time changed");
    if(xTimerChangePeriod(tmr_R2_Relay,(Out2_time *1000)/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)    ESP_LOGI(HTTP_TAG,"Gate2 Openning time changed");
    
    if(xTimerChangePeriod(tmr_R1_last, (wait_time *1000)/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)    ESP_LOGI(HTTP_TAG,"R1_Wait time changed");
    if(xTimerChangePeriod(tmr_R2_last,(wait_time *1000)/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)    ESP_LOGI(HTTP_TAG,"R2_Wait Openning time changed");
    
    ESP_LOGI(HTTP_TAG,"Reader Settings Changed Successfully cardtype:%d",card_type);
    httpd_resp_sendstr(req, "true");
    goto end;
    end:
        cJSON_Delete(root);
    return ESP_OK;
}

static esp_err_t get_gate_settings_handler(httpd_req_t *req){
    
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    // httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200);
     
    cJSON *gates=cJSON_CreateObject();
    if (gates == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE card_info OBJECT");
        goto end;
    }

    char _R1_call[5];
    bzero(_R1_call,5);
    sprintf(_R1_call,"%d",R1_call);
    cJSON *r1=cJSON_CreateString(_R1_call);
    if (r1 == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE r1  OBJECT");
        goto end;
    }
    
    char _R2_call[5];
    bzero(_R2_call,5);
    sprintf(_R2_call,"%d",R2_call);

    cJSON *r2=cJSON_CreateString(_R2_call);
    if (r2 == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE r2  OBJECT");
        goto end;
    }
    char _Out1_time[5];
    bzero(_Out1_time,5);
    sprintf(_Out1_time,"%d",Out1_time);

    cJSON *t1=cJSON_CreateString(_Out1_time);
    if (t1 == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE t1  OBJECT");
        goto end;
    }
    char _Out2_time[5];
    bzero(_Out2_time,5);
    sprintf(_Out2_time,"%d",Out2_time);
    cJSON *t2=cJSON_CreateString(_Out2_time);
    if (t2 == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE t2  OBJECT");
        goto end;
    }

    char _Rmode[5];
    bzero(_Rmode,5);
    sprintf(_Rmode,"%d",isStandalone);
    cJSON *_mode=cJSON_CreateString(_Rmode);
    if (_mode == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Reader Mode  OBJECT");
        goto end;
    }

    char _logMod[5];
    bzero(_logMod,5);
    sprintf(_logMod,"%d",mod_log);
    cJSON *_logMode=cJSON_CreateString(_logMod);
    if (_logMode == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE _logMode  OBJECT");
        goto end;
    }

    char _type[5];
    bzero(_type,5);

    if(card_type==CARD_32_BIT) sprintf(_type,"1");
    else if(card_type==CARD_34_BIT) sprintf(_type,"2");
    else if(card_type==CARD_26_BIT) sprintf(_type,"3");

    cJSON *typ=cJSON_CreateString(_type);
    if (typ == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE typ  OBJECT");
        goto end;
    }
    
    char _port[6];
    bzero(_port,6);

   sprintf(_port,"%d",port);

    cJSON *_Port=cJSON_CreateString(_port);
    if (_Port == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE PORT  OBJECT");
        goto end;
    }

    char wait[6];
    bzero(wait,6);

   sprintf(wait,"%d",wait_time);

    cJSON *_wait=cJSON_CreateString(wait);
    if (_wait == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Wait  OBJECT");
        goto end;
    }
    
    if(get_nvs_int_value(nvsHandle,READER_TYPE))  R_type=get_nvs_int_value(nvsHandle,READER_TYPE);

    cJSON *_R_type=cJSON_CreateNumber(R_type);
    if (_R_type == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"_R_type to CREATE Id  OBJECT");
        goto end;
    }

    cJSON *latitude_item = cJSON_CreateString(latitude);
    if (latitude_item == NULL){ESP_LOGE("MQTT","Failed to CREATE latitude_item");   goto end;}

    cJSON *longitude_item = cJSON_CreateString(longitude);
    if (longitude_item == NULL){ESP_LOGE("MQTT","Failed to CREATE longitude_item");   goto end;}

    cJSON_AddItemToObject(gates,"r1",r1);
    cJSON_AddItemToObject(gates,"r2",r2);
    cJSON_AddItemToObject(gates,"holdtime1",t1);
    cJSON_AddItemToObject(gates,"holdtime2",t2);
    cJSON_AddItemToObject(gates,"bits",typ);
    cJSON_AddItemToObject(gates,"mode",_mode);
    cJSON_AddItemToObject(gates,"log_mode",_logMode);
    cJSON_AddItemToObject(gates,"port",_Port);
    cJSON_AddItemToObject(gates,"wait",_wait);
    cJSON_AddItemToObject(gates,"type",_R_type);
    
    cJSON_AddItemToObject(gates,"lat"     ,latitude_item );
    cJSON_AddItemToObject(gates,"lng"    ,longitude_item);


    char *content = cJSON_Print(gates);
    httpd_resp_sendstr(req, content);
    free(content);
    goto end;
    end:
        cJSON_Delete(gates);
   
    return ESP_OK;
}

static esp_err_t get_lastdenied_handler(httpd_req_t *req){
   
    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    // httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, HTTPD_200);
     
    char state[6];  
    char msg[17];

    bzero(state,6);
    bzero(msg,17);

    sprintf(state,"false");
    sprintf(msg,"00000000");

    if(lastdenied!=0){
        char path[24];
        bzero(path,24);
        char val[12];
        bzero(val,12);
        sprintf(val,"%08x",lastdenied);
        sprintf(path,"/spiffs/%s",val);
        struct stat st; 
        if (stat(path, &st) != 0) {
            sprintf(state,"true");
            sprintf(msg,"%08x",lastdenied);
        }
    }            

    cJSON *info=cJSON_CreateObject();
    if (info == NULL){
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE info OBJECT");
            goto sub_end;
        }

    cJSON *status=cJSON_CreateString(state);
    if (status == NULL){
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE state  OBJECT");
            goto sub_end;
        }

    cJSON *message=cJSON_CreateString(msg);
    if (message == NULL){
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE msg  OBJECT");
            goto sub_end;
        }

    cJSON_AddItemToObject(info,"status",status);
    cJSON_AddItemToObject(info,"message",message);

    char *content = cJSON_Print(info);
    httpd_resp_sendstr(req, content);
    free(content);
    goto sub_end;
    sub_end:
        cJSON_Delete(info);
       
    return ESP_OK;
}

static esp_err_t setschedule_handler (httpd_req_t *req){

    httpd_resp_set_hdr(req,"Content-Type:", "text/html");
    httpd_resp_set_status(req, HTTPD_200);

    char buffer[BUFFER_512_B_LEN];
    uint8_t id=0;
    memset(buffer,0,BUFFER_512_B_LEN*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= 0) {httpd_resp_sendstr(req, "false"); return ESP_FAIL;}
    ESP_LOGI(HTTP_TAG,"Received len %d  %s ",req->content_len,buffer);

    const cJSON *role = NULL;
    const cJSON *roles = NULL;
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL)
    {
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }
    
    cJSON *Id_item=cJSON_GetObjectItemCaseSensitive(root,"id");
    if (Id_item == NULL){ESP_LOGE(HTTP_TAG,"Id is invalid"); httpd_resp_sendstr(req,"false"); goto end;}
    id=Id_item->valueint;

    roles = cJSON_GetObjectItemCaseSensitive(root, "roles");
    cJSON_ArrayForEach(role, roles)
    {
        cJSON *_day = cJSON_GetObjectItemCaseSensitive(role, "day");
        cJSON *_status = cJSON_GetObjectItemCaseSensitive(role, "sta");
        cJSON *_sthour = cJSON_GetObjectItemCaseSensitive(role, "sth");
        cJSON *_stmin = cJSON_GetObjectItemCaseSensitive(role, "stm");
        cJSON *_endhour = cJSON_GetObjectItemCaseSensitive(role, "edh");
        cJSON *_endmin = cJSON_GetObjectItemCaseSensitive(role, "edm");

        if (!cJSON_IsNumber(_day) || !cJSON_IsNumber(_status)|| !cJSON_IsNumber(_sthour)|| !cJSON_IsNumber(_stmin)|| !cJSON_IsNumber(_endhour)|| !cJSON_IsNumber(_endmin))
        {
            httpd_resp_sendstr(req,"false");
            ESP_LOGE(HTTP_TAG,"values is not number");
            goto end;
        }

        if ((_day->valueint >DAYS_OF_WEEK) || (_status->valueint >4)|| (_sthour->valueint >23)|| (_stmin->valueint >59)|| (_endhour->valueint >23)|| (_endmin->valueint >59))
        {
            httpd_resp_sendstr(req,"false");
            ESP_LOGE(HTTP_TAG,"values out of range >");
            goto end;
        }
        if ((_day->valueint <0) || (_status->valueint <0)|| (_sthour->valueint <0)|| (_stmin->valueint <0)|| (_endhour->valueint <0)|| (_endmin->valueint <0))
        {
            httpd_resp_sendstr(req,"false");
            ESP_LOGE(HTTP_TAG,"values out of range >");
            goto end;
        }
        if(id==SCHUDLE_ID_1){
                    schedule_0[(_day->valueint)-1].id=_day->valueint;
                    schedule_0[(_day->valueint)-1].status=_status->valueint;
                    schedule_0[(_day->valueint)-1].str_time.st_hour =_sthour->valueint;
                    schedule_0[(_day->valueint)-1].str_time.st_min =_stmin->valueint;
                    schedule_0[(_day->valueint)-1].end_time.st_hour =_endhour->valueint;
                    schedule_0[(_day->valueint)-1].end_time.st_min =_endmin->valueint;
        }
        else if (id==SCHUDLE_ID_2){
                    schedule_1[(_day->valueint)-1].id=_day->valueint;
                    schedule_1[(_day->valueint)-1].status=_status->valueint;
                    schedule_1[(_day->valueint)-1].str_time.st_hour =_sthour->valueint;
                    schedule_1[(_day->valueint)-1].str_time.st_min =_stmin->valueint;
                    schedule_1[(_day->valueint)-1].end_time.st_hour =_endhour->valueint;
                    schedule_1[(_day->valueint)-1].end_time.st_min =_endmin->valueint;
        }
        else if (id==SCHUDLE_ID_3){
                    schedule_2[(_day->valueint)-1].id=_day->valueint;
                    schedule_2[(_day->valueint)-1].status=_status->valueint;
                    schedule_2[(_day->valueint)-1].str_time.st_hour =_sthour->valueint;
                    schedule_2[(_day->valueint)-1].str_time.st_min =_stmin->valueint;
                    schedule_2[(_day->valueint)-1].end_time.st_hour =_endhour->valueint;
                    schedule_2[(_day->valueint)-1].end_time.st_min =_endmin->valueint;
        }
        else{
                    schedule_0[(_day->valueint)-1].id=_day->valueint;
                    schedule_0[(_day->valueint)-1].status=_status->valueint;
                    schedule_0[(_day->valueint)-1].str_time.st_hour =_sthour->valueint;
                    schedule_0[(_day->valueint)-1].str_time.st_min =_stmin->valueint;
                    schedule_0[(_day->valueint)-1].end_time.st_hour =_endhour->valueint;
                    schedule_0[(_day->valueint)-1].end_time.st_min =_endmin->valueint;
        }

    }

    FILE *file;
    if (id==SCHUDLE_ID_1) file = fopen("/spiffs/sched_0", "w");
    else if (id==SCHUDLE_ID_2) file = fopen("/spiffs/sched_1", "w");
    else if (id==SCHUDLE_ID_3) file = fopen("/spiffs/sched_2", "w");
    else file = fopen("/spiffs/sched_0", "w");
    
    if (file == NULL) { 
            httpd_resp_sendstr(req,"false");
            ESP_LOGE(HTTP_TAG, "Failed to open file for writing");
            goto end;}
    if (id==SCHUDLE_ID_1) 
            for(uint8_t i=0;i<DAYS_OF_WEEK;i++)
            {
                fprintf(file, "%d:%d:%02d:%02d:00:%02d:%02d:00 \n",schedule_0[i].id,schedule_0[i].status,schedule_0[i].str_time.st_hour,schedule_0[i].str_time.st_min,
                schedule_0[i].end_time.st_hour,schedule_0[i].end_time.st_min);
            }    
    else if (id==SCHUDLE_ID_2) 
            for(uint8_t i=0;i<DAYS_OF_WEEK;i++)
            {
                fprintf(file, "%d:%d:%02d:%02d:00:%02d:%02d:00 \n",schedule_1[i].id,schedule_1[i].status,schedule_1[i].str_time.st_hour,schedule_1[i].str_time.st_min,
                schedule_1[i].end_time.st_hour,schedule_1[i].end_time.st_min); 
            }
    else if (id==SCHUDLE_ID_3) 
            for(uint8_t i=0;i<DAYS_OF_WEEK;i++)
            {
                fprintf(file, "%d:%d:%02d:%02d:00:%02d:%02d:00 \n",schedule_2[i].id,schedule_2[i].status,schedule_2[i].str_time.st_hour,schedule_2[i].str_time.st_min,
                schedule_2[i].end_time.st_hour,schedule_2[i].end_time.st_min); 
            }
    
    fclose(file);


    vTaskDelay(DELAY_100_MS);
    if (id==SCHUDLE_ID_1) 
    {
        upload_profile(schedule_0,0);
    }
    else if (id==SCHUDLE_ID_2) 
    {
        upload_profile(schedule_1,1);
    }
    else if (id==SCHUDLE_ID_3) 
    {
        upload_profile(schedule_2,2);
    }

    httpd_resp_sendstr(req,"true");
goto end;
end:
    cJSON_Delete(root);
return ESP_OK;
}

#ifdef  LOG_DBG

static esp_err_t del_device_log_get_handler (httpd_req_t *req)
{
    struct stat st;
    if (stat("/spiffs/sch_id", &st) != 0) {
        ESP_LOGE(HTTP_TAG,"Devicelog not exists ");
        return -1;
    }
    if (remove("/spiffs/sch_id") != 0) {
        ESP_LOGE(HTTP_TAG, "Remove devicelog failed");
        return 0;
    }
    return ESP_OK;
}

static esp_err_t device_log_get_handler (httpd_req_t *req)
{
    FILE *file;
    char line[24];
    char _line[28];
    file = fopen("/spiffs/sch_id", "r");
    if (file == NULL) 
    { 
        ESP_LOGE("HTTP_LOG", "Failed to open file");  
        return 1;
    }
    do{                          
            bzero(line,24);
            bzero(_line,26);
            fgets(line, sizeof(line), file);
            if(feof(file)) 
                break;
            sprintf(_line,"%s \n",line);
            httpd_resp_send_chunk(req, _line,strlen(_line));
    }while(!feof(file));
    fclose(file); 
    httpd_resp_send_chunk(req,NULL,0);
    return ESP_OK;
}

#endif 

static esp_err_t getschedule_handler (httpd_req_t *req)
{
    char buffer[BUFFER_48_B_LEN];
    memset(buffer,0,BUFFER_48_B_LEN*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= 0)
    {
        httpd_resp_sendstr(req,"false"); 
        return ESP_FAIL;
    }
    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL)
    {
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto _end;
    }
    cJSON *id_item=cJSON_GetObjectItemCaseSensitive(root,"id");
    if (id_item== NULL)
    {
        ESP_LOGE(HTTP_TAG,"Id is invalid"); 
        httpd_resp_sendstr(req,"false,Id is invalid");  
        goto _end;
    }

    uint8_t id=id_item->valueint;
    if ((id<SCHUDLE_ID_1)||(id>SCHUDLE_ID_3)) 
    {
        ESP_LOGE(HTTP_TAG,"Id is invalid"); 
        httpd_resp_sendstr(req,"false,Id is invalid"); 
        goto _end;
    }

    httpd_resp_set_hdr(req,"Content-Type:", "application/json");
    httpd_resp_set_status(req, HTTPD_200);

    cJSON *roles = NULL;
    cJSON *role = NULL;
    cJSON * _day;
    cJSON * _status;
    cJSON * _sth;
    cJSON * _stm;
    cJSON * _edh;
    cJSON * _edm;

    cJSON *_schedule = cJSON_CreateObject();
    if (_schedule == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE _schedule ");
        goto end;

    }
    
    cJSON *Id=cJSON_CreateNumber(id);
    if (Id == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Id  OBJECT");
        goto end;
    }
    cJSON_AddItemToObject(_schedule,"id",Id);

    roles = cJSON_CreateArray();
    if (roles == NULL)
    {
        httpd_resp_sendstr(req, "false");
        ESP_LOGE(HTTP_TAG,"Failed to CREATE Roles Array");
        goto end;
    }

    cJSON_AddItemToObject(_schedule, "roles", roles);

    for (size_t index = 0; index < DAYS_OF_WEEK ; ++index)
    {
        role = cJSON_CreateObject();
        if (role == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE role  OBJECT");
            goto end;

        }
        cJSON_AddItemToArray(roles, role);

        if(id==SCHUDLE_ID_1)           _day=cJSON_CreateNumber(schedule_0[index].id);
        else if (id==SCHUDLE_ID_2 )    _day=cJSON_CreateNumber(schedule_1[index].id);
        else                _day=cJSON_CreateNumber(schedule_2[index].id);
        if (_day == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _day  OBJECT");
            goto end;
        }
        if(id==SCHUDLE_ID_1)           _status=cJSON_CreateNumber(schedule_0[index].status);
        else if (id==SCHUDLE_ID_2 )    _status=cJSON_CreateNumber(schedule_1[index].status);
        else                _status=cJSON_CreateNumber(schedule_2[index].status);
        if (_status == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _status  OBJECT");
            goto end;
        }

        if(id==SCHUDLE_ID_1)           _sth=cJSON_CreateNumber(schedule_0[index].str_time.st_hour);
        else if (id==SCHUDLE_ID_2 )    _sth=cJSON_CreateNumber(schedule_1[index].str_time.st_hour);
        else                _sth=cJSON_CreateNumber(schedule_2[index].str_time.st_hour);

        if (_sth == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _sth  OBJECT");
            goto end;
        }

        if(id==SCHUDLE_ID_1)           _stm=cJSON_CreateNumber(schedule_0[index].str_time.st_min);
        else if (id==SCHUDLE_ID_2 )    _stm=cJSON_CreateNumber(schedule_1[index].str_time.st_min);
        else                _stm=cJSON_CreateNumber(schedule_2[index].str_time.st_min);
        if (_stm == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _stm  OBJECT");
            goto end;
        }

        if(id==SCHUDLE_ID_1)           _edh=cJSON_CreateNumber(schedule_0[index].end_time.st_hour);
        else if (id==SCHUDLE_ID_2 )    _edh=cJSON_CreateNumber(schedule_1[index].end_time.st_hour);
        else                _edh=cJSON_CreateNumber(schedule_2[index].end_time.st_hour);
        
        if (_edh == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _edh  OBJECT");
            goto end;
        }

        if(id==SCHUDLE_ID_1)           _edm=cJSON_CreateNumber(schedule_0[index].end_time.st_min);
        else if (id==SCHUDLE_ID_2 )    _edm=cJSON_CreateNumber(schedule_1[index].end_time.st_min);
        else                _edm=cJSON_CreateNumber(schedule_2[index].end_time.st_min);
        
        if (_edm == NULL)
        {
            httpd_resp_sendstr(req, "false");
            ESP_LOGE(HTTP_TAG,"Failed to CREATE _edm  OBJECT");
            goto end;
        }

        cJSON_AddItemToObject(role, "day", _day);
        cJSON_AddItemToObject(role, "sta", _status);
        cJSON_AddItemToObject(role, "sth", _sth);
        cJSON_AddItemToObject(role, "stm", _stm);
        cJSON_AddItemToObject(role, "edh", _edh);
        cJSON_AddItemToObject(role, "edm", _edm);
    }

  
    char *content = cJSON_Print(_schedule);
    httpd_resp_sendstr(req, content);
    free(content);
    goto end;

    end:
        cJSON_Delete(_schedule);
    
    goto _end;
    _end:
        cJSON_Delete(root);    
   
    return ESP_OK;
}

static esp_err_t reset_post_handler (httpd_req_t *req)
{
    httpd_resp_set_hdr(req,"Content-Type:", "text/html");
    httpd_resp_set_status(req, HTTPD_200);

    bool _state= true;
    char buffer[BUFFER_48_B_LEN];
    memset(buffer,0,BUFFER_48_B_LEN*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= 0) {httpd_resp_sendstr(req,"false"); return ESP_FAIL;}

    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL){
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        _state= false;
        goto end;
    }

    cJSON *mode_item=cJSON_GetObjectItemCaseSensitive(root,"mode");
    if (!cJSON_IsString(mode_item)||(mode_item->valuestring == NULL)){ESP_LOGE(HTTP_TAG,"MODE is invalid"); httpd_resp_sendstr(req,"false,MODE is invalid"); _state= false; goto end;}
/*
1  STA  station
2  AP   accesspoint
3  Both  both 
*/

    uint8_t _mode=0;
    char temp_val[9];
    memset(temp_val,0,9*sizeof(char));
    strcpy(temp_val,mode_item->valuestring);

    
    if(strcmp(temp_val,"station")==0) _mode=1;
    else if (strcmp(temp_val,"accesspoint")==0) _mode=2;
    else if (strcmp(temp_val,"both")==0) _mode=3;

    // if ((_mode<0)||(_mode>3)) {ESP_LOGE(HTTP_TAG,"MODE value is invalid"); httpd_resp_sendstr(req,"false,MODE is invalid"); _state= false; goto end;}

    // ESP_LOGI(HTTP_TAG,"Reseived:%d",_mode);
    set_nvs_int_value(nvsHandle,WiFi_MODE,_mode);
    
    goto end;
    end:
        cJSON_Delete(root);


    char state[6];  
    char msg[23];

    bzero(state,6);
    bzero(msg,23);

    sprintf(state,"false");
    sprintf(msg,"Null");
    if (_state){
            sprintf(state,"true"); sprintf(msg,"Restarting after 5 sec");
            cJSON *info=cJSON_CreateObject();
            if (info == NULL)
                {
                httpd_resp_sendstr(req, "false");
                ESP_LOGE(HTTP_TAG,"Failed to CREATE info OBJECT");
                goto sub_end;
                 }

            cJSON *status=cJSON_CreateString(state);
            if (status == NULL){
                httpd_resp_sendstr(req, "false");
                ESP_LOGE(HTTP_TAG,"Failed to CREATE state  OBJECT");
                goto sub_end;
                }

            cJSON *message=cJSON_CreateString(msg);
            if (message == NULL){
                httpd_resp_sendstr(req, "false");
                ESP_LOGE(HTTP_TAG,"Failed to CREATE msg  OBJECT");
                goto sub_end;
            }

            cJSON_AddItemToObject(info,"status",status);
            cJSON_AddItemToObject(info,"message",message);

            ESP_LOGE(HTTP_TAG, "status:%s message:%s ", state,msg); 
            char *content = cJSON_Print(info);
            httpd_resp_sendstr(req, content);
            free(content);
            goto sub_end;
            sub_end:
            cJSON_Delete(info);

            vTaskDelay(DELAY_5_SEC);
            ESP_LOGE("HTTP_RST","Restarting now.");
            fflush(stdout);
            esp_restart();
    }

    return ESP_OK;
}

static esp_err_t getMQTT_handler (httpd_req_t *req){
    
    bool _state=false;
    cJSON *MQTT_setting=cJSON_CreateObject();
    if (MQTT_setting == NULL){ httpd_resp_sendstr(req, "false");ESP_LOGE(HTTP_TAG,"Failed to CREATE MQTT_setting OBJECT"); goto end;}

    cJSON *server = cJSON_CreateString(MQTT_server);
    if (server == NULL){httpd_resp_sendstr(req, "false"); ESP_LOGE(HTTP_TAG,"Failed to CREATE server OBJECT"); goto end;}

    cJSON *username = cJSON_CreateString(MQTT_name);
    if (username == NULL){ httpd_resp_sendstr(req, "false"); ESP_LOGE(HTTP_TAG,"Failed to CREATE MQTT Username OBJECT"); goto end;}

    cJSON *password = cJSON_CreateString(MQTT_pass);
    if (password == NULL){httpd_resp_sendstr(req, "false"); ESP_LOGE(HTTP_TAG,"Failed to CREATE IP OBJECT"); goto end;}

    cJSON *port=cJSON_CreateNumber(MQTT_port);
    if (port == NULL){ httpd_resp_sendstr(req, "false"); ESP_LOGE(HTTP_TAG,"Failed to CREATE port OBJECT"); goto end;}

    cJSON *state=cJSON_CreateNumber(ismqtt);
    if (state == NULL) { httpd_resp_sendstr(req, "false"); ESP_LOGE(HTTP_TAG,"Failed to CREATE state  OBJECT"); goto end;}

    cJSON_AddItemToObject(MQTT_setting,"ismqtt",state);
    cJSON_AddItemToObject(MQTT_setting,"server",server);
    cJSON_AddItemToObject(MQTT_setting,"port",port);
    cJSON_AddItemToObject(MQTT_setting,"user",username);
    cJSON_AddItemToObject(MQTT_setting,"password",password);
   
    char *content = cJSON_Print(MQTT_setting);
    _state=true;
    httpd_resp_sendstr(req, content);
    goto end;
    end:
    cJSON_Delete(MQTT_setting);
    if (_state) free(content);

    return ESP_OK;
}

static esp_err_t setMQTT_handler (httpd_req_t *req){

    ESP_LOGI("MQTT SETTING","setMQTT_handler");
    char buffer[256];
    memset(buffer,0,256*sizeof(char));
    int ret=0;
    if ((ret = httpd_req_recv(req, buffer, sizeof(buffer))) <= 0) {httpd_resp_sendstr(req,"false"); return ESP_FAIL;}
    // ESP_LOGI("MQTT SETTING","%s",buffer);

    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL){
        httpd_resp_sendstr(req,"false");
        ESP_LOGE(HTTP_TAG,"Root is Empty");
        goto end;
    }
    cJSON *server_item=cJSON_GetObjectItemCaseSensitive(root,"server");
    if (server_item== NULL){ESP_LOGE(HTTP_TAG,"server_item is invalid"); httpd_resp_sendstr(req,"false,password is invalid");  goto end;}

    cJSON *user_item=cJSON_GetObjectItemCaseSensitive(root,"user");
    if (user_item== NULL){ESP_LOGE(HTTP_TAG,"user_item is invalid"); httpd_resp_sendstr(req,"false,user is invalid");  goto end;}

    cJSON *pass_item=cJSON_GetObjectItemCaseSensitive(root,"password");
    if (pass_item== NULL){ESP_LOGE(HTTP_TAG,"pass_item is invalid"); httpd_resp_sendstr(req,"false,password is invalid");  goto end;}

    cJSON *port = cJSON_GetObjectItemCaseSensitive(root, "port");   
    if (!cJSON_IsNumber(port)){ httpd_resp_sendstr(req,"false"); ESP_LOGE(HTTP_TAG,"port values is not number");  goto end;}

    cJSON *_ismqtt = cJSON_GetObjectItemCaseSensitive(root, "ismqtt");   
    if (!cJSON_IsNumber(_ismqtt)){ httpd_resp_sendstr(req,"false"); ESP_LOGE(HTTP_TAG,"ismqtt values is not number");  goto end;}

    // ESP_LOGI("MQTT SETTING","%s %s %s %d %d",server_item->valuestring,user_item->valuestring,pass_item->valuestring,port->valueint,_ismqtt->valueint);
    
    bzero(MQTT_server,51);
    bzero(MQTT_name,31);
    bzero(MQTT_pass,21);

    strcpy(MQTT_server,server_item->valuestring);
    strcpy(MQTT_name,user_item->valuestring);
    strcpy(MQTT_pass,pass_item->valuestring);
    uint8_t isMQTT=_ismqtt->valueint;
    if ((port->valueint>0)||(port->valueint<65536))  MQTT_port=port->valueint;   

    
    set_nvs_int_value(nvsHandle,MQTTPORT,MQTT_port);
    set_nvs_int_value(nvsHandle,MQTTSTATE,isMQTT);
    if((add_str_key(nvsHandle, MQTTSERVER,MQTT_server)) && (add_str_key(nvsHandle, MQTTUSER, MQTT_name)) && (add_str_key(nvsHandle, MQTTPASS, MQTT_pass)))
        ESP_LOGI("NVS_Getlogin","MQTT SETTINGS SAVED SUCCESSFULLY ");

    httpd_resp_sendstr(req,"true");
    
    goto end;

    end:
       cJSON_Delete(root);
        
    return ESP_OK;}

static esp_err_t multiusers_post_handler(httpd_req_t *req)
{
    char buf[5120];
    memset(buf,0,sizeof(buf));
    int ret=ACTION_OK;
    if ((ret = httpd_req_recv(req, buf,sizeof(buf))) <= 0) 
    {
        httpd_resp_set_hdr(req,"Content-Type:", "application/json");
        httpd_resp_set_status(req, HTTPD_200);
        httpd_resp_sendstr(req, "false"); 
        return ESP_FAIL;
    }
    ESP_LOGE(HTTP_TAG, "RECEIVED DATA LEN %d", ret);
    ESP_LOGI(HTTP_TAG,"STACK[%d]  HEAP[%d]",uxTaskGetStackHighWaterMark(NULL),(int)(xPortGetFreeHeapSize()/1024));
    
    const cJSON *count = NULL;
    const cJSON *action=NULL;

    uint8_t  status = 0;
    volatile uint8_t  err=ACTION_OK;
    volatile int cnt=-1;
    volatile int act=0;

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL)
    {
            
        err=EMPTY_ROOT; 
        goto end;
    }
    count = cJSON_GetObjectItemCaseSensitive(root, "Count");
    if ((count == NULL)||(!cJSON_IsNumber(count)))
    {
        err=RECORD_COUNT_ERROR;
        goto end;
    } 
    else 
    {
        cnt=count->valueint;
    }

    action = cJSON_GetObjectItemCaseSensitive(root,"Action");
    if ((action == NULL)||(!cJSON_IsNumber(action)))
    {
        err=ACTION_TYPE_ERROR;
        goto end;
    } 
    else 
    {
        act=action->valueint;
    }

    switch(act)
    {
        case ADDING_ACTION:
        {
            ESP_LOGI(HTTP_TAG, "Action :Multi Adding");
            err = add_multi_users(root,&status,cnt); 
            break;
        }
        case UPDATING_ACTION:
        {
            ESP_LOGI(HTTP_TAG, "Action :Multi Update");
            err = update_multi_users(root,&status,cnt); 
            break;
        }
        case DELETE_ACTION:
        {
            ESP_LOGI(HTTP_TAG, "Action :Multi Delete");
            err = delete_multi_users(root,&status,cnt); 
            break;
        }
        case ADDING_LITE_ACTION:
        {
            ESP_LOGI(HTTP_TAG, "Action :Multi Adding Lite");
            err = add_multi_users_lite(root,&status,cnt); 
            break;            
        }

    };


    goto end;       
    end:

    httpd_resp_set_status(req, HTTPD_200); 
    char msg[HTTP_MSG_LEN]={ZERO}; 

    // if(status == cnt) 
    // {
    //     sprintf(msg,"true,%d",status);
    // }
    // else 
    // {
        switch(err)
        {
                case ACTION_OK :
                {
                    if(status == cnt)
                    {
                        sprintf(msg,"true,%d",status);
                    }
                    else
                    {
                        sprintf(msg,"false,General Exception,%d",status);
                    }                    
                    break;
                }
                case EMPTY_ROOT:
                {
                    sprintf(msg,"false,Root is Empty,%d",status);
                    break;
                }
                case RECORD_COUNT_ERROR:
                {
                    sprintf(msg,"false,Count is not number,%d",status);
                    break;
                }
                case ADD_USER_ERROR:
                {
                    sprintf(msg,"false,Failed to add user,%d",status);
                    break;
                }
                case ADD_OVERFLOW_ERROR:
                {
                    sprintf(msg,"false,User Count OverFllow Max Cap:%d,%d",MAX_USER_COUNT,status);
                    break;
                }
                case USER_NAME_ERROR:
                {
                    sprintf(msg,"false,Name value is invalid,%d",status); 
                    break;
                }
                case USER_LASTNAME_ERROR:
                {
                    sprintf(msg,"false,Last Name value is invalid,%d",status);
                    break;
                }
                case USER_PHONE_ERROR:
                {
                    sprintf(msg,"false,Phone value is invalid,%d",status);
                    break;
                }
                case USER_CARDID_ERROR:
                {
                    sprintf(msg,"false,CardID value is invalid,%d",status);
                    break; 
                }
                case USER_GENDER_ERROR:
                {
                    sprintf(msg,"false,Gender value is invalid,%d",status);
                    break;
                }
                case USER_PROFILID_ERROR:
                {
                    sprintf(msg,"false,profileID value is invalid,%d",status);
                    break;
                }
                case CARD_ID_FORMAT_ERROR:
                {
                    sprintf(msg,"false,CardId format is invalid,%d",status);
                    break;
                }
                case ACTION_TYPE_ERROR:
                {
                    sprintf(msg,"false,Invalid Action,%d",status);
                    break;
                }
                case USER_ID_ERROR:
                {
                    sprintf(msg,"false,Invalid user_id,%d",status);
                    break;
                }
                case FILE_NOT_EXIST:
                {
                    sprintf(msg,"false,file is not exist,%d",status);
                    break;
                }
                case OPEN_FILE_ERROR:
                {
                    sprintf(msg,"false,faild to open file,%d",status);
                    break;
                }
                case DELETE_FILE_ERROR:
                {
                    sprintf(msg,"false,faild delete user,%d",status);
                    break;
                }
                case MORE_RECORD_ERROR:
                {
                    sprintf(msg,"false,Record Count Error ,%d",status);
                    break;
                }
                default:
                {
                   sprintf(msg,"false,General Exception,%d",status);
                }
  
        };
 
    // }

    httpd_resp_sendstr(req, msg);
    ESP_LOGE(HTTP_TAG,"%s",msg); 

    cJSON_Delete(root);    
    return ESP_OK;   
}

static const httpd_uri_t del_all={
    .method=HTTP_POST,
    .uri="/delall",
    .handler=del_all_users,
    .user_ctx=NULL

};

static const httpd_uri_t nodata={
    .method=HTTP_GET,
    .uri="/assets/no-data.png",
    .handler=nodata_handler,
    .user_ctx=NULL
};

static const httpd_uri_t logo={
    .method=HTTP_GET,
    .uri="/assets/logo.png",
    .handler=logo_handler,
    .user_ctx=NULL
};

static const httpd_uri_t mainJS={
    .method=HTTP_GET,
    .uri="/main.js",
    .handler=mainJS_handler,
    .user_ctx=NULL
};

static const httpd_uri_t polyfullsJS={
    .method=HTTP_GET,
    .uri="/polyfills.js",
    .handler=polyfillsJS_handler,
    .user_ctx=NULL
};

static const httpd_uri_t runtimeJS={
    .method=HTTP_GET,
    .uri="/runtime.js",
    .handler=runtimeJS_handler,
    .user_ctx=NULL
};

static const httpd_uri_t styleCSS={
    .method=HTTP_GET,
    .uri="/styles.css",
    .handler=style_handler,
    .user_ctx=NULL
};

static const httpd_uri_t wifi_connect={
    .method=HTTP_POST,
    .uri="/connect",
    .handler=wificonnect_handler,
    .user_ctx=NULL
};

static const httpd_uri_t wifiscan={

    .method=HTTP_GET,
    .uri="/wifiscan",
    .handler=wifiscan_handler,
    .user_ctx=NULL
};

static const httpd_uri_t log_list={
    .uri="/logs",
    .method=HTTP_POST,
    .handler=log_handler,
    .user_ctx=NULL
};

static const httpd_uri_t index_page = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_get_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t get_lastdenied ={
    .uri="/lastid",
    .method=HTTP_POST,
    .handler=get_lastdenied_handler,
    .user_ctx=NULL
};

static const httpd_uri_t set_login = {
    .uri       = "/setlogin",
    .method    = HTTP_POST,
    .handler   = set_login_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t timeDate_get = {
    .uri       = "/time",
    .method    = HTTP_POST,
    .handler   = get_timeDate_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t timeDate_post = {
    .uri       = "/settime",
    .method    = HTTP_POST,
    .handler   = set_timeDate_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t adduser_post = {
    .uri       = "/adduser",
    .method    = HTTP_POST,
    .handler   = adduser_post_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t updateUser={
    .method=HTTP_POST,
    .uri="/updateUser",
    .handler=updateUser_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t delusers_post={
    .uri="/deluser",
    .method=HTTP_POST,
    .handler=delusers_post_handler,
    .user_ctx=NULL
    };

static const httpd_uri_t station_post={
    .uri="/setsta",
    .method=HTTP_POST,
    .handler=sta_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t reset_post ={
    .uri="/reset",
    .method=HTTP_POST,
    .handler=reset_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t uplodlist_post={
    .uri="/importuser",
    .method=HTTP_POST,
    .handler=uplodlist_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t login_post={
    .uri="/login",
    .method=HTTP_POST,
    .handler=login_handler,
    .user_ctx=NULL
};

static const httpd_uri_t userlist_get={
    .uri="/userlist",
    .method=HTTP_GET,
    .handler=user_list_handler,
    .user_ctx=NULL
};

static const httpd_uri_t dellog_post={
    .uri="/dellog",
    .method=HTTP_POST,
    .handler=del_log_handler,
    .user_ctx=NULL
};

static const httpd_uri_t delalllog_post={
    .uri="/delalllog",
    .method=HTTP_POST,
    .handler=delall_log_handler,
    .user_ctx=NULL
};

static const httpd_uri_t ap_setting_post={
    .uri="/setapsets",
    .method=HTTP_POST,
    .handler=apSettings_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t ap_setting_get={
    .uri="/getapsets",
    .method=HTTP_POST,
    .handler=apSettings_get_handler,
    .user_ctx=NULL
};

static const httpd_uri_t gates_setting={
    .uri="/gatesset",
    .method=HTTP_POST,
    .handler=gate_settings_handler,
    .user_ctx=NULL
};

static const httpd_uri_t get_gates_setting={
    .uri="/gatesget",
    .method=HTTP_POST,
    .handler=get_gate_settings_handler,
    .user_ctx=NULL
};

static const httpd_uri_t sta_setting_post={
    .uri="/setstasets",
    .method=HTTP_POST,
    .handler=staSettings_post_handler,
    .user_ctx=NULL
};

static const httpd_uri_t setschedule_post={
    .uri="/setschedule",
    .method=HTTP_POST,
    .handler=setschedule_handler,
    .user_ctx=NULL
};

static const httpd_uri_t getschedule_post={
    .uri="/getschedule",
    .method=HTTP_POST,
    .handler=getschedule_handler,
    .user_ctx=NULL
};

static const httpd_uri_t setMQTTpost={
    .uri="/setmqtt",
    .method=HTTP_POST,
    .handler=setMQTT_handler,
    .user_ctx=NULL
};

static const httpd_uri_t getMQTTpost={
    .uri="/getmqtt",
    .method=HTTP_POST,
    .handler=getMQTT_handler,
    .user_ctx=NULL
};

#ifdef  LOG_DBG

static const httpd_uri_t device_log = {
    .uri       = "/getdevlog",
    .method    = HTTP_GET,
    .handler   = device_log_get_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t del_device_log = {
    .uri       = "/deldevlog",
    .method    = HTTP_GET,
    .handler   = del_device_log_get_handler,
    .user_ctx  = NULL
};
#endif


static const httpd_uri_t multiusers_post = {
    .uri       = "/multiusers",
    .method    = HTTP_POST,
    .handler   = multiusers_post_handler,
    .user_ctx  = NULL
};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "page not found ,404 error message");
    return ESP_FAIL;
}

httpd_handle_t start_webserver(void)
{
    if (!httpstate){
        httpd_handle_t server = NULL;
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.task_priority=2;
        config.stack_size=9216;
        config.server_port=80;
        config.max_uri_handlers = 38;
        // config.max_open_sockets=7;
        config.lru_purge_enable=true;
        config.recv_wait_timeout  = 30;                       
        config.send_wait_timeout  = 10;                        

        ESP_LOGI(HTTP_TAG, "Starting server on port: '%d'", config.server_port);
        if (httpd_start(&server, &config) == ESP_OK) {

            ESP_LOGI(HTTP_TAG, "Registering URI handlers");
                 
            // httpd_register_uri_handler(server, &assets_get);
            httpd_register_uri_handler(server, &index_page);
            httpd_register_uri_handler(server, &nodata);  
            httpd_register_uri_handler(server, &updateUser);   
            httpd_register_uri_handler(server, &set_login);
            httpd_register_uri_handler(server, &timeDate_post);
            httpd_register_uri_handler(server, &timeDate_get);
            httpd_register_uri_handler(server,&adduser_post);
            httpd_register_uri_handler(server,&multiusers_post);
            httpd_register_uri_handler(server,&delusers_post);
            httpd_register_uri_handler(server,&log_list);
            httpd_register_uri_handler(server,&station_post);
            httpd_register_uri_handler(server,&reset_post);
            httpd_register_uri_handler(server,&uplodlist_post);
            httpd_register_uri_handler(server, &wifiscan);
            httpd_register_uri_handler(server,&wifi_connect);
            httpd_register_uri_handler(server,&styleCSS);
            httpd_register_uri_handler(server,&logo);
            httpd_register_uri_handler(server,&mainJS);
            httpd_register_uri_handler(server,&polyfullsJS);
            httpd_register_uri_handler(server,&runtimeJS);
            httpd_register_uri_handler(server,&login_post);
            httpd_register_uri_handler(server,&userlist_get);
            httpd_register_uri_handler(server,&del_all);
            httpd_register_uri_handler(server,&dellog_post);
            httpd_register_uri_handler(server,&delalllog_post);
            httpd_register_uri_handler(server,&ap_setting_post);
            httpd_register_uri_handler(server,&ap_setting_get); 
            httpd_register_uri_handler(server,&gates_setting); 
            httpd_register_uri_handler(server,&sta_setting_post);
            httpd_register_uri_handler(server,&get_gates_setting); 
            httpd_register_uri_handler(server,&setschedule_post);
            httpd_register_uri_handler(server,&getschedule_post);
            httpd_register_uri_handler(server,&get_lastdenied);
            httpd_register_uri_handler(server,&setMQTTpost);
            httpd_register_uri_handler(server,&getMQTTpost);

            #ifdef  LOG_DBG
            httpd_register_uri_handler(server,&device_log);
            httpd_register_uri_handler(server,&del_device_log);  
            #endif        

            httpstate=true;            
            return server;
        }
        ESP_LOGI(HTTP_TAG, "Error starting server!");
        return NULL;
    }   
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    if(httpstate){
            httpd_stop(server);
            ESP_LOGW("HTTP","Server Stopped");
            httpstate=false;
    }
    
}

static esp_err_t delete_multi_users(cJSON *root,uint8_t * status , int count )
{
    const cJSON *groups = NULL;    
    const cJSON *user = NULL;
    esp_err_t err=ACTION_OK;
    int record_count=0;


    groups = cJSON_GetObjectItemCaseSensitive(root, "Groups");
    if(groups==NULL)
    {
        return err;
    }
    char cardId[CARD_ID_LENGTH +1 ];
    memset(cardId,ZERO,sizeof(cardId));

    cJSON_ArrayForEach(user, groups)
    {
            
            cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(user, "id");
            if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
            { 
                err=USER_CARDID_ERROR;          
                return err;
            }

            strcpy(cardId, pcardId_item->valuestring);
            if(strlen(cardId)!=CARD_ID_LENGTH) 
            { 
                err=CARD_ID_FORMAT_ERROR;
                return err;  
            }
            else 
            {

                if (del_user(cardId)==1) 
                { 
                    set_nvs_int_value(nvsHandle,USER_COUNT,--USERCOUNT); 
                    err=ZERO; 
                    (*status)++;      
                } 
                else 
                {
                    err=DELETE_FILE_ERROR;
                    return err;
                }
                
                ++record_count;
                if ( record_count > count )
                {
                    err=MORE_RECORD_ERROR;
                    return err;
                }
            }
    }
    return err;
}

static esp_err_t update_multi_users(cJSON *root,uint8_t * status , int count )
{
    const cJSON *groups = NULL;    
    const cJSON *user = NULL;
    esp_err_t err=ACTION_OK;
    esp_err_t errCode;
    int record_count=0;


    groups = cJSON_GetObjectItemCaseSensitive(root, "Groups");
    if(groups==NULL)
    {
        return err;
    }

    char cardId[CARD_ID_LENGTH +1];
    uint8_t profile=ZERO;

    memset(cardId,ZERO,sizeof(cardId));
    cJSON_ArrayForEach(user, groups)
    {
            
            cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(user, "id");
            if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
            { 
                err=USER_CARDID_ERROR;          
                return err;
            }
            cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(user, "val");
            if (profile_item == NULL) 
            { 
                err=USER_PROFILID_ERROR;
                return err;
            }
            else 
            {
                profile=profile_item->valueint;
                if ((profile <= 0)||(profile > 5 ))
                {
                    err=USER_PROFILID_ERROR;
                    return err;
                } 
            }
            strcpy(cardId, pcardId_item->valuestring);
            if(strlen(cardId)!=8) 
            { 
                err=CARD_ID_FORMAT_ERROR;
                return err;  
            }
            else 
            {
                
                errCode=update_user("FFFFFFFF",profile,"0","0",cardId,"0",'0');
                if(errCode==1)
                {
                        (*status)++;
                        err=ZERO;
                }
                else if (errCode==0) 
                { 
                    err=FILE_NOT_EXIST;
                    return err;
                }
                else 
                {
                    err=OPEN_FILE_ERROR;
                    return err;
                }

                ++record_count;
                if ( record_count > count )
                {
                    err=MORE_RECORD_ERROR;
                    return err;
                }
            }
  
    }


    return err;
}

static esp_err_t add_multi_users(cJSON *root,uint8_t * status , int count )
{
    const cJSON *groups = NULL;    
    const cJSON *user = NULL;
    esp_err_t err = ACTION_OK;
    esp_err_t errCode;
    int record_count=0;


    groups = cJSON_GetObjectItemCaseSensitive(root, "Groups");
    if(groups==NULL)
    {
        return err;
    }
    char name[DATA_LENGTH];
    char lastname[DATA_LENGTH];
    char cardId[CARD_ID_LENGTH +1];
    char gender='0';
    uint8_t profile=ZERO;
    char phone[DATA_LENGTH];
    char gender_val[2];

    memset(name,ZERO,sizeof(name));
    memset(lastname,ZERO,sizeof(lastname));
    memset(cardId,ZERO,sizeof(cardId));
    memset(phone,ZERO,sizeof(phone));

    cJSON_ArrayForEach(user, groups)
    {
            cJSON *pname_item = cJSON_GetObjectItemCaseSensitive(user, "name");
            if (!cJSON_IsString(pname_item) || (pname_item->valuestring == NULL)) 
            { 
                err=USER_NAME_ERROR;
                return err;
            }
            cJSON *plastname_item = cJSON_GetObjectItemCaseSensitive(user, "lastname");
            if (!cJSON_IsString(plastname_item) || (plastname_item->valuestring == NULL)) 
            { 
                err=USER_LASTNAME_ERROR;
                return err;
            }
            cJSON *pphone_item = cJSON_GetObjectItemCaseSensitive(user, "phone");
            if (!cJSON_IsString(pphone_item) || (pphone_item->valuestring == NULL)) 
            { 
                err=USER_PHONE_ERROR;
                return err;
            }
            cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(user, "cardID");
            if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
            { 
                err=USER_CARDID_ERROR;          
                return err;
            }
            cJSON *pgender_item = cJSON_GetObjectItemCaseSensitive(user, "gender");
            if (!cJSON_IsString(pgender_item) || (pgender_item->valuestring == NULL)) 
            { 
                err=USER_GENDER_ERROR;
                return err;
            }
            cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(user, "profileId");
            if (profile_item == NULL) 
            { 
                err=USER_PROFILID_ERROR;
                return err;
            }
            strcpy(cardId, pcardId_item->valuestring);
            if(strlen(cardId)!=CARD_ID_LENGTH) 
            { 
                err=CARD_ID_FORMAT_ERROR;
                return err;  
            }
            else 
            {
                strcpy(name, pname_item->valuestring);
                strcpy(lastname, plastname_item->valuestring);
                strcpy(phone, pphone_item->valuestring);
                strcpy(gender_val,pgender_item->valuestring);
                gender=gender_val[0]; 
                profile=profile_item->valueint;              
                if (USERCOUNT<MAX_USER_COUNT)
                    {
                        errCode=add_user(++userCounter,profile,name,lastname,cardId,phone,gender);
                        if(errCode==RETURN_OK)
                        {
                            ESP_LOGI(HTTP_TAG, "user added");
                            set_nvs_int_value(nvsHandle,USER_COUNT,++USERCOUNT);
                            set_nvs_int_value(nvsHandle,USER_COUNTER,userCounter);                         
                            (*status)++;
                            err=ACTION_OK;
                        }
                        else if(errCode==RETURN_ERROR_1)
                        {                     
                            ESP_LOGW(HTTP_TAG,"User is already exist ");
                            (*status)++;
                            userCounter--;
                            err=ACTION_OK;
                        }    
                        else
                        {  
                            userCounter--; 
                            err=ADD_USER_ERROR;
                            return err;
                        }

                        ++record_count;
                        if ( record_count > count )
                        {
                            err=MORE_RECORD_ERROR;
                            return err;
                        }
                        if(userCounter>MAX_USERCOUNTER ) 
                        {
                            userCounter=0; 
                        }      
                    }
                    else 
                    { 
                        err=ADD_OVERFLOW_ERROR;
                        return err;
                    }
      

            }
  
    }


    return err;
}

static esp_err_t add_multi_users_lite(cJSON *root,uint8_t * status , int count )
{
    const cJSON *groups = NULL;    
    const cJSON *user = NULL;
    esp_err_t err = ACTION_OK;
    esp_err_t errCode;
    int record_count=0;

    groups = cJSON_GetObjectItemCaseSensitive(root, "Groups");
    if(groups==NULL)
    {
        return err;
    }
    char cardId[CARD_ID_LENGTH +1];
    uint8_t profile=ZERO;

    memset(cardId,ZERO,sizeof(cardId));

    cJSON_ArrayForEach(user, groups)
    {

            cJSON *pcardId_item = cJSON_GetObjectItemCaseSensitive(user, "id");
            if (!cJSON_IsString(pcardId_item) || (pcardId_item->valuestring == NULL)) 
            { 
                err=USER_CARDID_ERROR;          
                return err;
            }

            cJSON *profile_item = cJSON_GetObjectItemCaseSensitive(user, "val");
            if (profile_item == NULL) 
            { 
                err=USER_PROFILID_ERROR;
                return err;
            }
            strcpy(cardId, pcardId_item->valuestring);
            if(strlen(cardId)!=CARD_ID_LENGTH) 
            { 
                err=CARD_ID_FORMAT_ERROR;
                return err;  
            }
            else 
            {

                profile=profile_item->valueint;              
                if (USERCOUNT<MAX_USER_COUNT)
                    {
                        errCode=add_user(++userCounter,profile,"0","0",cardId,"0",'0');
                        if(errCode==RETURN_OK)
                        {
                            ESP_LOGI(HTTP_TAG, "user added");
                            set_nvs_int_value(nvsHandle,USER_COUNT,++USERCOUNT);
                            set_nvs_int_value(nvsHandle,USER_COUNTER,userCounter);                         
                            (*status)++;
                            err=ACTION_OK;
                        }
                        else if(errCode==RETURN_ERROR_1)
                        {                     
                            ESP_LOGW(HTTP_TAG,"User is already exist ");
                            (*status)++;
                            userCounter--;
                            err=ACTION_OK;
                        }    
                        else
                        {  
                            userCounter--; 
                            err=ADD_USER_ERROR;
                            return err;
                        } 
                        if(userCounter>MAX_USERCOUNTER ) 
                        {
                            userCounter=0; 
                        }     
                    }
                    else 
                    { 
                        err=ADD_OVERFLOW_ERROR;
                        return err;
                    }
                    ++record_count;
                    if ( record_count > count )
                    {
                        err=MORE_RECORD_ERROR;
                        return err;
                    }
      

            }
  
    }

    return err;
}




