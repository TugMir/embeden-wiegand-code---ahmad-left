#include "HF_TCPsocket.h"

static uint64_t previousTime=0;
 
int espx_last_socket_errno(int socket)
{
    fd_set readset;
    fd_set writeset;
    fd_set errset;
    struct timeval tv;
    int retval;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&errset);
	tv.tv_sec = 1;
	tv.tv_usec = 100;
	retval = select (socket + 1, &readset, &writeset, &errset, &tv);

    ESP_LOGE("TCPX","RETVAL:%d",retval);
    if (FD_ISSET(socket, &readset))  ESP_LOGE("TCPX","readset:%d",retval);
    if (FD_ISSET(socket, &writeset))  ESP_LOGE("TCPX","writeset:%d",retval);
    if (FD_ISSET(socket, &errset))  ESP_LOGE("TCPX","errset:%d",retval);

    return retval;
}

void setKeepalive(int listenfd )
{

	int keepAlive    =KEEPALIVE_MS;         /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
	int keepIdle     =KEEPIDLE_SEC;             /* The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes, */
	int keepInterval =KEEPINTERVAL_SEC;         /* set pcb->keep_intvl - Use seconds for get/setsockopt */
	int keepCount    =KEEPCOUNT_PROB;           /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */

	setsockopt (listenfd, SOL_SOCKET, SO_KEEPALIVE,&keepAlive, sizeof (int));
	setsockopt (listenfd, IPPROTO_TCP, TCP_KEEPIDLE,&keepIdle, sizeof (int));
	setsockopt (listenfd, IPPROTO_TCP, TCP_KEEPINTVL,&keepInterval, sizeof (int));
	setsockopt (listenfd, IPPROTO_TCP, TCP_KEEPCNT,&keepCount, sizeof (int));
}

void tcp_server_task(void *pvParameters)
{
    ESP_LOGW(TAG_TCP,"Starting Task : %s",pcTaskGetTaskName(NULL));
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    // IPv4 address configurations 
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        printf( "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    //printf("Socket created \n");
    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf("Socket unable to bind: errno %d ", errno);
        goto CLEAN_UP;
    }
    err = listen(listen_sock, 1);
    if (err != 0) {
        printf( "Error occurred during listen: errno %d ", errno);
        goto CLEAN_UP;
    }

    while (1) 
    {
        ESP_LOGI(TAG_TCP,"Socket listening on port:%d  ",port);
        isConnected=false;                                             // used in read_card  to check connection State  
        struct sockaddr_in6 source_addr;                           // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0)
        {
            ESP_LOGE(TAG_TCP,"Unable to accept connection: errno %d  ", errno);
            close(sock);
            continue;
        }
        // Convert ip address to string
        if (source_addr.sin6_family == PF_INET) 
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
        } 
        else if (source_addr.sin6_family == PF_INET6)
        {
            inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
        ESP_LOGW(TAG_TCP,"Socket accepted ip address: %s ", addr_str);

        err =_send(sock, Firmware);
        if (err < 0) 
        {
            ESP_LOGE(TAG_TCP,"Error occurred during sendin, Unable to send Firmware Version   : errno %d ", errno);
        }
        else
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            char HWver [23];
            bzero(HWver,23);
            sprintf(HWver,"SN|%s",MAC);
            err =_send(sock, HWver);
            if (err < 0) ESP_LOGE(TAG_TCP,"Error occurred during sendin, Unable to send MAC Address   : errno %d", errno);
        }
        
        if(in1State) 
        {
            err = _send(sock, (const char*) INPUT_1_ON);
        } 
        else          
        {
            err = _send(sock, (const char*) INPUT_1_OFF);
        } 
        vTaskDelay(500 / portTICK_PERIOD_MS);
        if (isStandalone==1)
        {
            err = _send(sock, "MODE|Sandalone");
            if (err < 0) 
            {
                ESP_LOGE(TAG_TCP,"Error occurred during sendin, Unable to send MAC Address   : errno %d ", errno);
            }
        }
        else 
        {
            err = _send(sock, "MODE|Online");
            if (err < 0) 
            {
                ESP_LOGE(TAG_TCP,"Error occurred during sendin, Unable to send MAC Address   : errno %d ", errno);
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);

        Clear_receive(sock);
        setKeepalive(sock );
        ESP_LOGI(TAG_TCP,"setKeepalive");

        fcntl(sock, F_SETFL, O_NONBLOCK) ;
        int pass;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &pass, sizeof(pass));
        previousTime=xTaskGetTickCount(); 
        int8_t rxState=0;
        err = heartBeat(sock);
        ESP_LOGE(TAG_TCP,"Sent(%d)",err);

        while (1) 
        {
           isConnected=true;
           // Receiving Data Via TCP 
           rxState=receive_cmd(sock);
           if(rxState==-1) 
           {
               ESP_LOGE("Receive_Cmd","Disconnected");
               isConnected= false; 
               break;
            } 
           else if 
           (rxState==1) 
           {
               previousTime=xTaskGetTickCount();
            }  

            // Sending Data Via TCP 
            if(xSemaphoreTake( xSemaphoreWebsocket_R1, (TickType_t) 10 ) == pdTRUE) 
            { 
                char Txbuff[Data_Size];    
                if(xQueueReceive(HFcardQueue, &(Txbuff) , (TickType_t)30 )){
                  // ESP_LOGW(TAG_TCP,"Got a data from HFcardQueue - payload: %s \n",Txbuff);  // send data 
                    err=send_data(sock,Txbuff);
                    if (err>=0){
                        R1_state=true; 
                        previousTime=xTaskGetTickCount();
                       // if(espx_last_socket_errno(sock)==-1) break;
                        xEventGroupSetBits(R1_connection_flag,CONNECTED_BIT_R1); 
                        // ESP_LOGI(TAG_TCP,"CardID has sent, Waiting Reply from Server......err:%d",err);
                        }
                    else {
                        isConnected= false;
                        R1_state=false;
                        xEventGroupSetBits(R1_connection_flag,CONNECTED_BIT_R1); 
                        ESP_LOGE(TAG_TCP,"CardID 1 has Not sent err:%d ",err);
                        break;
                        }                      
                    }
            }
                     
            if(xSemaphoreTake( xSemaphoreWebsocket_R2, (TickType_t) 10 ) == pdTRUE) 
            { 
                char Txbuff[Data_Size];    
                if(xQueueReceive(HFcardQueue, &(Txbuff) , (TickType_t)30 )){
                  // ESP_LOGW(TAG_TCP,"Got a data from HFcardQueue - payload: %s \n",Txbuff);  // send data 
                    err=send_data(sock,Txbuff);
                    if (err>=0){
                       R2_state=true; 
                       previousTime=xTaskGetTickCount();
                       // if(espx_last_socket_errno(sock)==-1) break;
                        xEventGroupSetBits(R2_connection_flag,CONNECTED_BIT_R2); 
                        // ESP_LOGI(TAG_TCP,"CardID has sent, Waiting Reply from Server...... err:%d",err);
                        }
                    else {
                        isConnected= false;
                        R2_state=false;
                        xEventGroupSetBits(R2_connection_flag,CONNECTED_BIT_R2); 
                        ESP_LOGE(TAG_TCP,"CardID 2 has Not sent err:%d",err);
                        break;
                        }
                }
            }

            if(xSemaphoreTake( xSemaphoreInput_1_ON, (TickType_t) 10 ) == pdTRUE) 
            {
                    // if(in1State) {err = _send(sock, (const char*) INPUT_1_ON);} 
                    // else          {err = _send(sock, (const char*) INPUT_1_OFF);} 
                    err = _send(sock, (const char*) INPUT_1_ON);
                    if (err<0)
                    {
                        ESP_LOGI(TAG_TCP,"Faild to send Input 1_1 .. err:%d",err); 
                        break;
                    }
                    previousTime=xTaskGetTickCount();
            }
            if(xSemaphoreTake( xSemaphoreInput_1_OFF, (TickType_t) 10 ) == pdTRUE) 
            {
                    // if(in1State) {err = _send(sock, (const char*) INPUT_1_ON);} 
                    // else          {err = _send(sock, (const char*) INPUT_1_OFF);} 
                    err = _send(sock, (const char*) INPUT_1_OFF);
                    if (err<0)
                    {
                        ESP_LOGI(TAG_TCP,"Faild to send Input 1_0 .. err:%d",err); 
                        break;
                    }
                    previousTime=xTaskGetTickCount();
            }
            /*
           if(xSemaphoreTake( xSemaphoreInput_2, (TickType_t) 10 ) == pdTRUE) { 
                    if(in2State) {err = _send(sock, (const char*) INPUT_2_ON);} 
                    else          {err = _send(sock, (const char*) INPUT_2_OFF);} 
                    if (err<0){ESP_LOGI(TAG_TCP,"Faild to send Input 2 .. err:%d",err);}
            }
            */  
           // Heart Beat Functions
           if((xTaskGetTickCount()-previousTime)>=500){
               err = heartBeat(sock);
               if (err<0){
                        isConnected= false;
                        ESP_LOGE(TAG_TCP,"Connection Lost(%d)",err);
                        break;
                   }
                // else {ESP_LOGE(TAG_TCP,"Sent(%d)",err);}
               previousTime=xTaskGetTickCount();
           }
        // vTaskDelay(50 / portTICK_PERIOD_MS);
        }
        R2_state=false;
        R1_state=false;
        xEventGroupSetBits(R1_connection_flag,CONNECTED_BIT_R1);
        xEventGroupSetBits(R2_connection_flag,CONNECTED_BIT_R2);
        ESP_LOGE(TAG_TCP,"Shutting down socket and restarting... ");                                              // Free all resources allocated befor for server  
        shutdown(sock, 0);
        close(sock);
    }
  
CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
    esp_restart();
    return;
}

int receive_cmd(int sock)
{
    int8_t state=0;
    static char cmd_buffer[RX_MSG_LEN];
    memset(cmd_buffer,0, RX_MSG_LEN*sizeof(cmd_buffer[0]));
    int len = recv(sock, cmd_buffer, sizeof(cmd_buffer), MSG_DONTWAIT);
    if (len < 0) 
    {
        if (errno == 128 || errno == 104 || errno == 113) 
        {
            ESP_LOGE(TAG_TCP,"Connection closed %d",errno);                     //When TCP connection is lost go to TCP Listen Stage                                                                                                                                  
            return -1;
        }
    }
    else 
    {        
      state=1;
      uint8_t bloop=0;
      ESP_LOGI(TAG_TCP,"RX(%d):%s",errno,cmd_buffer );
      char * token = strtok(cmd_buffer, "\n");
      while( token != NULL ) 
      {
        // printf("TOK(%d):%s\n",bloop,token);
        bloop++;
        if(strcmp(token,Cmd_Open_R1)==0)
            OpenDoor_R1();            
        else if(strcmp(token,Cmd_Open_R2)==0)
            OpenDoor_R2();
        else if( strcmp(token,Cmd_Close_R1)==0)
            R1_CloseDoor();
        else if( strcmp(token,Cmd_Close_R2)==0)
            R2_CloseDoor();
        else if( strncmp(token, Cmd_Open_Delay_R2, 7)==0 )
        {
                bool isnumber=true;
                if (strlen(token)>12 ||strlen(token)<10)
                {
                    ESP_LOGI(TAG_TCP,"Invalid Time : Time between 1 to 99999 ms \n");
                }
                else
                {
                    int period =0;
                    char delay_val[12];
                    memset(delay_val,0, 12*sizeof(delay_val[0]));
                    for(int j=0 ; j<strlen(token)-7;j++)
                    {
                            if ((isdigit(token[j+7]) > 0)&&(isalnum(token[j+7]) != 0)) 
                            { delay_val[j]=token[j+7];}                      
                            else if (token[j+7]=='\n') break;
                            else
                            {
                                isnumber=false; break;
                            }
                    }
                    if(isnumber)
                    {
                        period=atoi(delay_val);
                        if(period<1000) 
                        {
                            ESP_LOGI(TAG_TCP,"Invalid delay time %d \n",period); goto end;
                        }                     
                        if(xTimerChangePeriod(tmr_R2_Relay,  period/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)
                        {
                            ESP_LOGE(TAG_TCP,"Open delay time R2_Relay %d \n",atoi(delay_val));
                        } 
                        else
                        {
                            ESP_LOGW(TAG_TCP,"Unable to change Open Time, Used defualt Value!!! \n");
                        }
                        R2_OpenDoor();
                        xTimerReset(tmr_R2_Relay,( TickType_t) 10);
                }
                else printf("RElay-2 Delay value is invaled :%s \n",delay_val);                    
                }
            }
        else if( strncmp(token, Cmd_Open_Delay_R1, 7)==0 )
        {       
                bool isnumber=true;
                if (strlen(token)>12 ||strlen(token)<10)
                         printf("Invalid Time : Time between 1 to 99999 ms \n");
                else{
                        int period =0;
                        char delay_val[12];
                        memset(delay_val, 0, 12*sizeof(delay_val[0]));
                        for(int j=0 ; j<strlen(token)-7;j++){
                            if ((isdigit(token[j+7]) > 0)&&(isalnum(token[j+7]) != 0)) { delay_val[j]=token[j+7];}                      
                            else if (token[j+7]=='\n') break;
                            else {isnumber=false; break;}
                    }
                
                if(isnumber){
                        period=atoi(delay_val);  
                        if(period<1000) {printf("Invalid delay time %d \n",period); goto end;}                  
                        if(xTimerChangePeriod(tmr_R1_Relay,  period/ portTICK_PERIOD_MS, (TickType_t) 10 )==pdPASS)
                              printf("Open delay time %d \n",period);
                        else{printf("Unable to change Open Time, Used defualt Value!!! \n");}
                        R1_OpenDoor();
                        xTimerReset(tmr_R1_Relay,( TickType_t) 10);
                }
                else printf("Delay value is invaled :%s \n",delay_val);
                }
            }
        else if(strcmp(token,SEND_LOG)==0){
            //if(isStandalone) return 0;
            char path[32];
            bzero(path,32);
            unsigned int count=0;   
            int err;
            struct dirent *entry;
            char name[12];
            DIR *dir = opendir("/spiffs");   
            if (dir == NULL) {
                    ESP_LOGE("TCP_LOG","UNABLE TO OPEN DIR");
                    return 0 ;
                }
            ESP_LOGW("HTTP_LOG","START FETCHING " );
            err= _send(sock,START_SEND);
            while ((entry = readdir(dir)) != NULL) {
                    if (strncmp(entry->d_name,"log",3)==0){ 
                        strncpy(name,entry->d_name,12);
                        sprintf(path,"/spiffs/%s",name);
                        printf("path %s\n",path);
                        err = send_file(sock ,path);
                        ESP_LOGW("HTTP_LOG","Finish Sending file" );                        
                        if (err < 0) {
                            if (err==-1) ESP_LOGW("HTTP_LOG","No Logs To Send" ); 
                               // err= send_data(sock,"No Logs To Send ");
                            else
                                ESP_LOGW("HTTP_LOG","error" );
                               // err= send_data(sock,"error  ");
                        }
                        else{ 
                            ESP_LOGW("HTTP_LOG","Send File Done " );
                            // if(del_log(name)) ESP_LOGE(TAG_TCP, "%s file Removed",name); 
                            // else ESP_LOGE(TAG_TCP, "Failed to Remove Log file");
                            count++;
                        }                       
                    }
                }   
            closedir (dir);

            err=_send(sock,FINISH_SEND);         
            ESP_LOGW("HTTP_LOG","Finish Sending ,  File Count:%d      err:%d",count,err);
                
        }
        else if(strcmp(token,REMOVE_LOG)==0){

                char path[32];
                bzero(path,32);
                unsigned int count=0;   

                struct dirent *entry;
                char name[12];

                DIR *dir = opendir("/spiffs");   
                if (dir == NULL) {
                    ESP_LOGE("TCP_LOG","UNABLE TO OPEN DIR");
                    return 0 ;
                }
                ESP_LOGW("HTTP_LOG","START Deleting " );
        
                while ((entry = readdir(dir)) != NULL) {
                    
                    if (strncmp(entry->d_name,"log",3)==0){ 
                        strncpy(name,entry->d_name,12);
                        sprintf(path,"/spiffs/%s",name);
                        printf("path %s\n",path);
                        if(del_log(name)) ESP_LOGE(TAG_TCP, "%s file Removed",name); 
                        else ESP_LOGE(TAG_TCP, "Failed to Remove Log file");
                        count++;
                    }
                }
                closedir (dir);              
            printf("Count %d\n",count);  
        }
        else ESP_LOGE(TAG_TCP, "Invalid CMD ");
        token = strtok(NULL, "\n");        
        if(bloop==5) {printf("Break LOOP \n"); break;}                 
    }
   }
    goto end;
    end:
    return state;
}

void Clear_receive(int sock)
{
    char cmd_buffer[RX_MSG_LEN];
    memset(cmd_buffer,0, RX_MSG_LEN*sizeof(cmd_buffer[0]));
    int len = recv(sock, cmd_buffer, sizeof(cmd_buffer) - 1, MSG_DONTWAIT);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    if(len>0)
        len = recv(sock, cmd_buffer, sizeof(cmd_buffer) - 1, MSG_DONTWAIT);
    printf("Receive buffer is clear - receive length: %d \n",len);
    
}
      
int send_data(int sock,char* tx_buffer)
{
    int err = send(sock, tx_buffer, strlen(tx_buffer)+1, (int) NON_BLOCK); 
    if (err < 0)
    {
            ESP_LOGE(TAG_TCP,"Error occurred during sending, Connection lost  : errno %d", errno);
    }
    return err;
}

int send_file(int sock , char* path)
{

    int err;
    FILE *file;
    file = fopen(path, "r");
    if (file == NULL) 
    { 
        ESP_LOGE(TAG_TCP, "Failed to open file for writing");
        return ERROR_OPEN_FILE;
    }
    ESP_LOGW(TAG_TCP, "Start sending file %s",path);
    if(feof(file))
    {
        ESP_LOGE(TAG_TCP,"Empty File "); 
        return ERROR_EMPTY_FILE; 
    }   

    char line[LINE_LENGTH];
    char val[VALUE_LENGTH];
    int count=0;
    bzero(line,LINE_LENGTH);
    bzero(val,VALUE_LENGTH);
    do
    {
        fgets(line, sizeof(line), file);
        if(feof(file)) 
        {
            break;
        }
        sprintf(val,"L|%s\n",line);
        err = send(sock, (void *) val, strlen(val), (int) NON_BLOCK); 
        bzero(line,LINE_LENGTH);
        bzero(val,VALUE_LENGTH);
        if (err < 0) 
        { 
            ESP_LOGE(TAG_TCP,"Error occurred during sending, Connection lost  : errno %d", errno); 
            goto end;
        }
        count++;

    } while(!feof(file));

    ESP_LOGW(TAG_TCP,"Total Count:%d \n",count);
    err=count;      
    goto end;
    end:
    fclose(file);       
    return err;
} 

int _send(int s,const char *dataptr)
{
    int err;
    char buffer[TX_MSG_LEN];
    bzero(buffer,TX_MSG_LEN);
    sprintf(buffer,"%s\n",dataptr);
    err = send(s, buffer, strlen(buffer), (int) NON_BLOCK);
    return err;
}

int heartBeat(int s)
{
    char buffer[TX_MSG_LEN];
    bzero(buffer,TX_MSG_LEN);
    if(gettimeofday(&tv,NULL)==OK_RESULT) 
    {
        sprintf(buffer,"HEART|%ld\n",tv.tv_sec);
    }
    else 
    {
        sprintf(buffer,"HEART|000000\n");
    }
    return  send(s, buffer, strlen(buffer),(int) NON_BLOCK);
}




