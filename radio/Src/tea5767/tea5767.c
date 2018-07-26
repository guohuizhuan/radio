#include "tea5767.h"
#include "log.h"
#define  LOG_MODULE_LEVEL    LOG_LEVEL_DEBUG
#define  LOG_MODULE_NAME     "[tea5767]"




#define TEA5767_WRITE_ADDR      0xC0 // I2C address of TEA5767
#define TEA5767_READ_ADDR       0xC1 // I2C address of TEA5767

#define QUARTZ           32768
#define FILTER           225000


typedef struct 
{
union {
uint8_t reg1;
struct {
uint8_t pllh:6;
uint8_t mute:1;
uint8_t sm:1;
};
};

union  {
uint8_t reg2;
struct {
uint8_t plll:8;
};
};

union {
uint8_t reg3;
struct {
uint8_t swp1:1;
uint8_t ml:1;
uint8_t mr:1;
uint8_t ms:1;
uint8_t hlsi:1;
uint8_t ssl:2;
uint8_t sud:1;
};
};

union {
uint8_t reg4;
struct {
uint8_t si:1;
uint8_t snc:1;
uint8_t hcc:1;
uint8_t smute:1;
uint8_t xtal:1;
uint8_t bl:1;
uint8_t stby:1;
uint8_t swp2:1;
};
};

union{
uint8_t reg5;
struct {
uint8_t reg5_reserved:6;
uint8_t dtc:1;
uint8_t pllref:1;
};
};
}tea5767_config_t;



typedef struct 
{
union {
uint8_t reg1;
struct {
uint8_t pllh:6;
uint8_t blf:1;
uint8_t rf:1;
};
};

union {
uint8_t reg2;
struct {
uint8_t plll:8;
};
};

union {
uint8_t reg3;
struct {
uint8_t ifs:7;
uint8_t stereo:1;
};
};

union {
uint8_t reg4;
struct {
uint8_t reg4_reserved:1;
uint8_t ci:3;
uint8_t lev:4;
};
};

union{
uint8_t reg5;
struct {
uint8_t reg5_reserved:8;
};
};
}tea5767_status_t;

/*******************************************/
/*            config regs                  */
/*******************************************/

/*reg1*/
#define  CONFIG_MUTE_ON                     1
#define  CONFIG_MUTE_OFF                    0
#define  CONFIG_SEARCH_MODE_ENABLE          1
#define  CONFIG_SEARCH_MODE_DISABLE         0
#define  CONFIG_PLLH_MASK                   ((1<<6)-1)
/*reg2*/
#define  CONFIG_PLLL_MASK                    (0xFF)
/*reg3*/
#define  CONFIG_SEARCH_DIR_UP               1
#define  CONFIG_SEARCH_DIR_DOWN             0
#define  CONFIG_SEARCH_STOP_LEVEL_MASK      (0x03)
#define  CONFIG_HLSI_HIGH                   1
#define  CONFIG_HLSI_LOW                    0
#define  CONFIG_MONO_ON_STEREO_OFF          1
#define  CONFIG_MONO_OFF_STEREO_ON          0
#define  CONFIG_RIGHT_CHANNEL_MUTE_ON       1
#define  CONFIG_RIGHT_CHANNEL_MUTE_OFF      0
#define  CONFIG_LEFT_CHANNEL_MUTE_ON        1
#define  CONFIG_LEFT_CHANNEL_MUTE_OFF       0
#define  CONFIG_SWP1_SET                    1
#define  CONFIG_SWP1_CLR                    0
/*reg4*/
#define  CONFIG_SWP2_SET                    1
#define  CONFIG_SWP2_CLR                    0
#define  CONFIG_ENTRY_STANDBY_MODE          1
#define  CONFIG_EXIT_STANDBY_MODE           0
#define  CONFIG_BAND_LIMIT_JAPANESE         1
#define  CONFIG_BAND_LIMIT_EUROPE           0
#define  CONFIG_XTAL_32768HZ                1
#define  CONFIG_XTAL_13000KHZ               0
#define  CONFIG_SOFT_MUTE_ON                1
#define  CONFIG_SOFT_MUTE_OFF               0
#define  CONFIG_HIGH_CUT_CONTROL_ON         1
#define  CONFIG_HIGH_CUT_CONTROL_OFF        0
#define  CONFIG_STEREO_NOISE_CANCLE_ON      1
#define  CONFIG_STEREO_NOISE_CANCLE_OFF     0
#define  CONFIG_SEARCH_INDICATORE_ON        1
#define  CONFIG_SEARCH_INDICATORE_OFF       0
/*reg5*/
#define  CONFIG_PLL_REFERENCE_6500KHZ_ON    1
#define  CONFIG_PLL_REFERENCE_6500KHZ_OFF   0
#define  CONFIG_DTC_75US                    1
#define  CONFIG_DTC_50US                    0

/*******************************************/
/*            status regs                  */
/*******************************************/
/*reg1*/
#define  STATUS_IS_STATION_FOUND            1
#define  STATUS_IS_BAND_LIMIT_REACHED       1
/*reg2*/
/*reg3*/
#define  STATUS_IS_STEREO_STATION           1
/*reg4*/
/*reg5*/


#define  MHZ_VALUE                  1000000UL
#define  FREQ_UINIT                 10000U



#if (BAND_LIMIT_REGION == BAND_LIMIT_REGION_EUROPE)
#define  BAND_LIMIT_UP              (108*1000000)
#define  BAND_LIMIT_DOWN            (875*100000)
#else
#define  BAND_LIMIT_UP              (91*1000000)
#define  BAND_LIMIT_DOWN            (76*1000000)
#endif





static tea5767_config_t config;
static tea5767_status_t status;

int iic_write(uint8_t addr,uint8_t *buff,uint8_t cnt);
int iic_read(uint8_t addr,uint8_t *buff,uint8_t cnt);




int tea5767_init(void) 
{
int result;
log_debug("init start...\r\n");

config.reg1=0;
config.mute=CONFIG_MUTE_OFF;
config.sm=CONFIG_SEARCH_MODE_DISABLE;
config.reg2=0;
config.reg3=0;
config.hlsi=CONFIG_HLSI_HIGH;

config.reg4=0;
config.smute=CONFIG_SOFT_MUTE_ON;
config.snc=CONFIG_STEREO_NOISE_CANCLE_ON;
config.stby=CONFIG_EXIT_STANDBY_MODE;
config.xtal=CONFIG_XTAL_32768HZ;

#if (BAND_LIMIT_REGION == BAND_LIMIT_REGION_EUROPE)
config.bl=CONFIG_BAND_LIMIT_EUROPE;
#else
config.bl=CONFIG_BAND_LIMIT_JAPANESE;
#endif

config.reg5=0;
config.dtc=1;

result =tea5767_update();
if(result == -1){
log_error("init  error.\r\n");
return -1;
}

tea5767_set_cur_freq(87500000);
log_debug("init ok.done.\r\n");

return 0;

}


int tea5767_update(void)
{
 int result;
 result = iic_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,5);
 return result;
}

int tea5767_status(void)
{
 int result;
 result = iic_read(TEA5767_READ_ADDR,(uint8_t *)&status,5);
 return result;
}

int tea5767_id(uint8_t *id)
{
 int result;
 result =tea5767_status();
 if(result == -1){
 log_error("read id error.\r\n");
 return -1;
 }
 *id=status.ci;
 log_debug("read id:%d ok.\r\n",*id);

 return 0;
}

int tea5767_mute_on(void)
{
 int result;
 
config.mute=CONFIG_MUTE_ON;

result = tea5767_update();
if(result == -1){
log_error("mute on error.\r\n");
return -1;
}
log_debug("mute on ok.\r\n");

return 0;

}

int tea5767_mute_off(void)
{
int result;

config.mute=CONFIG_MUTE_OFF;
result = tea5767_update();
if(result == -1){
log_error("mute off error.\r\n");
return -1;
}
log_debug("mute off ok.\r\n");

return 0;

}

int tea5767_stereo_on(void)
{
int result;

config.ms=CONFIG_MONO_OFF_STEREO_ON;
result = tea5767_update();
if(result == -1){
log_error("stereo on error.\r\n");
return -1;
}
log_debug("stereo on ok.\r\n");


return 0;
}

int tea5767_stereo_off(void)
{
 int result;

config.ms=CONFIG_MONO_ON_STEREO_OFF;
result = tea5767_update();
if(result == -1){
log_error("stereo off error.\r\n");
return -1;
}
log_debug("stereo off ok.\r\n");

return 0;

}

static int tea5767_pll_increase_one_step()
{
 uint32_t freq;
 uint16_t pll;
 pll=(uint16_t)config.pllh << 8 |config.plll;
 freq= (pll * (QUARTZ / 4)) - FILTER;
 freq+=(32768*3);
 pll = (freq + FILTER) / (QUARTZ/4);
 config.pllh=(pll >> 8) & CONFIG_PLLH_MASK;
 config.plll=(pll & CONFIG_PLLL_MASK);
 return  0;
}

static int tea5767_pll_decrease_one_step()
{
 uint32_t freq;
 uint16_t pll;
 pll=(uint16_t)config.pllh << 8 |config.plll;
 freq= (pll * (QUARTZ / 4)) - FILTER;
 freq-=(32768*3);
 pll = (freq + FILTER) / (QUARTZ/4);
 config.pllh=(pll >> 8) & CONFIG_PLLH_MASK;
 config.plll=(pll & CONFIG_PLLL_MASK);
 return  0;
}

static int tea5767_pll_sync()
{

 log_debug("pll sync start...\r\n");

 config.pllh=status.pllh;
 config.plll=status.plll;
 
 log_debug("pll sync done.\r\n");  
 return 0;
}


int tea5767_cancle_search(void)
{
int result;

config.sm=CONFIG_SEARCH_MODE_DISABLE ;
config.mute=CONFIG_MUTE_OFF;
result = tea5767_update();
if(result == -1){
log_error("cancle search error.\r\n");
return -1;
}
log_debug("cancle search ok.\r\n");

return 0;
}


int tea5767_search_up(uint8_t stop_level,uint32_t *freq)
{
 int result;
 
 tea5767_pll_increase_one_step();

 config.mute=CONFIG_MUTE_ON;
 config.sm=CONFIG_SEARCH_MODE_ENABLE ;
 config.sud=CONFIG_SEARCH_DIR_UP;
 config.ssl=stop_level & CONFIG_SEARCH_STOP_LEVEL_MASK;
 
 result = tea5767_update();
 if(result == -1){
 log_error("search up error.\r\n");
 return -1;
 }

 while(1){
 osDelay(50);
 result = tea5767_status();
 if(result == -1){
 log_error("search up error.\r\n");
 return -1;
 }
 
 if(status.rf == STATUS_IS_STATION_FOUND ){
  tea5767_pll_sync();
  result =tea5767_cancle_search();
  if(result == -1){
  return -1;
 }
 if(status.blf== STATUS_IS_BAND_LIMIT_REACHED){
   log_warning("up band limit reached.\r\n");
   return 1;
 }else{
   result = tea5767_get_cur_freq(freq);
   if(result ==-1){
   	return -1;
   	}
  log_debug("station %d Hz found.\r\n",*freq);
  break;
 	}
 }
 }
 
 return 0;
}
int tea5767_search_down(uint8_t stop_level,uint32_t *freq)
{
    int result;
    result =tea5767_pll_decrease_one_step();
    if(result == -1){
    log_error("search down error.\r\n");
    return -1;
    }
    config.mute=CONFIG_MUTE_ON;
	config.sm=CONFIG_SEARCH_MODE_ENABLE ;
	config.sud=CONFIG_SEARCH_DIR_DOWN;
	config.ssl=stop_level & CONFIG_SEARCH_STOP_LEVEL_MASK;
	
	result = tea5767_update();
	if(result == -1){
	 log_error("search down error.\r\n");
	return -1;
	}

	while(1){
    osDelay(50);
	result = tea5767_status();
	if(result == -1){
	log_error("search down error.\r\n");
	return -1;
	}
	
	if(status.rf == STATUS_IS_STATION_FOUND ){
    tea5767_pll_sync();
    result =tea5767_cancle_search();
    if(result == -1){
    return -1;
    }
	if(status.blf== STATUS_IS_BAND_LIMIT_REACHED){
	  log_warning("down band limit reached.\r\n");
	  return 1;
	}else{
	  result = tea5767_get_cur_freq(freq);
      if(result ==-1){	
   	  return -1;
   	  }
     log_debug("station %d Hz found.\r\n",*freq);
	 break;
	   }
	}
	}
  
   return 0;

}


/*unit 0.01MHz*/
int tea5767_get_cur_freq(uint32_t *freq)
{
int result;
uint16_t pll;

result =tea5767_status();
if(result == -1){
log_error("get freq error.\r\n");
return -1;
}
pll=(uint16_t)status.pllh << 8 |status.plll;
*freq= (pll * (QUARTZ / 4)) - FILTER;

log_debug("get cur freq: %d Hz.ok.\r\n",*freq);
log_debug("ifs: %d.\r\n",status.ifs);
log_debug("level: %d.\r\n",status.lev);
return 0;
}


/*unit 0.01MHz*/
int tea5767_set_cur_freq(uint32_t freq)
{
int result;
uint16_t pll;

if(freq > BAND_LIMIT_UP || freq < BAND_LIMIT_DOWN){
log_error("freq :%d Hz is invalid.\r\n",freq);
return -1;
}

pll = (freq + FILTER) / (QUARTZ/4);
config.pllh=(pll >> 8) & CONFIG_PLLH_MASK;
config.plll=(pll & CONFIG_PLLL_MASK);

result =tea5767_update();
if(result == -1){
log_error("set freq error.\r\n");
return -1;
}

log_debug("set freq: %d Hz) ok.\r\n",freq);

return 0;
}






















