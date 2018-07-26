#include "cmsis_os.h"
#include "radio.h"
#include "tea5767.h"
#include "lib1602a.h"
#include "adc.h"
#include "i2c.h"
#include "log.h"
#define  LOG_MODULE_LEVEL    LOG_LEVEL_DEBUG
#define  LOG_MODULE_NAME     "[radio]"

osThreadId radio_task_hdl;
osThreadId adc_task_hdl;
static uint32_t adc_sample[2];
static uint32_t adc_cusum[2];
static uint32_t adc_average[2];

static uint32_t frequence;
static int8_t   temperature;

#define  ADC_SAMPLE_CNT_MAX           32



#define  ADC_TASK_INTERVAL            5
#define  ADC_TIMEOUT                  osWaitForever  
#define  ADC_COMPLETED_SIGNAL         (1<<0)
#define  ADC_ERROR_SIGNAL             (1<<1)
#define  ADC_ALL_SIGNALS              ((1<<2)-1)

#define  RADIO_TASK_INTERVAL          5
#define  RADIO_TASK_TIMEOUT           osWaitForever
#define  RADIO_TASK_SAMPLE_COMPLETED_SIGNAL  (1<<0)




#define  INTERVAL_FREQ_THRESHOLD          10000

#define  INTERVAL_TEMPERATURE_THRESHOLD   1   
#define  UV25                             1430000
#define  SCATE_RATE_UV_PER_C              4300



void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
osSignalSet(adc_task_hdl,ADC_COMPLETED_SIGNAL);   
}


void  HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
 osSignalSet(adc_task_hdl,ADC_ERROR_SIGNAL);   
}

void adc_task(void const * argument)
{
  osEvent signals;
  HAL_StatusTypeDef  status;
  static uint8_t cnt=0;
  while(1){
adc_start:
  osDelay(ADC_TASK_INTERVAL);
  status =HAL_ADC_Start_DMA(&hadc1,adc_sample,2);
  if(status !=HAL_OK){
adc_reset:
    log_error("start adc dma error:%d.\r\n",status);
    HAL_ADC_MspDeInit(&hadc1);
    osDelay(100);
    MX_ADC1_Init();
    goto adc_start;
  }
  signals = osSignalWait(ADC_COMPLETED_SIGNAL|ADC_ERROR_SIGNAL,ADC_TIMEOUT);
  
  if(signals.status == osEventSignal ){
  if(signals.value.signals & ADC_COMPLETED_SIGNAL){
    if(cnt < ADC_SAMPLE_CNT_MAX){
      adc_cusum[0]+=adc_sample[0];
      adc_cusum[1]+=adc_sample[1];
      cnt++;
    }else{
      adc_average[0]=adc_cusum[0]/cnt;  
      adc_average[1]=adc_cusum[1]/cnt; 
      cnt=0;
      adc_cusum[0]=0;
      adc_cusum[1]=0;
      osSignalSet(radio_task_hdl,RADIO_TASK_SAMPLE_COMPLETED_SIGNAL);      
    }
  }

  if(signals.value.signals & ADC_ERROR_SIGNAL){
  log_error("adc error.reset.\r\n");
  goto adc_reset;
  }
   }
  }
}

void radio_task(void const * argument)
{
  osEvent signals;
  int result;
  static uint32_t freq;
  static int8_t   t;
  char *temperature_str;
  
  while(1){
  signals = osSignalWait(RADIO_TASK_SAMPLE_COMPLETED_SIGNAL,RADIO_TASK_TIMEOUT);
  
  if(signals.status == osEventSignal ){
  if(signals.value.signals & RADIO_TASK_SAMPLE_COMPLETED_SIGNAL){ 
  
/*频率处理*/
   freq= 87500000+adc_average[0]*((108000000-87500000)/4096);  
   log_debug("freq:%d.\r\n",freq);
/*
   if((freq > frequence? freq-frequence : frequence -freq) >= INTERVAL_FREQ_THRESHOLD){
tea5767_set_freq:
   result =tea5767_set_cur_freq(freq);
   if(result !=0){
   log_error("radio event set freq error.\r\n");
   osDelay(100);
   MX_I2C1_Init(); 
   tea5767_init();
   goto tea5767_set_freq;
   }
   frequence=freq;
   log_debug("radio event set freq ok.\r\n");
   }
*/
  
   /*温度处理*/
   t = 25+(UV25 - (int32_t)(adc_average[1]*3300000/4096))/SCATE_RATE_UV_PER_C;
   
   if((t > temperature ? t-temperature:temperature -t ) >= INTERVAL_TEMPERATURE_THRESHOLD){
    temperature =t;
    log_debug("temperature :%d.\r\n",t);
    lib1602a_int_to_str_dec(&temperature_str,t,2);
    lib1602a_display_str(temperature_str,LIB1602A_POS_LINE_2,10);
    log_debug("lcd temperature :%s.\r\n",temperature_str);  
  }
  

  }
  
  }
  }
}   
 
