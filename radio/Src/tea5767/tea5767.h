#ifndef  __TEA5767_H__
#define  __TEA5767_H__
#include "stdint.h"
#include "cmsis_os.h"


#ifdef   __cplusplus
#define  TEA5767_BEGIN        extern "C" {
#define  TEA5767_END          }
#else
#define  TEA5767_BEGIN      
#define  TEA5767_END       
#endif


#define  TEA5767_BEGIN   

#define  BAND_LIMIT_REGION_EUROPE       1
#define  BAND_LIMIT_REGION_JAPANESE     2

#define  BAND_LIMIT_REGION              BAND_LIMIT_REGION_EUROPE


#define  SEARCH_STOP_LEVEL_5            1
#define  SEARCH_STOP_LEVEL_7            2
#define  SEARCH_STOP_LEVEL_10           3




int tea5767_init(void);
int tea5767_id(uint8_t *id);
int tea5767_update(void);
int tea5767_status(void);
int tea5767_mute_on(void);
int tea5767_mute_off(void);
int tea5767_stereo_on(void);
int tea5767_stereo_off(void);
int tea5767_cancle_search(void);
/*unit 0.01MHz*/
int tea5767_search_up(uint8_t stop_level,uint32_t *freq);
/*unit 0.01MHz*/
int tea5767_search_down(uint8_t stop_level,uint32_t *freq);
/*unit 0.01MHz*/
int tea5767_get_cur_freq(uint32_t *freq);
/*unit 0.01MHz*/
int tea5767_set_cur_freq(uint32_t freq);












TEA5767_END


#endif

