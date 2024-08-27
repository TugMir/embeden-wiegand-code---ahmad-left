#include "HF_storage.h"

static const char *TAG_Storage = "Storage";

// Show a Files ther are saved in Flash Memory  
int show_file_list() 
{
   int count=0;
   struct dirent *entry;
   DIR *dir = opendir("/spiffs");   
   if (dir == NULL) {
      return 0;
   }
//    printf("\n---------------------SPIFFS----------------------------\n" );
   while ((entry = readdir(dir)) != NULL) 
   {
       count++;
    //    if(count%4==0)
    //     printf("\n");
    //    printf("%s |", entry->d_name);
   }
//    printf("\n-------------------------------------------------------\n" );
   closedir(dir);
   return count;
}

//initialize and mount SPIFFS filesystem.
int init_storage()
{
    ESP_LOGI(TAG_Storage, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 20,
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) 
    {
        if (ret == ESP_FAIL) 
        {
            ESP_LOGE(TAG_Storage, "Failed to mount or format filesystem");
        } 
        else if (ret == ESP_ERR_NOT_FOUND) 
        {
            ESP_LOGE(TAG_Storage, "Failed to find SPIFFS partition");
        } 
        else
        {
            ESP_LOGE(TAG_Storage, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return FAILED_OPERATION;
    }
    ESP_LOGI(TAG_Storage, "SPIFFS Initialized");
    return SUCCESS_OPERATION;
}

void unmount_spiffs()
{
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG_Storage, "SPIFFS unmounted");
}

// Show SPIFFS TOTAL AND FREE SIZE 
void flash_info()
{
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) 
    {
        ESP_LOGE("Flash", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } 
    else 
    {
        ESP_LOGW("Flash:", "Partition size: total: %d, used: %d", total, used);
    }
}

// Read Content of File 

void read_file(char* path)
{
    FILE *file;
    struct stat st;
    if (stat(path, &st) == OK_RESULT) 
    {
        ESP_LOGI(TAG_Storage, "Opening file %s",path);
        file = fopen(path, "r");
        if (file == NULL) 
        { 
            ESP_LOGE(TAG_Storage, "Failed to open file for writing");
            return;
        }
        ESP_LOGI("Storage","Start Reading File");
        if(feof(file))
        {
            ESP_LOGE("Storage","Empty File");
        }
        
        char line[LINE_SIZE];
        int count=0;
        do
        {
            if (feof(file))
            {
                break;
            } 
            fgets(line, sizeof(line), file);
            if(!feof(file))
            {
                ESP_LOGI("Storage","%s", line);
                count++;
            }
            else
            {
                break;
            } 
                
        }while(!feof(file));

        ESP_LOGI("Storage","Total Count:%d",count);
        fclose(file);       
    }
    else 
    {
        ESP_LOGI("Storage","File is not exists");
    }
}

// Get Number of Recourds inside Log File 
int get_recordCount(char* path)
{

    uint32_t count=0;   
    FILE *file;
    struct stat st;
    if (stat(path, &st) == OK_RESULT) 
    {
        file = fopen(path, "r");
        if (file == NULL) 
        { 
            ESP_LOGE("GET_REC_Count", "Failed to open %s",path);
            return FAILED_OPERATION;
        }    
        char line[LOG_LINE_SIZE];         
        do
        {
            fgets(line, sizeof(line), file);
            if (!feof(file))
            {
                count++;
            }                
            else
            {
                break;
            } 
                
        }while(!feof(file));
        fclose(file);       
        }
    else 
    {
        ESP_LOGE("GET_REC_Count", "No Logs");
        return FAILED_OPERATION;
    }
    return count;
}

//save Logs to File 
int write_to_file(char* card,int8_t reader,uint8_t permission,int year,int mon,int day,int hour,int min,int sec)
{
    xSemaphoreTake(xMutex, portMAX_DELAY);
    char log_file[9];
    char path[PATH_SIZE]; 
    bzero(log_file,9);
    memset(path,0,PATH_SIZE*sizeof(char));
    sprintf(log_file,"%d%02d%02d",year,mon,day);
    // char newest[9];
    // bzero(newest,9);
    // if(get_str_key(nvsHandle,NEWEST_FLOG,newest)==0) {ESP_LOGE("WR_LOG","UNABLE TO GET NEWEST FILE"); sprintf(newest,"%d%02d%02d",year,mon,day);}
    // if (strcmp(log_file,newest)<0) sprintf(path,"/spiffs/log%s",newest);
    // else{
        sprintf(path,"/spiffs/log%s",log_file);
        struct stat st; 
        if (stat(path, &st) != OK_RESULT) 
        {
           ESP_LOGE("LOG","File is not exists,Create :%s",log_file);
           f_logCount++;                      
           // set_nvs_int_value(nvsHandle,F_LOG_COUNT,f_logCount); 
           // if(add_str_key(nvsHandle, NEWEST_FLOG,log_file)){ESP_LOGW("WR_LOG","SET NEWSET LOG FILE:%s",log_file);}
        }
    // } 

    FILE *file;
    file = fopen(path, "a");
    if (file == NULL) 
    { 
        ESP_LOGE(TAG_Storage, "Failed to open file for writing");
        return FAILED_OPERATION;
    }
    fprintf(file, "%s;%d;%d;%04d-%02d-%02d;%02d:%02d:%02d\n",card,reader,permission,year,mon,day,hour,min,sec);
    fclose(file);
    recordCount++;
    //set_nvs_int_value(nvsHandle,R_LOG_COUNT,recordCount);
    if (recordCount%50==0) 
    {
        vTaskResume(task_delFlogs);
    }    
    xSemaphoreGive(xMutex); 
    return SUCCESS_OPERATION;
}

// Delete The oldest LOG file 
void del_old_flog()
{
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) 
    {
        ESP_LOGE("Flash", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } 
    else 
    {
        ESP_LOGI("Flash:", "Partition size: total: %d, used: %d", total, used);
    }
    // 
    
    if ((total-used<MAX_ALLOWED_SIZE )||(recordCount>MAX_RECORD_SIZE)||(f_logCount>MAX_LOG_FILE_COUNT))
    {
            int count=0;
            struct dirent *entry;
            char temp[LOG_FILE_NAME_LEN];
            char current[9];

            char oldest_file[LOG_FILE_NAME_LEN];
            bzero(oldest_file,LOG_FILE_NAME_LEN);
            bzero(current,9);
            if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {ESP_LOGE("DEL_OLD_LOG", "Could not get time from DS3231"); return;}
            uint8_t month=rtcinfo.tm_mon;
            sprintf(current,"%d%02d%02d",rtcinfo.tm_year, month ,rtcinfo.tm_mday);


            // if(get_str_key(nvsHandle,NEWEST_FLOG,current)==0) {       
            //     if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {ESP_LOGE("DEL_OLD_LOG", "Could not get time from DS3231"); return;}
            //     uint8_t month=rtcinfo.tm_mon;
            //     sprintf(current,"%d%02d%02d",rtcinfo.tm_year, month ,rtcinfo.tm_mday);
            // }
            sprintf(oldest_file,"log%s",current);
           /// ESP_LOGI("DEL_OLD_LOG","GET NEWSET LOG FILE:%s",oldest_file);
        
            DIR *dir = opendir("/spiffs");   
            if (dir == NULL) 
            {
            ESP_LOGE("DEL_OLD_LOG","UNABLE TO OPEN DIR");
            return;
            }
            // ESP_LOGW("DEL_OLD_LOG","START FETCHING " );

            while ((entry = readdir(dir)) != NULL) 
            {

                if (strncmp(entry->d_name,"log",COMPARE_CHARACHER_LEN)==OK_RESULT)
                {       
                    if (strncmp(entry->d_name,oldest_file,LOG_FILE_NAME_LEN)<0)
                    {
                        bzero(temp,LOG_FILE_NAME_LEN);
                        strncpy(temp,entry->d_name,LOG_FILE_NAME_LEN-1);
                        sprintf(oldest_file,"%s",temp);
                        count++ ;
                    }
            }      
        }
        // ESP_LOGI("DEL_OLD_LOG","COUNT:%d . The oldest file is:%s ",count,oldest_file );
        closedir(dir);
        char path[22];
            memset(path,0,22*sizeof(char));
            sprintf(path,"/spiffs/%s",oldest_file);
            count=0;
            count=get_recordCount(path);
            
            ESP_LOGW("DEL_OLD_LOG","%s contained %d Records",oldest_file,count);

            if (remove(path) != OK_RESULT) 
            {
                ESP_LOGE("DEL_OLD_LOG", "remove failed");
                // unlink(path);
            }
            else
            {
                ESP_LOGE("DEL_OLD_LOG", "%s was removed",oldest_file);
                f_logCount--;
                recordCount-=count;
                // set_nvs_int_value(nvsHandle,F_LOG_COUNT,f_logCount);                         
                //set_nvs_int_value(nvsHandle,R_LOG_COUNT,recordCount);
            }
    }
    else 
    {
        ESP_LOGI("DEL_OLD_LOG:", "No Need to Delete  Free Size:%d KB", (total- used)/1024);
    }

}

void del_flogs (void *arg)
{
    ESP_LOGW("DEL_Flogs_Task","Starting Task DEL_LOGS: %s ! ",pcTaskGetTaskName(NULL));
    while(1)
    {
            ESP_LOGE("DEL_Flogs_Task","Resume! ");
            del_old_flog();  
            vTaskSuspend(NULL);
    }
}

int del_log(char* name)
{

    char path[22];
    memset(path,0,22*sizeof(char));
    sprintf(path,"/spiffs/%s",name);
    struct stat st;
    if (stat(path, &st) != 0) 
    {
        ESP_LOGE("DEL_FLOG","%s File is not exists",name);
        return FILE_NOT_EXISTS;
    }

    int count=0;
    count=get_recordCount(path);

    if (remove(path) != OK_RESULT) 
    {
        ESP_LOGE("DEL_FLOG", "%s Log File Remove failed",name);
        return FAILED_OPERATION;
    }

    f_logCount--;
    recordCount-=count;
    ESP_LOGW("DEL_FLOG", "%s Removed",name);
    return SUCCESS_OPERATION;
}


//Add new user Method 
int add_user(uint32_t userCounter,uint8_t profile,char* name, char* lastname,char * cardId,char* phone, char gender)
{
    FILE *file;
    char path[PATH_SIZE];
    memset(path,0,sizeof(path));
    for(uint8_t i=0;i<strlen(cardId);i++)
    {
        cardId[i] = tolower(cardId[i]);
    }
    sprintf(path,"/spiffs/%s",cardId);
    struct stat st;
    if (stat(path, &st) == OK_RESULT)
    {
        return FILE_NOT_EXISTS;
    }
    file = fopen(path, "w");
    if (file == NULL) 
    { 
        ESP_LOGE(TAG_Storage, "Failed to open file for writing");
        return FAILED_OPERATION;
    }
    fprintf(file,"%08x;%d;%s;%s;%s;%s;%c",userCounter,profile,name,lastname,cardId,phone,gender);
    fclose(file);
    return SUCCESS_OPERATION;
}


//update existing user 
int  update_user(char* id,uint8_t  profile,char* name, char* lastname,char * cardId,char* phone, char gender)
{
    FILE *file;
    char path[PATH_SIZE];
    memset(path,0,PATH_SIZE*sizeof(char));
    for(uint8_t i=0;i<strlen(cardId);i++)
    {
        cardId[i] = tolower(cardId[i]);
    }
        
    sprintf(path,"/spiffs/%s",cardId);
    struct stat st;
    if (stat(path, &st) == 0) 
    {
        ESP_LOGI(TAG_Storage,"File is exist");
        file = fopen(path, "w");
        if (file == NULL) 
        { 
            ESP_LOGE(TAG_Storage, "Failed to open file for writing");
            return -1;
        }
    fprintf(file, "%s;%d;%s;%s;%s;%s;%c",id,profile,name,lastname,cardId,phone,gender);
    fclose(file);

    return SUCCESS_OPERATION;
    }
    else 
    {
        return FAILED_OPERATION; 
    }
}

int del_user(char* cardId)
{
    char path[PATH_SIZE];
    memset(path,0,PATH_SIZE*sizeof(char));
    for(uint8_t i=0;i<strlen(cardId);i++)
    {
        cardId[i] = tolower(cardId[i]);
    }
    sprintf(path,"/spiffs/%s",cardId);
    struct stat st;
    if (stat(path, &st) != 0) 
    {
        ESP_LOGE(TAG_Storage,"File is not exists ");
        return FILE_NOT_EXISTS;
    }
    if (remove(path) != 0) 
    {
        ESP_LOGE(TAG_Storage, "remove failed");
        return FAILED_OPERATION;
    }
    return SUCCESS_OPERATION;
}

int check_file(char* cardId)
{
        FILE *file;
        char path[PATH_SIZE];
       sprintf(path,"/spiffs/%s",cardId);
        file=fopen(path,"r");
        if (file == NULL) 
        {
            return FAILED_OPERATION;
        }
        else 
        {
            fclose(file); 
            return SUCCESS_OPERATION; 
        }
}

void show_logs()
{
   int count=0;
   struct dirent *entry;
   char path[PATH_SIZE];
   char temp[12];

   DIR *dir = opendir("/spiffs");   
   if (dir == NULL) 
   {
      ESP_LOGE("SHOW_LOG","UNABLE TO OPEN DIR");
      return;
   }
   ESP_LOGW("SHOW_LOG","---------------------SPIFFS----------------------------" );
   while ((entry = readdir(dir)) != NULL) {

       if (strncmp(entry->d_name,"log",COMPARE_CHARACHER_LEN)==OK_RESULT)
       {
            bzero(path,PATH_SIZE);
            bzero(temp,12);
            strncpy(temp,entry->d_name,11);
            sprintf(path,"/spiffs/%s",temp);
            ESP_LOGE("SHOW_LOG","%s;",entry->d_name);
            count++ ;
       }        
   }
   ESP_LOGI("SHOW_LOG","LOG FILES COUNT:%d",count );
   closedir(dir);
}

void show_stored_file() 
{
   struct dirent *entry;
   char path[PATH_SIZE];
   char temp[FILE_NAME_SIZE];

   DIR *dir = opendir("/spiffs");   
   if (dir == NULL)
   {
      return;
   }
   ESP_LOGE("SHOW_FILES","---------------------SPIFFS----------------------------" );
   while ((entry = readdir(dir)) != NULL) 
   {
        bzero(path,PATH_SIZE);
        bzero(temp,FILE_NAME_SIZE);
        strncpy(temp,entry->d_name,FILE_NAME_SIZE);
        sprintf(path,"/spiffs/%s",temp);
        read_file(path);
        ESP_LOGE("SHOW_FILES","_______________________________________");
   }
   ESP_LOGE("SHOW_FILES","-------------------------------------------------------" );
   closedir(dir);
}

int clear_logs()
{
   int count=0;
   struct dirent *entry;
   char path[PATH_SIZE];
   char temp[12];

   DIR *dir = opendir("/spiffs");   
   if (dir == NULL) {
      ESP_LOGE("CLEAR_LOG","UNABLE TO OPEN DIR");
      return 0;
    }
   while ((entry = readdir(dir)) != NULL) 
   {

       if (strncmp(entry->d_name,"log",COMPARE_CHARACHER_LEN)==OK_RESULT)
       {
            bzero(path,PATH_SIZE);
            bzero(temp,LOG_FILE_NAME_LEN);
            strncpy(temp,entry->d_name,LOG_FILE_NAME_LEN-1);
            if (del_log(temp))
            {
                count++ ;
            } 
       }        
   }
   ESP_LOGI("CLEAR_LOG","REMOVED LOG FILES COUNT:%d",count );
   closedir(dir);
   return count;
}

int get_users_count()
{
   int count=0;
   struct dirent *entry;
   DIR *dir = opendir("/spiffs");   
   if (dir == NULL) 
   {
      ESP_LOGE("USER_COUNT","UNABLE TO OPEN DIR ");
      return FAILED_OPERATION;
   }
   while ((entry = readdir(dir)) != NULL) 
   {

       if ((strncmp(entry->d_name,"log",COMPARE_CHARACHER_LEN)==OK_RESULT)||((strncmp(entry->d_name,"sch",COMPARE_CHARACHER_LEN)==OK_RESULT )))
       {
           continue;
       } 
        count++ ;              
   }
   ESP_LOGI("USER_COUNT","USER_COUNT:%d",count );
   closedir(dir);
   return count;
}

int get_total_logs(int * filecount)
{

   int count=0;
   int fcount=0;
   struct dirent *entry;
   char path[PATH_SIZE];
   char temp[LOG_FILE_NAME_LEN];

   DIR *dir = opendir("/spiffs");   
   if (dir == NULL) 
   {
      ESP_LOGE("CLEAR_LOG","UNABLE TO OPEN DIR");
      return 0;
    }
   while ((entry = readdir(dir)) != NULL) 
   {

       if (strncmp(entry->d_name,"log",COMPARE_CHARACHER_LEN)==OK_RESULT)
       {
            bzero(path,PATH_SIZE);
            bzero(temp,LOG_FILE_NAME_LEN);
            fcount++;
            strncpy(temp,entry->d_name,LOG_FILE_NAME_LEN-1);
            sprintf(path,"/spiffs/%s",temp);
            count+=get_recordCount(path) ;
       }        
   }
   ESP_LOGI("TOTAL LOGS","Total LOGS :%d",count );
   closedir(dir);
   *filecount=fcount;
   return count;
}

// Read Content of File 
void upload_profile(role_t* schedule,uint8_t id)
{

   FILE *file;
   char val_1[3];
   int num=0;

   bzero(val_1,3);
   if(id==SHEDULE_NO_0)  
   {
       file = fopen("/spiffs/sched_0", "r");
   }
   else if(id==SHEDULE_NO_1)  
   {
       file = fopen("/spiffs/sched_1", "r");
   }
   else if(id==SHEDULE_NO_2) 
   {
       file = fopen("/spiffs/sched_2", "r");
   }
   else
   {
       ESP_LOGE("Schedule","Schedule number is wrong !!!!");
       return ;
   }

   if (file == NULL) 
   { 
       ESP_LOGE(TAG_Storage, "Failed to open file for writing");
        return ;
   }

    char line[LINE_SIZE];
    do
    {                          
            bzero(line,LINE_SIZE);
            fgets(line, sizeof(line), file);

            char val_3[2];
            sprintf(val_3,"%c",line[0]);
            num=atoi(val_3);

            schedule[num-1].id=num;

            sprintf(val_1,"0%c",line[2]);
            schedule[num-1].status=atoi(val_1);      

            sprintf(val_1,"%c%c",line[4],line[5]);
            schedule[num-1].str_time.st_hour=atoi(val_1);

            sprintf(val_1,"%c%c",line[7],line[8]);
            schedule[num-1].str_time.st_min=atoi(val_1);

            sprintf(val_1,"%c%c",line[13],line[14]);
            schedule[num-1].end_time.st_hour=atoi(val_1);

            sprintf(val_1,"%c%c",line[16],line[17]);
            schedule[num-1].end_time.st_min=atoi(val_1);
            if((num==7)) break;

    }while(!feof(file));
    fclose(file);            
}


bool is_exists(char* cardID)
{
    FILE *file;
    char path[PATH_SIZE];
    bzero(path,PATH_SIZE);
    uint8_t _state=0;
    sprintf(path,"/spiffs/%s",cardID);
    struct stat st; 
    if (stat(path, &st) == OK_RESULT) 
    {
           file = fopen(path, "r");
            if (file == NULL) 
            { 
                ESP_LOGE(TAG_Storage, "Failed to open file for writing"); 
                return DENIED;
            }
            char line[LINE_SIZE];
            fgets(line, sizeof(line), file);
            fclose(file);
            char val[3];
            bzero(val,3);
            sprintf(val,"%c",line[9]);

            uint8_t profile=atoi(val);
            if(profile==ALLOWED_PROFILE) 
            {
                _state=ALLOWED;
            }
            else if (profile ==DENIED_PROFILE) 
            {
                _state=DENIED;
            }
            else if (profile ==SCHEDULE_1_PROFILE)
            {
                if(rtcinfo.tm_wday>7) 
                {
                    return DENIED;
                }
                if(schedule_0[rtcinfo.tm_wday-1].status==DENIED_PROFILE) 
                { 
                    return DENIED;
                }     
                else if (schedule_0[rtcinfo.tm_wday-1].status==ALLOWED_PROFILE) 
                {
                    return ALLOWED;
                } 
                else if (schedule_0[rtcinfo.tm_wday-1].status==CUSTOM_ALLOWED)
                {
                    if((rtcinfo.tm_hour<schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)||(rtcinfo.tm_hour>schedule_0[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    {
                        return DENIED;
                    }
                    if((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min <schedule_0[rtcinfo.tm_wday-1].str_time.st_min )||(rtcinfo.tm_min >schedule_0[rtcinfo.tm_wday-1].end_time.st_min)) 
                        { 
                            return DENIED;
                        }
                    }
                    else
                    {
                        if ((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min <schedule_0[rtcinfo.tm_wday-1].str_time.st_min )) 
                        {
                            return DENIED;
                        }
                        if ((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min>schedule_0[rtcinfo.tm_wday-1].end_time.st_min )) 
                        {
                            return DENIED;
                        }
                    }
                    return 1;
                } 
                else if (schedule_0[rtcinfo.tm_wday-1].status==CUSTOM_DENIED) 
                {
                    if((rtcinfo.tm_hour>schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour<schedule_0[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    {
                        return DENIED;
                    }
                    
                    if ((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min >=schedule_0[rtcinfo.tm_wday-1].str_time.st_min )&&(rtcinfo.tm_min <=schedule_0[rtcinfo.tm_wday-1].end_time.st_min)) 
                        {
                            return DENIED;
                        }
                    }
                    else
                    {
                        if ((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min >=schedule_0[rtcinfo.tm_wday-1].str_time.st_min )) 
                        {
                            return DENIED;
                        }
                        if ((rtcinfo.tm_hour==schedule_0[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min <=schedule_0[rtcinfo.tm_wday-1].end_time.st_min )) 
                        {
                            return DENIED;
                        }
                    }
                    return 1;} 
            } 
            else if (profile ==SCHEDULE_2_PROFILE)
            {
                if(rtcinfo.tm_wday>7) 
                {
                    return DENIED;
                }
                if(schedule_1[rtcinfo.tm_wday-1].status==DENIED_PROFILE) 
                { 
                    return DENIED;
                }     
                else if (schedule_1[rtcinfo.tm_wday-1].status==ALLOWED_PROFILE) 
                {
                    return ALLOWED;
                } 
                else if (schedule_1[rtcinfo.tm_wday-1].status==CUSTOM_ALLOWED)
                {

                    if((rtcinfo.tm_hour<schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)||(rtcinfo.tm_hour>schedule_1[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    {
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min <schedule_1[rtcinfo.tm_wday-1].str_time.st_min )||(rtcinfo.tm_min > schedule_1[rtcinfo.tm_wday-1].end_time.st_min)) 
                        {
                            return DENIED;
                        }
                    }
                    else
                    {
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min <schedule_1[rtcinfo.tm_wday-1].str_time.st_min )) 
                    {
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min >schedule_1[rtcinfo.tm_wday-1].end_time.st_min )) 
                    {
                        return DENIED;
                    }
                     }
                    return ALLOWED;} 
                else if (schedule_1[rtcinfo.tm_wday-1].status==CUSTOM_DENIED) 
                {
                    if((rtcinfo.tm_hour>schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour<schedule_1[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    {
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min >=schedule_1[rtcinfo.tm_wday-1].str_time.st_min )&&(rtcinfo.tm_min <=schedule_1[rtcinfo.tm_wday-1].end_time.st_min)) 
                        {
                            return DENIED;
                        }
                    }
                    else{
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min >=schedule_1[rtcinfo.tm_wday-1].str_time.st_min )) 
                    {
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_1[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min <=schedule_1[rtcinfo.tm_wday-1].end_time.st_min )) 
                    {
                        return DENIED;
                    }
                    }
                    return ALLOWED;} 

            } 
            else if (profile ==SCHEDULE_3_PROFILE)
            {
                if(rtcinfo.tm_wday>7) 
                {
                    return DENIED;
                }
                if(schedule_2[rtcinfo.tm_wday-1].status==DENIED_PROFILE) 
                { 
                    return DENIED;
                }     
                else if (schedule_2[rtcinfo.tm_wday-1].status==ALLOWED_PROFILE) 
                {
                    return ALLOWED;
                } 
                else if (schedule_2[rtcinfo.tm_wday-1].status==CUSTOM_ALLOWED)
                {

                    if((rtcinfo.tm_hour<schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)||(rtcinfo.tm_hour>schedule_2[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    {
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min <schedule_2[rtcinfo.tm_wday-1].str_time.st_min )||(rtcinfo.tm_min > schedule_2[rtcinfo.tm_wday-1].end_time.st_min)) 
                        {
                            return DENIED;
                        }
                    }
                    else
                    {                   
                        if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min <schedule_2[rtcinfo.tm_wday-1].str_time.st_min )) 
                        { 
                            return DENIED;
                        }
                        if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min >schedule_2[rtcinfo.tm_wday-1].end_time.st_min )) 
                        {
                            return DENIED;
                        }
                    }
                    return ALLOWED;
                } 
                else if(schedule_2[rtcinfo.tm_wday-1].status==CUSTOM_DENIED) 
                {
                    if((rtcinfo.tm_hour>schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour<schedule_2[rtcinfo.tm_wday-1].end_time.st_hour)) 
                    { 
                        return DENIED;
                    }
                    if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].end_time.st_hour))
                    {
                        if ((rtcinfo.tm_min >=schedule_2[rtcinfo.tm_wday-1].str_time.st_min )&&(rtcinfo.tm_min <=schedule_2[rtcinfo.tm_wday-1].end_time.st_min)) 
                        {
                            return DENIED;
                        }
                    }
                    else
                    {
                        if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].str_time.st_hour)&&(rtcinfo.tm_min >=schedule_2[rtcinfo.tm_wday-1].str_time.st_min )) 
                        {
                            return DENIED;
                        }
                        if ((rtcinfo.tm_hour==schedule_2[rtcinfo.tm_wday-1].end_time.st_hour)&& (rtcinfo.tm_min <=schedule_2[rtcinfo.tm_wday-1].end_time.st_min )) 
                        {
                            return DENIED;
                        }
                    }                    
                    return ALLOWED;
                }
            } 
   }
    return _state;    
 }



int writeDeviceLog(int type)
{
# ifdef LOG_DBG     
    FILE *file;
    file = fopen("/spiffs/sch_id", "a");
    if (file == NULL) 
    { 
        ESP_LOGE(TAG_Storage, "Failed to open file"); 
        return FAILED_OPERATION;
    }
    if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {ESP_LOGE("DS3231", "Could not get time.");}
    if(type==1)
        fprintf(file, "MST[%04d-%02d-%02d;%02d:%02d:%02d]\n",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);                       // ESP323 MAIN START SUCCESS 
    else if (type==2)
        fprintf(file, "WFC[%04d-%02d-%02d;%02d:%02d:%02d]\n",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);                       //WIFI STATION MODE CONNECT 
    else if (type==3)
        fprintf(file, "WFD[%04d-%02d-%02d;%02d:%02d:%02d]\n",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);                       // WIFI STATION MODE DISCONNECT
    else if (type==4)
        fprintf(file, "WFA[%04d-%02d-%02d;%02d:%02d:%02d]\n",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);                        // WIFI ACCESS POINT MODE 
    else if (type==5)
        fprintf(file, "MSR[%04d-%02d-%02d;%02d:%02d:%02d]\n",rtcinfo.tm_year, rtcinfo.tm_mon,rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec);                        //ESP323 MAIN STORAGE AND FLASH SUCCESS  
    fclose(file);
# endif
    return SUCCESS_OPERATION;
}
