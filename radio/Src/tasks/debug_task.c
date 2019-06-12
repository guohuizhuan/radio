#include "cmsis_os.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "debug_task.h" 
#include "tea5767.h"
#include "log.h"

osThreadId debug_task_hdl;

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void debug_task(void const * argument)
{
    int rc;
    char cmd[20];
    uint8_t id;
    uint32_t freq;
 
    char *cmd_raw;
    char *cmd_str;
    char *param_str;

    log_debug("waiting for cmd...\r\n");

    while (1) {

        osDelay(100);
        rc = log_read(cmd,20);
        cmd[19] = 0;

        if (rc == 0) {
            continue;
        }
        rc = -1;
        
        cmd_raw = strtok((char *)cmd,"\r");
        if (cmd_raw == NULL) {
            continue;
        }

        cmd_str = strtok(cmd_raw," ");
        if (cmd_str) {
            param_str = strtok(NULL," ");
        } 

        /*读取ID*/
        if (strcmp(cmd_str,"id") == 0) {
            rc = tea5767_read_id(&id);   
        }
        /*初始化*/
        if (strcmp(cmd_str,"init") == 0) {
            rc = tea5767_init();   
        }

        /*搜索台*/
        if (strcmp(cmd_str,"search") == 0) {
            if (strcmp(param_str,"up") == 0) {
                rc = tea5767_search(1,SEARCH_STOP_LEVEL_10);   
            } else {
                rc = tea5767_search(-1,SEARCH_STOP_LEVEL_10);   
            }
        }
        /*设置频率*/
        if (strcmp(cmd_str,"set_freq") == 0 && param_str) {
            freq = atoi((char *)param_str);
            rc = tea5767_set_freq(freq * 100000);   
        }
        /*获取频率*/
        if (strcmp(cmd_str,"get_freq") == 0) {
            rc = tea5767_get_freq();   
        }

        /*静音开关*/
        if (strcmp(cmd_str,"mute") == 0) {
            if (strcmp(param_str,"on") == 0) {
                rc = tea5767_mute_on();
            } else {
                rc = tea5767_mute_off();
            }
        }

        /*立体声*/
        if (strcmp(cmd_str,"stereo") == 0) {
            if (strcmp(param_str,"on") == 0) {
                rc = tea5767_stereo_on();
            } else {
                rc = tea5767_stereo_off();
            }
        }
        /*设置日志等级*/
        if (strcmp(cmd_str,"set_log_level") == 0) {
            if (param_str) {
                uint8_t level = atoi(param_str);
                rc = log_set_level(level);
            }
          
        }
           
        if (rc == -1) {
            log_error("excute error.\r\n");
        } else {
            log_debug("excute ok.\r\n");
        }

    }
}