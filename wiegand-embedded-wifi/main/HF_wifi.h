#ifndef _HF_WIFI__H__
#define _HF_WIFI__H__

#include "esp_wifi.h"
#include "esp_event.h"
#include <esp_http_server.h>
#include "mdns.h"
#include "nvs.h"
#include "HF_cmd_list.h"

#define TAG_WiFi                        "WiFi"
#define AP_MAX_CONN                     4
#define AP_CHANNEL                      0
#define WIFI_MGMT_SBUF                  32
#define DEFAULT_SCAN_LIST_SIZE          20


#define TRY_COUNT                       3 


#define RSSI_THRESHOLD                  -85
#define MAC_ADD_LEN                     6

#ifdef __cplusplus
extern "C" {
#endif


// extern httpd_handle_t server;
// extern TaskHandle_t taskTCPsocket;
// extern TimerHandle_t tmr_startup;
// extern TaskHandle_t wifi_handler;
// extern nvs_handle_t nvsHandle; 


extern void stop_webserver(httpd_handle_t server);
extern httpd_handle_t start_webserver(void);
extern void ntp_service_init(void);
extern int writeDeviceLog(int type);
extern int get_str_key(nvs_handle_t handle, char* parameter,char* output);
extern int add_str_key(nvs_handle_t handle, char* parameter1, char* parameter2 );
extern int get_nvs_int_value(nvs_handle_t nvsHandle,char* paramter);
extern void set_nvs_int_value(nvs_handle_t nvsHandle,char* paramter,int value);

extern void ntp_stop_RQ(void);
extern void ntp_restart_RQ(void);

void startup_mode( TimerHandle_t xTimer );
void connect_wifi(char* ssid,char* pass);
void wifi_init_softap(void);
void wifi_task(void *arg);
void wifi_sta_start(void);
void print_scan(void);
void start_sta(void);
void scann(void);

// static void wifi_scan(void);


#ifdef __cplusplus
}
#endif


#endif 