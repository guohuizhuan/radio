#include "cmsis_os.h"
#include "radio.h"
#include "adc.h"
#include "i2c.h"
#include "log.h"

osThreadId adc_task_hdl;

#define  ADC_TASK_SAMPLE_CHN_CNT      2

static uint32_t adc_sample[ADC_TASK_SAMPLE_CHN_CNT];
static uint32_t adc_cusum[ADC_TASK_SAMPLE_CHN_CNT];
static uint32_t adc_average[ADC_TASK_SAMPLE_CHN_CNT];

static uint32_t frequence;
static int8_t   temperature;

#define  ADC_SAMPLE_CNT_MAX           32



#define  ADC_TASK_INTERVAL            5
#define  ADC_TIMEOUT                  osWaitForever  
#define  ADC_COMPLETED_SIGNAL         (1 << 0)
#define  ADC_ERROR_SIGNAL             (1 << 1)
#define  ADC_ALL_SIGNALS              ((1 << 2) - 1)

#define  RADIO_TASK_INTERVAL          5
#define  RADIO_TASK_TIMEOUT           osWaitForever
#define  RADIO_TASK_SAMPLE_COMPLETED_SIGNAL  (1 << 0)