#ifndef  _HF_SNTP__H__
#define  _HF_SNTP__H__
#include <time.h>
#include <sys/time.h>
#include "esp_sntp.h"
#include "esp_log.h"
#include "ds3231.h"
#include "HF_cmd_list.h"


// #define NTP_ADR_LEN        25  
#define MS_TO_SEC          1000
#define MS_TO_MIN          MS_TO_SEC*60
// #define DEFUALT_INTERVAL   600 

// extern i2c_dev_t dev;
// 
// static char     NTP_Server_Adress[NTP_ADR_LEN]      ="10.34.4.100";  
// static int      interval                            =DEFUALT_INTERVAL;

void ntp_service_init(void);

void ntp_stop_RQ(void);

void ntp_restart_RQ(void);

int get_unix_time(struct tm rtcinfo);

#endif 