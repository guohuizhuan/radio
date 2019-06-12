#include "cmsis_os.h"
#include "radio_task.h"
#include "tea5767.h"
#include "display_task.h"
#include "tasks_init.h"
#include "log.h"


osThreadId radio_task_hdl;

#define  RADIO_TASK_PUT_MSG_TIMEOUT       5
#define  RADIO_TASK_MSG_WAIT_TIMEOUT      osWaitForever
#define  FREQUENCY_STEP                   100000 /*0.1 MHz*/

typedef struct
{
    uint8_t is_err;
    uint8_t is_changed;
    uint8_t signal_level;
    float value_mhz;/*MHz*/
    uint32_t value_100khz;/*100 kHz*/
}fm_frequency_t;

static fm_frequency_t fm_freq;


/*
* @brief 
* @param
* @param
* @return 
* @note
*/

void radio_task(void const * argument)
{
    radio_message_t radio_msg;
    display_message_t display_msg;
    uint16_t adc;
    uint32_t temp;
    uint8_t level_hi,level_lo;

    tea5767_init();

    while (1) 
    {
        /*等待消息*/
        if (xQueueReceive(radio_msg_q_hdl,&radio_msg,RADIO_TASK_MSG_WAIT_TIMEOUT)) {

            /*AD转换完成消息*/
            if (radio_msg.id == MSG_ID_ADC_COMPLETED) {
                /*adc是错误的*/
                if (radio_msg.adc.is_err) {
                    if (!fm_freq.is_err) {
                        fm_freq.is_err = 1;
                        fm_freq.is_changed = 1;
                        log_error("fm_freq bad.\r\n");
                    }
                    continue;
                }
    
                adc = radio_msg.adc.value;
                /*频率处理 换算成100 kHz为单位*/
                temp = (87500000 + adc * ((108000000 - 87500000) / 4096)) / FREQUENCY_STEP;  
                if (temp != fm_freq.value_100khz) {
                    fm_freq.is_changed = 1;
                    fm_freq.value_100khz = temp;
                    fm_freq.value_mhz = temp / 10.0;
                    log_debug("freq change to:%.1f MHz.\r\n",fm_freq.value_mhz);
                }
                /*如果频率发生了改变*/
                if (fm_freq.is_changed) {
                    fm_freq.is_changed = 0;
                    /*设置tea5767新频率*/
                    tea5767_set_freq(fm_freq.value_100khz * FREQUENCY_STEP);
                    /*设置tea5767上偏移*/
                    tea5767_injection(1);
                    /*读信号质量*/
                    tea5767_read_level(&level_hi);
                    /*设置tea5767下偏移*/
                    tea5767_injection(-1);
                    /*读信号质量*/
                    tea5767_read_level(&level_lo);
                    if (level_hi >= level_lo) {
                        fm_freq.signal_level = level_hi;
                        /*重新设置tea5767上偏移*/
                        tea5767_injection(1);
                    } else {
                        fm_freq.signal_level = level_lo;
                    }

                    /*发送频率显示消息*/
                    display_msg.id = FM_FREQ_DISPLAY_MSG;
                    display_msg.fm_freq = fm_freq.value_mhz;
                    if (xQueueSend(display_msg_q_hdl,&display_msg,RADIO_TASK_PUT_MSG_TIMEOUT) != pdPASS) {
                        log_error("send display msg err.\r\n");
                    } 
                    /*发送信号显示消息*/
                    display_msg.id = SIGNAL_LEVEL_DISPLAY_MSG;
                    display_msg.signal_level = fm_freq.signal_level;
                    if (xQueueSend(display_msg_q_hdl,&display_msg,RADIO_TASK_PUT_MSG_TIMEOUT) != pdPASS) {
                        log_error("send display msg err.\r\n");
                    } 
                }                           
            }

            /*其他消息*/
            /*
            if (radio_msg.id == MSG_ID_ADC_COMPLETED) {

            }
            */

        }
    }
}   
 
