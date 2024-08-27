#include "HF_sntp.h"


static void update_ntp_time(void);

static void time_sync_notification_cb(struct timeval *tv);

static void print_current_time(struct tm* ctime);

static void update_rtc_time(struct tm* ctime);



static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGW("NTP","  ----Time Sync-----  ");
    update_ntp_time();
    if(ntpState)
    {
        sntp_set_sync_interval(ntpRate*MS_TO_SEC);
    }    
}

void ntp_service_init(void)
{
    ESP_LOGI("NTP", "Initializing SNTP");
    if(ntpState)
    {
        if (!ntp_Status)
        {
            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_setservername(0, ntpAddress);                        
            sntp_set_time_sync_notification_cb(time_sync_notification_cb);
            sntp_init();
            ntp_Status =true;
        }
        else 
        {
           /* no thing to do*/ 
        }

    }
    else 
    {
        ntp_Status =false;
    }
    
}

void ntp_stop_RQ(void)
{
    sntp_stop();
    ntp_Status =false;
    ESP_LOGW("NTP", "Stop SNTP");
}

void ntp_restart_RQ(void)
{
    bool ntpStatus=sntp_restart();

    if (ntpStatus)
    {
       ESP_LOGI("NTP", "Restart SNTP OK");  
       ntp_Status =true;
    }
    else 
    {
        ESP_LOGE("NTP", "SNTP was not initialized yet");
        ntp_service_init();
    }

}

static void update_ntp_time(void)
{
    time_t now = 0;
    struct tm timeinfo = {0};
    
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && retry < retry_count) 
    {
        ++retry;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    setenv("TZ", "MSK-3MSD,M3.5.0/2,M10.5.0/3", 1);
    tzset();
    time(&now);
    localtime_r(&now, &timeinfo);
    update_rtc_time(&timeinfo);
    print_current_time(&timeinfo);
}

static void print_current_time(struct tm* timeinfo)
{
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", timeinfo);
    ESP_LOGI("NTP", "The current date/time is: %s", strftime_buf);
}

static void update_rtc_time(struct tm* timeinfo)
{
    timeinfo->tm_mon++;
    timeinfo->tm_year+=1900;
    timeinfo->tm_wday++;

    if (ds3231_set_time(&dev, timeinfo) != ESP_OK) 
    { 
        ESP_LOGE("NTP","Failed to update RTC time");
    }
    else 
    {
        /*   No thigs to do*/
    }

    rtcinfo.tm_year =   timeinfo->tm_year;
    rtcinfo.tm_mon  =   timeinfo->tm_mon;
    rtcinfo.tm_mday =   timeinfo->tm_mday;
    rtcinfo.tm_hour =   timeinfo->tm_hour;    
    rtcinfo.tm_min  =   timeinfo->tm_min;   
    rtcinfo.tm_sec  =   timeinfo->tm_sec;
    rtcinfo.tm_wday =   timeinfo->tm_wday;

    ESP_LOGI("Time:","%04d-%02d-%02d %02d:%02d:%02d/ %02d",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, rtcinfo.tm_wday);

    timeinfo->tm_mon--;
    timeinfo->tm_wday--;
    timeinfo->tm_year-=1900;
}

int get_unix_time(struct tm rtcinfo)
{
      struct tm temp;
      time_t bday;
      temp.tm_mon = rtcinfo.tm_mon-1;
      temp.tm_mday = rtcinfo.tm_mday;
      temp.tm_year = rtcinfo.tm_year-1900;
      temp.tm_hour  =rtcinfo.tm_hour;
      temp.tm_sec  =rtcinfo.tm_sec;
      temp.tm_min  =rtcinfo.tm_min;
      bday = mktime(&temp);
      return bday;
}  