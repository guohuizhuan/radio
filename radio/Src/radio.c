#include "cmsis_os.h"
#include "radio.h"
#include "tea5767.h"
#include "lib1602a.h"
#include "adc.h"
#include "i2c.h"
#include "log.h"

osThreadId radio_task_hdl;





#define  INTERVAL_FREQ_THRESHOLD          32000

#define  INTERVAL_TEMPERATURE_THRESHOLD   1   
#define  UV25                             1430000
#define  SCATE_RATE_UV_PER_C              4300


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
                cnt=0;
                adc_cusum[0] = 0;
                adc_cusum[1] = 0;
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

    osPoolDef(memory_pool,MEMORY_POOL_BLOCK_CNT,memory_pool_block_t);
    memory_pool_id = osPoolCreate(osPool(memory_pool));
    if (memory_pool_id == NULL) {
        return -1;
    }

void radio_task(void const * argument)
{
  osEvent signals;
  int result;
  static uint32_t freq;
  static int8_t   t;
  char *temperature_str;
  tea5767_init();
  while(1){
  signals = osSignalWait(RADIO_TASK_SAMPLE_COMPLETED_SIGNAL,RADIO_TASK_TIMEOUT);
  
  if(signals.status == osEventSignal ){
  if(signals.value.signals & RADIO_TASK_SAMPLE_COMPLETED_SIGNAL){ 
  
/*频率处理*/
   freq= 87500000+adc_average[0]*((108000000-87500000)/4096);  
   //log_debug("freq:%d.\r\n",freq);

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

  
 /*温度处理*/
 /*
   t = 25+(UV25 - (int32_t)(adc_average[1]*3300000/4096))/SCATE_RATE_UV_PER_C;
   
   if((t > temperature ? t-temperature:temperature -t ) >= INTERVAL_TEMPERATURE_THRESHOLD){
    temperature =t;
    log_debug("temperature :%d.\r\n",t);
    lib1602a_int_to_str_dec(&temperature_str,t,2);
    lib1602a_display_str(temperature_str,LIB1602A_POS_LINE_2,10);
    log_debug("lcd temperature :%s.\r\n",temperature_str);  
  }
*/
  

  }
  
  }
  }
}   
 
