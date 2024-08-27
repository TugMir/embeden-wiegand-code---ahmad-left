#ifndef _HF_STORAGE__H__
#define _HF_STORAGE__H__

#include <dirent.h> 
#include <sys/unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include "esp_spiffs.h"
#include "esp_heap_caps.h"
#include "ctype.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "HF_cmd_list.h"


#define MAX_ALLOWED_SIZE     1024*10            //max allowed size to save logs 
#define MAX_RECORD_SIZE      3000               //  max count of logs can be saved  
#define MAX_LOG_FILE_COUNT   360
#define LINE_SIZE            64
#define LOG_LINE_SIZE        96
#define PATH_SIZE            24
#define FILE_NAME_SIZE       16


#define LOG_FILE_NAME_LEN    12
#define FILE_NOT_EXISTS      -1

#define COMPARE_CHARACHER_LEN 3

#define SUCCESS_OPERATION     1
#define FAILED_OPERATION      0

#define ALLOWED              1
#define DENIED               0

#define CUSTOM_ALLOWED       3
#define CUSTOM_DENIED        4


#define ALLOWED_PROFILE      1
#define DENIED_PROFILE       2
#define SCHEDULE_1_PROFILE   3
#define SCHEDULE_2_PROFILE   4
#define SCHEDULE_3_PROFILE   5



#ifdef __cplusplus
extern "C" {
#endif

// extern nvs_handle_t nvsHandle; 
// extern SemaphoreHandle_t  xMutex;
// extern i2c_dev_t dev;
// extern struct tm rtcinfo;
// extern TaskHandle_t task_delFlogs;

extern int get_str_key(nvs_handle_t handle, char* parameter,char* output);
extern int add_str_key(nvs_handle_t handle, char* parameter1, char* parameter2 );
extern int get_nvs_int_value(nvs_handle_t nvsHandle,char* paramter);
extern void set_nvs_int_value(nvs_handle_t nvsHandle,char* paramter,int value);
extern esp_err_t ds3231_get_time(i2c_dev_t *dev, struct tm *time);


int init_storage(void);
int  show_file_list(void);
void del_old_flog(void);
bool is_exists(char* cardID);
void del_flogs (void *arg);
int get_recordCount(char* path);
int write_to_file(char* card,int8_t reader,uint8_t permission,int year,int mon,int day,int hour,int min,int sec);
int  add_user(uint32_t userCounter,uint8_t profile,char* name, char* lastname,char * cardId,char* phone, char gender);
int  update_user(char* id,uint8_t  profile,char* name, char* lastname,char * cardId,char* phone, char gender);
int del_log(char* name);
void read_file(char* path);
int clear_logs();
int get_users_count();
int get_total_logs(int * filecount);
void upload_profile(role_t* schedule,uint8_t id);
void unmount_spiffs(void);
void flash_info(void);

// FILE* open_file(char* path);
// bool is_exists_r(char* cardID,char* outdata);
int del_user(char* cardId);
int check_file(char* cardId);
void show_stored_file();


#ifdef __cplusplus
}
#endif


#endif 
