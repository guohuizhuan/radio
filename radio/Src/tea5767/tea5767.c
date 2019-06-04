#include "tea5767.h"
#include "log.h"


#define BAND_LIMIT_REGION_EUROPE        1
#define BAND_LIMIT_REGION_JAPAN         2

#define BAND_LIMIT_REGION               BAND_LIMIT_REGION_EUROPE

#define FREQ_REF                        32768
#define FREQ_IF                         225000




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
#define  BAND_LIMIT_UP              (108*1000000)
#define  BAND_LIMIT_DOWN            (875*100000)
#else
#define  BAND_LIMIT_UP              (91*1000000)
#define  BAND_LIMIT_DOWN            (76*1000000)
#endif




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
    int rc;
    rc = iic_write(addr,src,size);
    if (rc != 0) {
        log_error("tea5767 write error.\r\n");
        return -1;
    }

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
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
* @brief 
* @param
* @param
* @return 
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
* @brief 
* @param
* @param
* @return 
* @note
*/
static int tea5767_update_config(tea5767_config_t *config)
{
    if (config == NULL) {
        return -1;
    }

    tea5767_config =*config;
    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_init(void) 
{
    int rc;
    
    log_debug("tea5767 init...\r\n");

    tea5767_config.reg1 = 0;
    tea5767_config.mute = CONFIG_MUTE_ON;
    tea5767_config.sm = CONFIG_SEARCH_MODE_DISABLE;
    tea5767_config.reg2 = 0;
    tea5767_config.reg3 = 0;
    tea5767_config.hlsi = CONFIG_HLSI_HIGH;

    tea5767_config.reg4 = 0;
    tea5767_config.hcc = CONFIG_HIGH_CUT_CONTROL_ON;
    tea5767_config.smute = CONFIG_SOFT_MUTE_ON;
    tea5767_config.snc = CONFIG_STEREO_NOISE_CANCLE_ON;
    tea5767_config.stby = CONFIG_EXIT_STANDBY_MODE;
    tea5767_config.xtal = CONFIG_XTAL_32768HZ;

#if (BAND_LIMIT_REGION == BAND_LIMIT_REGION_EUROPE)
    tea5767_config.bl = CONFIG_BAND_LIMIT_EUROPE;
#else
    tea5767_config.bl=CONFIG_BAND_LIMIT_JAPANESE;
#endif

    tea5767_config.reg5 = 0;
    tea5767_config.dtc = 1;

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&tea5767_config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("tea5767 init error.\r\n");
        return -1;
    }

    log_debug("tea5767 init ok..\r\n");

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_read_id(uint8_t *id)
{
    int rc;
    tea5767_status_t status;
    
    rc = tea5767_read(TEA5767_READ_ADDR,(uint8_t *)&status,sizeof(tea5767_status_t));
    if (rc != 0) {
        log_error("tea5767 read id error.\r\n");
        return -1;
    }
    *id = status.ci;
    log_debug("tea5767 id:%d.\r\n",*id);

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_read_level(uint8_t *level)
{
    int rc;
    tea5767_status_t status;
    
    rc = tea5767_read(TEA5767_READ_ADDR,(uint8_t *)&status,sizeof(tea5767_status_t));
    if (rc != 0) {
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

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("injection error.\r\n");
        return -1;
    }
    
    tea5767_update_config(&config);
    log_debug("injection ok.\r\n");

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_mute_on(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.mute = CONFIG_MUTE_ON;

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("mute on error.\r\n");
        return -1;
    }
    
    tea5767_update_config(&config);
    log_debug("mute on ok.\r\n");

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_mute_off(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.mute = CONFIG_MUTE_OFF;

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("mute off error.\r\n");
        return -1;
    }
    
    tea5767_update_config(&config);
    log_debug("mute off ok.\r\n");

    return 0;
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_stereo_on(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.ms = CONFIG_MONO_OFF_STEREO_ON;

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("stereo on error.\r\n");
        return -1;
    }

    tea5767_update_config(&config);
    log_debug("stereo on ok.\r\n");

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_stereo_off(void)
{
    int rc;
    tea5767_config_t config;

    tea5767_get_config(&config);

    config.ms = CONFIG_MONO_ON_STEREO_OFF;

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t *)&config,sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("stereo off error.\r\n");
        return -1;
    }
    
    tea5767_update_config(&config);
    log_debug("stereo off ok.\r\n");
    
    return 0;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int32_t tea5767_get_freq(void)
{
    uint32_t freq;
    uint16_t pll;
    tea5767_status_t status;
    tea5767_config_t config;

    rc = tea5767_read(TEA5767_READ_ADDR,(uint8_t *)&status,sizeof(tea5767_status_t));
    if (rc != 0) {
        return -1;
    }
    
    tea5767_get_config(&config);       
    pll = (uint16_t)status.pllh << 8 | status.plll;
    if (config->hlsi == CONFIG_HLSI_HIGH) {
        freq = (int32_t)pll * FREQ_REF / 4 - FREQ_IF;
    } else {
        freq = (int32_t)pll * FREQ_REF / 4 + FREQ_IF;
    }

    return freq;
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int tea5767_set_freq(int32_t freq)
{
    uint16_t pll;
    tea5767_config_t config;
    
    tea5767_get_config(&config);
    
    if (config->hlsi == CONFIG_HLSI_HIGH) {
        pll = (freq + FREQ_IF) * 4 / FREQ_REF;
    } else {
        pll = (freq - FREQ_IF) * 4 / FREQ_REF;
    }

    config.pllh = pll >> 8;
    config.plll = pll & 0xFF;
    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t*)&config, sizeof(tea5767_config_t));
    if (rc != 0) {
        return -1;;
    }
    
    /*更新缓存的tea5767配置参数，保证参数实时更新*/
    tea5767_update_config(&config);

    return 0;
}


/*
* @brief 
* @param
* @param
* @return 
* @note
*/
static int tea5767_freq_step(int32_t freq_step)
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
* @brief 
* @param
* @param
* @return 
* @note
*/
int32_t tea5767_search(int dir,uint8_t stop_level,uint32_t freq_start)
{
    int rc;
    int is_ok = 0;
    tea5767_status_t status;
    tea5767_config_t config;

    log_debug("tea5767 start search...\r\n");

    rc = tea5767_set_freq(freq);
    if (rc != 0) {
        goto err_exit;
    }
    
    tea5767_get_config(&config);
    
    config.mute = CONFIG_MUTE_ON;
    config.sm = CONFIG_SEARCH_MODE_ENABLE ;
    if (dir > 0) {
        config.sud = CONFIG_SEARCH_DIR_UP;
    } else {
        config.sud = CONFIG_SEARCH_DIR_DOWN;
    }
    config.ssl = stop_level & CONFIG_SEARCH_STOP_LEVEL_MASK;
    

    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t*)&config, sizeof(tea5767_config_t));
    if (rc != 0) {
        goto err_exit;
    }

    while (!is_ok)
        rc = tea5767_read(TEA5767_READ_ADDR,(uint8_t *)&status,sizeof(tea5767_status_t));
        if (rc != 0) {
            goto err_exit;
        }
 
        if (status.rf == STATUS_IS_STATION_FOUND) {
            if (status.blf == STATUS_IS_BAND_LIMIT_REACHED){
                log_warning("band limit reached.\r\n");
                goto err_exit;
            } 
            rc = tea5767_get_freq();
            if (rc < 0) {
                goto err_exit;
            }
            is_ok = 1;
            log_debug("station %d Hz found.\r\n",rc);
        }
    }
 
    return 0;
    config.sm = CONFIG_SEARCH_MODE_DISABLE ;
    config.mute = CONFIG_MUTE_OFF;
    
    rc = tea5767_write(TEA5767_WRITE_ADDR,(uint8_t*)&config, sizeof(tea5767_config_t));
    if (rc != 0) {
        log_error("tea5767 cancle search error.\r\n");
        return -1;
    }

    log_debug("tea5767 cancle search ok.\r\n");
    return 0;    
            rc = tea5767_cancle_search();
            if (rc != 0) {
                return -1;
            }
}


