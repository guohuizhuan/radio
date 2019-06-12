#ifndef  __RADIO_H__
#define  __RADIO_H__

#ifdef   __cplusplus
#define  RADIO_BEGIN        extern "C" {
#define  RADIO_END          }
#else
#define  RADIO_BEGIN      
#define  RADIO_END       
#endif



RADIO_BEGIN

extern osThreadId radio_task_hdl;


/*
* @brief 
* @param
* @param
* @return 
* @note
*/

void radio_task(void const * argument);





RADIO_END


#endif