#include "tea5767.h"
#include "log.h"


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
#define  CONFIG_PLLL_MASK                   (0xFF)
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


#define TEA5767_WRITE_ADDR              0xC0 // I2C address of TEA5767
#define TEA5767_READ_ADDR               0xC1 // I2C address of TEA5767
 
#if (BAND_LIMIT_REGION == BAND_LIMIT_REGION_EUROPE)
#define  BAND_LIMIT_UP                  (108*1000000)
#define  BAND_LIMIT_DOWN                (875*100000)
#else
#define  BAND_LIMIT_UP                  (91*1000000)
#define  BAND_LIMIT_DOWN                (76*1000000)
#endif


#define  FREQ_REF                        32768
#define  FREQ_IF                         225000



/*tea5767缓存的配置参数*/
static tea5767_config_t tea5767_config;


/*外部声明*/
int iic_write(uint8_t addr,uint8_t *buff,uint8_t cnt);
int iic_read(uint8_t addr,uint8_t *buff,uint8_t cnt);



/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int tea5767_write(uint8_t addr,uint8_t *src,uint32_t size)
{
<<<<<<< HEAD
    int rc;
    rc = iic_write(addr,src,size);
    if (rc != 0) {
        log_error("tea5767 write error.\r\n");
        return -1;
    }
=======
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
>>>>>>> 0d9120d3f4ab0b73a65ce95f15b652cc04c1e9b5

    return 0;
}


/*
* @brief 读寄存器操作
* @param addr tea5767器件读操作地址
* @param dst 数据存储的地址指针
* @param size 读取的数量
* @return 0：成功 -1：失败
* @note
*/
static int tea5767_read(uint8_t addr,uint8_t *dst,uint32_t size)
{
    int rc;

    rc = iic_read(addr,dst,size);
    if (rc != 0) {
        log_error("tea5767 read error.\r\n");
        return -1;
    }

    return 0;
}

/*
* @brief 读tea5767状态寄存器
* @param status 状态结构指针
* @param
* @return 0：成功 -1：失败
* @note
*/
static int tea5767_read_status(tea5767_status_t *status)
{
    int rc;
    rc = tea5767_read(TEA5767_READ_ADDR,(uint8_t *)status,sizeof(tea5767_status_t));
    if (rc != 0) {
        log_error("tea5767 err in read status.\r\n");
        return -1;
    }

    return 0;

}

/*
* @brief 获取tea5767缓存的配置参数
* @param config 配置参数指针
* @param
* @return 0：成功 -1：失败
* @note
*/
static int tea5767_get_config(tea5767_config_t *config)
{
    if (config == NULL) {
        return -1;
    }

    *config = tea5767_config;
    return 0;
}


/*
* @brief 更新tea5767缓存的参数
* @param config 新配置参数指针
* @param
* @return 0：成功 -1：失败
* @note
*/
static int tea5767_update_config(tea5767_config_t *config)
{
    if (config == NULL) {
        return -1;
    }

    tea5767_config = *config;
    return 0;
}

/*
* @brief 更新配置参数到tea5767芯片内部并缓存
* @param config 配置参数指针
* @param
* @return 0：成功 -1：失败
* @note
*/
static int tea5767_flush_config(tea5767_config_t *config)
{
    int rc;
    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)config,sizeof(tea5767_config_t));
    if (rc != 0) {
        return -1;
    }
    /*每一次flush成功都要缓存配置参数*/
    tea5767_config = *config;

    return 0;
}



/*
* @brief 根据hlsi和pll计算频率
* @param hlsi injection值
* @param pll 频率锁相环值
* @return 频率值
* @note
*/
static uint32_t tea5767_calculate_freq(uint8_t hlsi,uint16_t pll)
{
    uint32_t freq;
    
    if (hlsi == CONFIG_HLSI_HIGH) {
        freq = (uint32_t)pll * FREQ_REF / 4 - FREQ_IF;
    } else {
        freq = (uint32_t)pll * FREQ_REF / 4 + FREQ_IF;
    }

    return freq;
}

/*
* @brief 根据hlsi和freq计算锁相环值
* @param hlsi injection值
* @param freq 频率值
* @return 频率锁相环值
* @note
*/
static uint16_t tea5767_calculate_pll(uint8_t hlsi,uint32_t freq)
{
    uint16_t pll;

    if (hlsi == CONFIG_HLSI_HIGH) {
        pll = (freq + FREQ_IF) * 4 / FREQ_REF;
    } else {
        pll = (freq - FREQ_IF) * 4 / FREQ_REF;
    }

    return pll;

}

/*
* @brief tea5767初始化
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_init(void) 
{
    int rc;
    tea5767_config_t config;
    
    log_debug("tea5767 init...\r\n");

    config.reg1 = 0;
    config.mute = CONFIG_MUTE_ON;
    config.sm = CONFIG_SEARCH_MODE_DISABLE;
    config.reg2 = 0;
    config.reg3 = 0;
    config.hlsi = CONFIG_HLSI_HIGH;

    config.reg4 = 0;
    config.hcc = CONFIG_HIGH_CUT_CONTROL_ON;
    config.smute = CONFIG_SOFT_MUTE_ON;
    config.snc = CONFIG_STEREO_NOISE_CANCLE_ON;
    config.stby = CONFIG_EXIT_STANDBY_MODE;
    config.xtal = CONFIG_XTAL_32768HZ;

#if (BAND_LIMIT_REGION == BAND_LIMIT_REGION_EUROPE)
    config.bl = CONFIG_BAND_LIMIT_EUROPE;
#else
    config.bl=CONFIG_BAND_LIMIT_JAPANESE;
#endif

    config.reg5 = 0;
    config.dtc = 1;

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("tea5767 flush config error. init error.\r\n");
        return -1;
    }

    log_debug("tea5767 init ok..\r\n");

    return 0;
}


/*
* @brief 读tea5767内部id
* @param id id指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_read_id(uint8_t *id)
{
    int rc;
    tea5767_status_t status;

    rc = tea5767_read_status(&status)
    if (rc != 0) {
        log_error("tea5767 read id error.\r\n");
        return -1;
    }
    *id = status.ci;
    log_debug("tea5767 id:%d.\r\n",*id);

    return 0;
}


/*
* @brief 读tea5767电台信号强度
* @param level 信号强度指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_read_level(uint8_t *level)
{
    int rc;
    tea5767_status_t status;
    
    rc = tea5767_read_status(&status);
    if (rc != 0) {
        log_error("tea5767 read level error.\r\n");
        return -1;
    }
    *level = status.lev;
    log_debug("tea5767 level:%d.\r\n",*level);

    return 0;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_injection(int dir)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    if (dir > 0) {
        config->hlsi == CONFIG_HLSI_HIGH;
    } else {
        config->hlsi == CONFIG_HLSI_LOW;
    }
    
    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("injection error.\r\n");
        return -1;
    }
    
    log_debug("injection ok.\r\n");

    return 0;
}


/*
* @brief 打开tea5767静音
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_mute_on(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.mute = CONFIG_MUTE_ON;

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("mute on error.\r\n");
        return -1;
    }
    
    log_debug("mute on ok.\r\n");

    return 0;
}


/*
* @brief 关闭tea5767静音
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_mute_off(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.mute = CONFIG_MUTE_OFF;

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("mute off error.\r\n");
        return -1;
    }
    
    log_debug("mute off ok.\r\n");

    return 0;
}

/*
* @brief 打开tea5767立体声
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_stereo_on(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.ms = CONFIG_MONO_OFF_STEREO_ON;

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("stereo on error.\r\n");
        return -1;
    }

    log_debug("stereo on ok.\r\n");

    return 0;
}


/*
* @brief 关闭tea5767立体声
* @param 无
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_stereo_off(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.ms = CONFIG_MONO_ON_STEREO_OFF;

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("stereo off error.\r\n");
        return -1;
    }
    
    log_debug("stereo off ok.\r\n");
    
    return 0;
}

/*
* @brief 获取tea5767电台频率
* @param 无
* @param
* @return > 0：成功获取电台的频率 0：失败
* @note
*/
uint32_t tea5767_get_freq(void)
{
    uint32_t freq;
    uint16_t pll;
    tea5767_config_t config;
    tea5767_status_t status;

    rc = tea5767_read_status(&status);
    if (rc != 0) {
        log_error("tea5767 get freq error.\r\n");
        return 0;
    }

    tea5767_get_config(&config);
    pll = (uint16_t)status.pllh << 8 | status.plll;
    freq = tea5767_calculate_freq(config.hlsi, uint16_t pll)
    
    log_debug("tea5767 get freq:%d ok.\r\n",freq);
    return freq;
    
}

/*
* @brief 设置tea5767电台频率
* @param freq 要设置的频率
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_set_freq(uint32_t freq)
{
    uint16_t pll;
    tea5767_config_t config;
    
    tea5767_get_config(&config);
    
    pll = tea5767_calculate_pll(config.hlsi,freq);
    config.pllh = pll >> 8;
    config.plll = pll & 0xFF;
    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        log_error("set freq error.\r\n");
        return -1;
    }
    
    log_debug("set freq ok.\r\n");

    return 0;
}


/*
* @brief tea5767频率步进
* @param 步进的频率值
* @param
* @return 0：成功 -1：失败
* @note
*/
int tea5767_freq_step(int32_t freq_step)
{
    int rc;
    int32_t freq;
    uint16_t pll;

    freq = tea5767_get_freq();
    if (freq < 0) {
        goto err_exit;
    }

    freq += freq_step;
    if (freq > BAND_LIMIT_UP) {
        log_error("freq setting %d Hz too large than %d Hz.\r\n",freq,BAND_LIMIT_UP);
    } else if (freq < BAND_LIMIT_DOWN) {
        log_error("freq setting  %d Hz too small than %d Hz.\r\n",freq,BAND_LIMIT_DOWN);
    } else {
        log_debug("freq setting  %d Hz...\r\n",freq);
    }

    rc = tea5767_set_freq(freq);
    if (rc != 0) {
        goto err_exit;
    }
    
    log_debug("tea5767 freq step ok.freq:%d.\r\n",freq);
    return 0;
    
err_exit:
    log_error("tea5767 freq step error.\r\n");
    return  -1;
}


/*
* @brief tea5767自动搜索电台
* @param dir 搜索方向
* @param stop_level 搜索停止信号强度
* @return > 0：成功搜索的电台频率 0：失败
* @note dir >= 0 当前频率向上搜索 < 0 当前频率向下搜索
*/
uint32_t tea5767_search(int dir,uint8_t stop_level)
{
    int rc;
    int is_ok = 0;
    uint32_t freq;
    uint16_t pll;
    
    tea5767_status_t status;
    tea5767_config_t config;

    log_debug("tea5767 start search...\r\n");

    tea5767_get_config(&config);
    
    config.mute = CONFIG_MUTE_ON;
    config.sm = CONFIG_SEARCH_MODE_ENABLE ;
    if (dir >= 0) {
        config.sud = CONFIG_SEARCH_DIR_UP;
    } else {
        config.sud = CONFIG_SEARCH_DIR_DOWN;
    }
    config.ssl = stop_level & CONFIG_SEARCH_STOP_LEVEL_MASK;
    

    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        goto err_exit;
    }

    
    while (!is_ok)
    {
        rc = tea5767_read_status(&status);
        if (rc != 0) {
            goto err_exit;
        }
 
        if (status.rf == STATUS_IS_STATION_FOUND) {
            if (status.blf == STATUS_IS_BAND_LIMIT_REACHED) {
                log_warning("band limit reached.\r\n");
                goto err_exit;
            } 
            pll = (uint16_t)status.pllh << 8 | status.plll;
            freq = tea5767_calculate_freq(config.hlsi,pll);
            is_ok = 1;
            log_debug("station %d Hz found.\r\n",freq);
            /*回写搜索到的频率到tea5767*/
            config.pllh = status.pllh;
            config.plll = status.plll;
            rc = tea5767_flush_config(&config);
            if (rc != 0) {
                goto err_exit;
            }
        }
    }
 
    config.sm = CONFIG_SEARCH_MODE_DISABLE ;
    config.mute = CONFIG_MUTE_OFF;
    
    rc = tea5767_flush_config(&config);
    if (rc != 0) {
        
        goto err_exit;
    }

    log_debug("tea5767 cancle search ok.\r\n");

    return freq;
    
err_exit:
    log_error("tea5767 search error.\r\n");

    return 0;    

}


