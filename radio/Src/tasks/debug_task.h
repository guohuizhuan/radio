#ifndef  __DEBUG_TASK_H__
#define  __DEBUG_TASK_H__

#ifdef  __cplusplus
extern  "C" {
#endif



/*任务句柄*/
extern osThreadId debug_task_hdl;
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void debug_task(void const * argument);



#ifdef  __cplusplus
}
#endif


#endif