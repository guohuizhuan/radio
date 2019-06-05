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


TEA5767_BEGIN   

#define  BAND_LIMIT_REGION_EUROPE       1
#define  BAND_LIMIT_REGION_JAPANESE     2

#define  BAND_LIMIT_REGION              BAND_LIMIT_REGION_EUROPE


#define  SEARCH_STOP_LEVEL_5            1
#define  SEARCH_STOP_LEVEL_7            2
#define  SEARCH_STOP_LEVEL_10           3




/*
* @brief tea5767初始化
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_init(void);

/*
* @brief 读tea5767内部id
* @param id id指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_read_id(uint8_t *id);

/*
* @brief 读tea5767电台信号强度
* @param level 信号强度指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_read_level(uint8_t *level);

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_injection(int dir);

/*
* @brief 打开tea5767静音
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_mute_on(void);

/*
* @brief 关闭tea5767静音
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_mute_off(void);

/*
* @brief 打开tea5767立体声
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_stereo_on(void);

/*
* @brief 关闭tea5767立体声
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_stereo_off(void);

/*
* @brief 获取tea5767电台频率
* @param 无
* @param
* @return > 0：成功获取电台的频率 0：失败
* @note
*/
uint32_t tea5767_get_freq(void);

/*
* @brief 设置tea5767电台频率
* @param freq 要设置的频率
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_set_freq(uint32_t freq);

/*
* @brief tea5767频率步进
* @param 步进的频率值
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_freq_step(int32_t freq_step);

/*
* @brief tea5767自动搜索电台
* @param dir 搜索方向
* @param stop_level 搜索停止信号强度
* @return > 0：成功搜索的电台频率 0：失败
* @note dir >= 0 当前频率向上搜索 < 0 当前频率向下搜索
*/
uint32_t tea5767_search(int dir,uint8_t stop_level);








TEA5767_END


#endif

