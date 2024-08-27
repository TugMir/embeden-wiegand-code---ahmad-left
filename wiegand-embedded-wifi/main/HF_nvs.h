#ifndef _HF_NVS__H__
#define _HF_NVS__H__

#include "esp_partition.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "HF_cmd_list.h"

#ifdef __cplusplus
extern "C" {
#endif


extern int get_total_logs(int * filecount);
extern int get_users_count();

// extern SemaphoreHandle_t mutex;



int get_str_key(nvs_handle_t handle, char* parameter,char* output);
int add_str_key(nvs_handle_t handle, char* parameter1, char* parameter2 );
int init_nvs_hf(nvs_handle_t* handle);
int get_nvs_int_value(nvs_handle_t nvsHandle,char* paramter);
void set_nvs_int_value(nvs_handle_t nvsHandle,char* paramter,int value);
bool login_check(nvs_handle_t nvsHandle,char * loginId, char* password);
void get_settings(nvs_handle_t *nvsHandle);
void reset_user_values(nvs_handle_t *nvsHandle);

#ifdef __cplusplus
}
#endif

#endif 



