#ifndef _HF_MQTT__H__
#define _HF_MQTT__H__

#include "esp_private/system_internal.h"
#include "soc/sens_periph.h"
#include "esp_event.h"
#include "esp_netif.h"
#include <driver/adc.h>
#include "cJSON.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_wifi.h"
#include "mqtt_client.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "HF_cmd_list.h"

#ifdef __cplusplus
extern "C" {
#endif

// extern esp_mqtt_client_handle_t client;
// extern xTaskHandle task_mqttBrodcast;


extern void OpenDoor_R2(void);
extern void OpenDoor_R1(void);

extern void R1_CloseDoor(void);
extern void R2_CloseDoor(void);

void publish_QC(char* code , int reader);
void mqtt_brodcast_handller (void *arg);
void mqtt_app_start(void);


#ifdef __cplusplus
}
#endif

#endif 


