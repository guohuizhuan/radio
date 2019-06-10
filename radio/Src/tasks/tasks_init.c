#include "tasks_init.h"
#include "radio_task.h"
#include "adc_task.h"
#include "debug_task.h"
#include "st_cm3_uart_hal_driver.h"
#include "xuart.h"
#include "log.h"

/*消息队列句柄*/
QueueHandle_t adc_msg_q_hdl;
QueueHandle_t temperature_msg_q_hdl;
QueueHandle_t radio_msg_q_hdl;
QueueHandle_t display_msg_q_hdl;

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tasks_init()
{
    /*串口驱动初始化*/
    xuart_init(&xuart_hal_driver);

    /*创建ADC消息队列*/
	adc_msg_q_hdl = xQueueCreate(ADC_MESSAGE_QUEUE_SIZE, sizeof(adc_message_t));
    log_assert(adc_msg_q_hdl);
    /*创建按键消息队列*/
	key_msg_q_hdl = xQueueCreate(KEY_MESSAGE_QUEUE_SIZE, sizeof(key_message_t));
    log_assert(key_msg_q_hdl);
    /*创建显示消息队列*/
	display_msg_q_hdl = xQueueCreate(DISPLAY_MESSAGE_QUEUE_SIZE, sizeof(display_message_t));
    log_assert(display_msg_q_hdl);

    /*创建ADC任务*/
    osThreadDef(radio_task, radio_task, osPriorityNormal, 0, 128);
    radio_task_hdl = osThreadCreate(osThread(radio_task), NULL);
    log_assert(radio_task_hdl);
    /*创建收音机任务*/
    osThreadDef(adc_task, adc_task, osPriorityNormal, 0, 128);
    adc_task_hdl = osThreadCreate(osThread(adc_task), NULL);
    log_assert(adc_task_hdl);
    /*创建显示任务*/
    osThreadDef(display_task, display_task, osPriorityNormal, 0, 128);
    display_task_hdl = osThreadCreate(osThread(display_task), NULL);  
    log_assert(display_task_hdl);  


}