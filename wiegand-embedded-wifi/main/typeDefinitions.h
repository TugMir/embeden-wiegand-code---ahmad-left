#ifndef  _TYPEDEFINITIONS__H__
#define _TYPEDEFINITIONS__H__

#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef char card_t[10];

typedef  struct ap_list
{
    char ssid[33];
    int8_t rssi;
}ap_list_t;

typedef struct _time
{
   // uint8_t st_sec;   
    uint8_t st_min;   
    uint8_t st_hour;  

} _time_t;


typedef struct role
{
    uint8_t id;   
    uint8_t status;   
    _time_t str_time;  
    _time_t end_time;  
} role_t;


typedef struct {
    i2c_port_t port;            // I2C port number
    uint8_t addr;               // I2C address
    gpio_num_t sda_io_num;      // GPIO number for I2C sda signal
    gpio_num_t scl_io_num;      // GPIO number for I2C scl signal
    uint32_t clk_speed;             // I2C clock frequency for master mode
} i2c_dev_t;



#ifdef __cplusplus
}
#endif

#endif 