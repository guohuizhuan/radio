#include "cmsis_os.h"
#include "tasks_init.h"
#include "adc.h"
#include "adc_task.h"
#include "temperature_task.h"
#include "radio_task.h"
#include "log.h"

osThreadId adc_task_hdl;

#define  ADC_TASK_SAMPLE_CHN_CNT             2
#define  ADC_TASK_TEMPERATURE_SAMPLE_INDEX   0
#define  ADC_TASK_RADIO_SAMPLE_INDEX         1

static uint32_t adc_sample[ADC_TASK_SAMPLE_CHN_CNT];
static uint32_t adc_cusum[ADC_TASK_SAMPLE_CHN_CNT];
static uint32_t adc_average[ADC_TASK_SAMPLE_CHN_CNT];

#define  ADC_SAMPLE_CNT_MAX           32



#define  ADC_TASK_INTERVAL            5
#define  ADC_TIMEOUT                  osWaitForever  
#define  ADC_COMPLETED_SIGNAL         (1 << 0)
#define  ADC_ERROR_SIGNAL             (1 << 1)
#define  ADC_ALL_SIGNALS              ((1 << 2) - 1)


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    osSignalSet(adc_task_hdl,ADC_COMPLETED_SIGNAL);   
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void  HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    osSignalSet(adc_task_hdl,ADC_ERROR_SIGNAL);   
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static void adc_reset()
{
    HAL_ADC_MspDeInit(&hadc1);
    MX_ADC1_Init();
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void adc_task(void const * argument)
{
    osEvent signals;
    HAL_StatusTypeDef status;
    temperature_message_t temperature_msg;
    radio_message_t radio_msg;
    static uint8_t cnt = 0;

    while (1) 
    {
    osDelay(ADC_TASK_INTERVAL);
    status = HAL_ADC_Start_DMA(&hadc1,adc_sample,2);
    if (status != HAL_OK) {
        log_error("start adc dma error:%d.\r\n",status);
        osDelay(1000);
        adc_reset();
        continue;
    }
    signals = osSignalWait(ADC_COMPLETED_SIGNAL | ADC_ERROR_SIGNAL,ADC_TIMEOUT);
  
    if (signals.status == osEventSignal) {
        if (signals.value.signals & ADC_COMPLETED_SIGNAL) {
            if (cnt < ADC_SAMPLE_CNT_MAX) {
                for (uint8_t i = 0;i < ADC_TASK_SAMPLE_CHN_CNT;i++) {
                    adc_cusum[i] += adc_sample[i];
                }
                cnt++;
            } else {
                for (uint8_t i = 0;i < ADC_TASK_SAMPLE_CHN_CNT;i++) {
                    adc_average[i] += adc_cusum[i] / ADC_SAMPLE_CNT_MAX;
                    adc_cusum[i] = 0;
                }
                cnt = 0;
                temperature_msg.id = MSG_ID_ADC_COMPLETED;
                temperature_msg.adc.is_err = 0;
                temperature_msg.adc.value = adc_average[0];

                if (xQueueSend(temperature_msg_q_hdl,&temperature_msg,ADC_TASK_PUT_MSG_TIMEOUT) != pdPASS) {
                    log_error("send temperature msg err.\r\n");
                } 
                radio_msg.id = MSG_ID_ADC_COMPLETED;
                radio_msg.adc.is_err = 0;
                radio_msg.adc.value = adc_average[0];

                if (xQueueSend(radio_msg_q_hdl,&radio_msg,ADC_TASK_PUT_MSG_TIMEOUT) != pdPASS) {
                    log_error("send temperature msg err.\r\n");
                }
            }
        }

        if (signals.value.signals & ADC_ERROR_SIGNAL) {
            log_error("adc error.reset.\r\n");
            adc_reset();
            continue;
        }
    }
    }
}
