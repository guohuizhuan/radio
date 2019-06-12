#include "cmsis_os.h"
#include "display_task.h"
#include "tasks_init.h"
#include "radio_task.h"
#include "lib1602a.h"
#include "stdio.h"
#include "board.h"
#include "log.h"

osThreadId display_task_hdl;

#define  BUFFER_SIZE  20


void display_io_delay_ms(uint8_t ms)
{
    osDelay(ms);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void display_task(void const * argument)
{
    int rc;
    display_message_t display_msg;
    lib1602a_config_t display_config;
    lib1602a_io_driver_t display_driver;
    char buffer[BUFFER_SIZE];

    /*注册显示驱动*/
    display_driver.io_e_set = bsp_1602a_e_set;
    display_driver.io_e_clr = bsp_1602a_e_clr;
    display_driver.io_rs_set = bsp_1602a_rs_set;
    display_driver.io_rs_clr = bsp_1602a_rs_clr;
    display_driver.io_rw_set = bsp_1602a_rw_set;
    display_driver.io_rw_clr = bsp_1602a_rw_clr;
    display_driver.io_8bits_in = bsp_1602a_read_8bits;
    display_driver.io_8bits_out = bsp_1602a_write_8bits;
    display_driver.io_delay_ms = display_io_delay_ms;
    
    rc = lib1602a_register_io_driver(&display_driver);
    log_assert(rc == 0);

    /*初始化配置参数*/
    display_config.mode.dir = LIB1602A_ADDR_DIR_INCREASE;
    display_config.mode.auto_shift = LIB1602A_NO_AUTO_SHIFT_SCREEN;
    display_config.display.screen_display = LIB1602A_SCREEN_DISPLAY_ON;
    display_config.display.cursor_display = LIB1602A_CURSOR_DISPLAY_ON;
    display_config.display.cursor_type = LIB1602A_CURSOR_TYPE_BLINK;


    /*显示初始化*/
    rc = lib1602a_config(&display_config);
    log_assert(rc == 0);

    /*显示温度框架*/
    lib1602a_display_string(DISPLAY_TEMPERATURE_HEAD_X,DISPLAY_TEMPERATURE_Y,DISPLAY_TEMPERATURE_HEAD_STR);    
    lib1602a_display_string(DISPLAY_TEMPERATURE_TAIL_X,DISPLAY_TEMPERATURE_Y,DISPLAY_TEMPERATURE_TAIL_STR); 
    /*显示信号质量框架*/
    lib1602a_display_string(DISPLAY_LEVEL_HEAD_X,DISPLAY_LEVEL_Y,DISPLAY_LEVEL_HEAD_STR);    

    /*显示FM频率框架*/
    lib1602a_display_string(DISPLAY_FM_FREQ_HEAD_X,DISPLAY_FM_FREQ_Y,DISPLAY_FM_FREQ_HEAD_STR);    
    lib1602a_display_string(DISPLAY_FM_FREQ_TAIL_X,DISPLAY_FM_FREQ_Y,DISPLAY_FM_FREQ_TAIL_STR); 

    while (1) 
    {
        /*等待消息*/
        if (xQueueReceive(display_msg_q_hdl,&display_msg,DISPLAY_TASK_MSG_WAIT_TIMEOUT)) {

            /*频率显示消息*/
            if (display_msg.id == FM_FREQ_DISPLAY_MSG) {
                snprintf(buffer,BUFFER_SIZE,"%.1f",display_msg.fm_freq);
                lib1602a_display_string(DISPLAY_FM_FREQ_Y,DISPLAY_FM_FREQ_VALUE_X,buffer);
            }
            /*信号质量显示消息*/
            if (display_msg.id == SIGNAL_LEVEL_DISPLAY_MSG) {
                snprintf(buffer,BUFFER_SIZE,"%2d",display_msg.signal_level);
                lib1602a_display_string(DISPLAY_LEVEL_VALUE_X,DISPLAY_LEVEL_Y,buffer);
            }
            /*温度显示消息*/
            if (display_msg.id == TEMPERATURE_DISPLAY_MSG) {
                snprintf(buffer,BUFFER_SIZE,"%.1f",display_msg.temperature);
                lib1602a_display_string(DISPLAY_TEMPERATURE_VALUE_X,DISPLAY_TEMPERATURE_Y,buffer);

            }

            /*其他消息*/
            /*
            if (display_msg.id == TEMPERATURE_DISPLAY_MSG) {

            }
            */
        }
    }


}