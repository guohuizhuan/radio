#ifndef __LIB1602A_H__
#define __LIB1602A_H__

#ifdef   __cplusplus
#define  LIB1602A_BEGIN        extern "C" {
#define  LIB1602A_END          }
#else
#define  LIB1602A_BEGIN      
#define  LIB1602A_END       
#endif

#ifndef  NULL 
#define  NULL             (void*)0
#endif

LIB1602A_BEGIN


typedef struct
{
    uint8_t is_registered;
    void (*io_e_set)(void);
    void (*io_e_clr)(void);
    void (*io_rs_set)(void);
    void (*io_rs_clr)(void);
    void (*io_rw_set)(void);
    void (*io_rw_clr)(void);
    uint8_t (*io_8bits_in)(void);
    void (*io_8bits_out)(uint8_t);
    void (*io_delay_ms)(uint8_t);
}lib1602a_io_driver_t;


typedef enum
{
    LIB1602A_ADDR_DIR_INCREASE,
    LIB1602A_ADDR_DIR_DECREASE
}lib1602a_addr_dir_t;

typedef enum
{
    /*写入一个数据后，屏幕自动左移或者右移一个字符宽度*/
    LIB1602A_AUTO_SHIFT_SCREEN,
    /*写入一个数据后，屏幕不自动移动*/
    LIB1602A_NO_AUTO_SHIFT_SCREEN	
}lib1602a_auto_shift_screen_t;


typedef struct 
{
    lib1602a_addr_dir_t  dir;
    lib1602a_auto_shift_screen_t  auto_shift;
}lib1602a_mode_t;

  
typedef enum
{
    LIB1602A_SCREEN_DISPLAY_ON,
    LIB1602A_SCREEN_DISPLAY_OFF
}lib1602a_screen_display_t;



typedef enum
{
    LIB1602A_CURSOR_DISPLAY_ON,
    LIB1602A_CURSOR_DISPLAY_OFF
}lib1602a_cursor_display_t;


typedef enum
{
    LIB1602A_CURSOR_TYPE_NO_BLINK,
    LIB1602A_CURSOR_TYPE_BLINK
}lib1602a_cursor_type_t;

typedef struct
{
    lib1602a_screen_display_t screen_display;
    lib1602a_cursor_display_t cursor_display;
    lib1602a_cursor_type_t cursor_type;
}lib1602a_display_t;



typedef struct
{
    lib1602a_mode_t mode;
    lib1602a_display_t display;
}lib1602a_config_t;

typedef enum
{
    LIB1602A_POS_LINE_1,
    LIB1602A_POS_LINE_2	
}lib1602a_pos_line_t;  

/*
* @brief 1602a注册io驱动
* @param io_driver 驱动指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_register_io_driver(lib1602a_io_driver_t *io_driver);

/*
* @brief 1602a是否忙
* @param 无
* @param
* @return 0：否 其他：是
* @note
*/
uint8_t lib1602a_is_busy(void);

/*
* @brief 1602a配置参数
* @param 配置参数指针
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_config(lib1602a_config_t *config);

/*
* @brief 1602a屏幕左移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_scroll_screen_left(void);

/*
* @brief 1602a屏幕右移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/

int lib1602a_scroll_screen_right(void);

/*
* @brief 1602a清屏
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_clear_screen(void);

/*
* @brief 1602a读取当前光标地址
* @param 无
* @param
* @return 光标地址
* @note
*/
uint8_t lib1602a_get_cursor_position(void);

/*
* @brief 1602a设置光标位置
* @param line 行号
* @param x 列号 
* @return 0：成功 -1：失败
* @note x最大值为LIB1602A_CHARACTERS_CNT_MAX_PER_LINE
*/
int lib1602a_set_cursor_position(lib1602a_pos_line_t line ,uint8_t x);

/*
* @brief 1602a光标左移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_left(void);

/*
* @brief 1602a光标右移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_right(void);

/*
* @brief 1602a光标上移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_up(void);

/*
* @brief 1602a光标下移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_down(void);

/*
* @brief 1602a在指定位置显示字符串
* @param line 行号
* @param x 列号 
* @return 0：成功 -1：失败
* @note x最大值为LIB1602A_CHARACTERS_CNT_MAX_PER_LINE
*/
int lib1602a_display_string(lib1602a_pos_line_t line,uint8_t x,const char *str);









LIB1602A_END




#endif









