#include "cmsis_os.h"
#include "adc_task.h"
#include "radio_task.h"

#include "log.h"

osThreadId adc_task_hdl;

#define  ADC_TASK_SAMPLE_CHN_CNT      2

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
                adc_cusum[0] += adc_sample[0];
                adc_cusum[1] += adc_sample[1];
                cnt++;
            } else {
                adc_average[0] = adc_cusum[0] / cnt;  
                adc_average[1] = adc_cusum[1] / cnt; 
                cnt = 0;
                adc_cusum[0] = 0;
                adc_cusum[1] = 0;
                osSignalSet(radio_task_hdl,RADIO_TASK_SAMPLE_COMPLETED_SIGNAL);      
            }
        }

        if (signals.value.signals & ADC_ERROR_SIGNAL) {
            log_error("adc error.reset.\r\n");
            goto adc_reset;
        }
    }
    }
}
