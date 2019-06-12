#include "cmsis_os.h"
#include "temperature_task.h"
#include "adc_task.h"
#include "display_task.h"
#include "tasks_init.h"
#include "log.h"



#define  TEMPERATURE_DIFF_THRESHOLD           0.1
#define  UV25                                 1430000.0
#define  UV_PER_C                             43000.0
#define  UV_PER_ADC                           805.664

typedef struct
{
    float value;
    uint8_t is_err;
    uint8_t is_changed;
}temperature_t;

osThreadId temperature_task_hdl;
static temperature_t temperature;
/*
* @brief 
* @param
* @param
* @return 
* @note
*/

void temperature_task(void const * argument)
{
    temperature_message_t temperature_msg;
    display_message_t display_msg;
    uint16_t adc;
    float temp;
    float diff;

    while (1) 
    {
        /*等待消息*/
        if (xQueueReceive(temperature_msg_q_hdl,&temperature_msg,TEMPERATURE_TASK_MSG_WAIT_TIMEOUT)) {

            /*AD转换完成消息*/
            if (temperature_msg.id == MSG_ID_ADC_COMPLETED) {
                /*adc是错误的*/
                if (temperature_msg.adc.is_err) {
                    if (!temperature.is_err) {
                        temperature.is_err = 1;
                        temperature.is_changed = 1;
                        log_error("temperature bad.\r\n");
                    }
                    continue;
                }

                adc = temperature_msg.adc.value;
                temp = 25.0 + (UV25 - (float)adc * UV_PER_ADC) / UV_PER_C;
                diff = temp - temperature.value;
                if (diff >= TEMPERATURE_DIFF_THRESHOLD || diff <= -TEMPERATURE_DIFF_THRESHOLD) {
                    temperature.value = temp;
                    temperature.is_changed = 1;
                    if (temperature.is_err) {
                        temperature.is_err = 0;
                    }

                    log_debug("temperature change to:%.1fC\r\n",temperature.value);
                }
                /*温度变化就发送消息*/
                if (temperature.is_changed) {
                    temperature.is_changed = 0;

                    display_msg.id = TEMPERATURE_DISPLAY_MSG;
                    display_msg.is_err = temperature.is_err;
                    display_msg.temperature = temperature.value;
                    if (xQueueSend(display_msg_q_hdl,&display_msg,TEMPERATURE_TASK_PUT_MSG_TIMEOUT) != pdPASS) {
                        log_error("send display msg err.\r\n");
                    }  
                } 
            }

            /*其他消息*/
            /*
            if (temperature_msg.id == MSG_ID_ADC_COMPLETED) {

            }
            */
        }

    }

}