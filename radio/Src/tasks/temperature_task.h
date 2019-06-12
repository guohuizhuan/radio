#ifndef  __TEMPERATURE_TASK_H__
#define  __TEMPERATURE_TASK_H__


#define  TEMPERATURE_TASK_MSG_WAIT_TIMEOUT    osWaitForever
#define  TEMPERATURE_TASK_PUT_MSG_TIMEOUT     5


extern osThreadId temperature_task_hdl;

/*
* @brief 
* @param
* @param
* @return 
* @note
*/

void temperature_task(void const * argument);










#endif