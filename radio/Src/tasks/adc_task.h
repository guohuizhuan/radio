#ifndef  __ADC_TASK_H__
#define  __ADC_TASK_H__
#include "stdint.h"
#include "cmsis_os.h"


#ifdef   __cplusplus
#define  ADC_TASK_BEGIN        extern "C" {
#define  ADC_TASK_END          }
#else
#define  ADC_TASK_BEGIN      
#define  ADC_TASK_END       
#endif


ADC_TASK_BEGIN   


/*adc任务句柄*/
extern osThreadId adc_task_hdl;









ADC_TASK_END


#endif