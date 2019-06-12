#ifndef  __TASKS_INIT_H__
#define  __TASKS_INIT_H__
#include "stdint.h"
#include "cmsis_os.h"

#ifdef   __cplusplus
#define  TASKS_INIT_BEGIN        extern "C" {
#define  TASKS_INIT_END          }
#else
#define  TASKS_INIT_BEGIN      
#define  TASKS_INIT_END       
#endif


TASKS_INIT_BEGIN 

extern QueueHandle_t radio_msg_q_hdl;
extern QueueHandle_t temperature_msg_q_hdl;
extern QueueHandle_t display_msg_q_hdl;

typedef struct
{
    uint8_t id;
    union {
        struct {
            uint8_t is_err;
            uint16_t value;
        }adc;
    };
}temperature_message_t;

typedef struct
{
    uint8_t id;
    union {
        struct {
            uint8_t is_err;
            uint16_t value;
        }adc;
    };
}radio_message_t;

typedef struct
{
    uint8_t id;
    uint8_t is_err;
    union {
        float fm_freq;
        float temperature;
        uint8_t signal_level;
    };
}display_message_t;

#define  MSG_ID_ADC_COMPLETED                1

#define  TEMPERATURE_MESSAGE_QUEUE_SIZE      4
#define  RADIO_MESSAGE_QUEUE_SIZE            4
#define  DISPLAY_MESSAGE_QUEUE_SIZE          4

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tasks_init();

TASKS_INIT_END

#endif