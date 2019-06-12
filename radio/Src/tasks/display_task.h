#ifndef  __DISPLAY_TASK_H__
#define  __DISPLAY_TASK_H__
#include "stdint.h"
#include "cmsis_os.h"


#ifdef   __cplusplus
#define  DISPLAY_TASK_BEGIN        extern "C" {
#define  DISPLAY_TASK_END          }
#else
#define  DISPLAY_TASK_BEGIN      
#define  DISPLAY_TASK_END       
#endif


DISPLAY_TASK_BEGIN   


/*显示任务句柄*/
extern osThreadId display_task_hdl;
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void display_task(void const * argument);


#define  DISPLAY_TASK_MSG_WAIT_TIMEOUT       osWaitForever
#define  FM_FREQ_DISPLAY_MSG                 1
#define  SIGNAL_LEVEL_DISPLAY_MSG            2
#define  TEMPERATURE_DISPLAY_MSG             3


/*显示配置*/
#define  DISPLAY_TEMPERATURE_Y               0
#define  DISPLAY_TEMPERATURE_HEAD_X          0
#define  DISPLAY_TEMPERATURE_HEAD_STR        "t:"
#define  DISPLAY_TEMPERATURE_VALUE_X         2
#define  DISPLAY_TEMPERATURE_TAIL_X          5 
#define  DISPLAY_TEMPERATURE_TAIL_STR        "C"

#define  DISPLAY_LEVEL_Y                     0
#define  DISPLAY_LEVEL_HEAD_X                7
#define  DISPLAY_LEVEL_HEAD_STR              "level:"
#define  DISPLAY_LEVEL_VALUE_X               13


#define  DISPLAY_FM_FREQ_Y                   1 
#define  DISPLAY_FM_FREQ_HEAD_X              2
#define  DISPLAY_FM_FREQ_HEAD_STR            "FM"
#define  DISPLAY_FM_FREQ_VALUE_X             5 
#define  DISPLAY_FM_FREQ_TAIL_X              11
#define  DISPLAY_FM_FREQ_TAIL_STR            "MHz"



DISPLAY_TASK_END


#endif