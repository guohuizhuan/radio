#include "stdint.h"
#include "lib1602a.h"

               
/*1602A  字符(共80个)      1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  不可见           */
/*                   --------------------------------------------------------               */
/*1602A LINE1 地址  |0x00 ...............................................0x0F|0x10...0x27   */
/*1602A LINE2 地址  |0x40 ...............................................0x4F|0x50...0x67   */
/*                   --------------------------------------------------------               */


static lib1602a_io_driver_t *driver;

/*clear screen icode*/
#define  LIB1602A_CLEAR_SCREEN_I_CODE                   (1 << 0)

/*return home icode*/
#define  LIB1602A_RETURN_HOME_I_CODE                    (1 << 1)


/*entry mode set icode*/
#define  LIB1602A_MODE_CTRL_I_CODE                      (1 << 2)
#define  LIB1602A_MODE_ADDR_DIR_BIT_POS                 1  
#define  LIB1602A_MODE_AUTO_SHIFT_SCREEN_BIT_POS        0

#define  LIB1602A_MODE_ADDR_DIR_INCREASE                1 
#define  LIB1602A_MODE_ADDR_DIR_DECREASE                0   

#define  LIB1602A_MODE_AUTO_SHIFT_SCREEN                1
#define  LIB1602A_MODE_NO_AUTO_SHIFT_SCREEN             0

/*display on off crl icode*/
#define  LIB1602A_DISPLAY_ON_OFF_CTRL_I_CODE           (1 << 3)
#define  LIB1602A_SCREEN_ON_OFF_CTRL_BIT_POS            2   
#define  LIB1602A_CURSOR_ON_OFF_CTRL_BIT_POS            1 
#define  LIB1602A_CURSOR_BLINK_CTRL_BIT_POS             0   

#define  LIB1602A_SCREEN_ON                             1
#define  LIB1602A_SCREEN_OFF                            0

#define  LIB1602A_CURSOR_ON                             1
#define  LIB1602A_CURSOR_OFF                            0

#define  LIB1602A_CURSOR_BLINK                          1
#define  LIB1602A_CURSOR_NO_BLINK                       0

/*cursor or display shift*/
#define  LIB1602A_CS_CTRL_I_CODE                       (1 << 4)
#define  LIB1602A_CS_SELECT_CTRL_BIT_POS                3   
#define  LIB1602A_CS_DIR_CTRL_BIT_POS                   2 

#define  LIB1602A_CS_SELECT_CURSOR                      0  
#define  LIB1602A_CS_SELECT_SCREEN_SHIFT                1   


#define  LIB1602A_CS_DIR_LEFT                           0
#define  LIB1602A_CS_DIR_RIGHT                          1


/*fucntion set*/
#define  LIB1602A_FUNCTION_CTRL_I_CODE                 (1 << 5)
#define  LIB1602A_FUNCTION_DATA_CTRL_BIT_POS            4   
#define  LIB1602A_FUNCTION_LINE_CTRL_BIT_POS            3 
#define  LIB1602A_FUNCTION_FONT_CTRL_BIT_POS            2

#define  LIB1602A_FUNCTION_DATA8                        1 
#define  LIB1602A_FUNCTION_DATA4                        0 

#define  LIB1602A_FUNCTION_LINE_2                       1
#define  LIB1602A_FUNCTION_LINE_1                       0   


#define  LIB1602A_FUNCTION_FONT_5X8                     0
#define  LIB1602A_FUNCTION_FONT_5X11                    1


/*set cgram address*/
#define  LIB1602A_CGRAM_ADDR_CTRL_I_CODE               (1 << 6)
#define  LIB1602A_CGRAM_ADDR_CTRL_MASK                 ((1 << 6) - 1)

/*set ddram address*/
#define  LIB1602A_DDRAM_ADDR_CTRL_I_CODE               (1 << 7)
#define  LIB1602A_DDRAM_ADDR_CTRL_MASK                 ((1 << 7) - 1)

/*每行最大字符数*/
#define  LIB1602A_CHARACTERS_CNT_MAX_PER_LINE          40
/*第一行最大字符地址*/
#define  LIB1602A_CHARACTERS_ADDR_MAX_LINE1            0x27
/*第二行最大字符地址*/
#define  LIB1602A_CHARACTERS_ADDR_MAX_LINE2            0x67





/*chip enbale*/
#define  LIB1602A_CHIP_ENABLE()                        driver->io_e_clr()
#define  LIB1602A_CHIP_DISABLE()                       driver->io_e_set()

/*read or write enable*/
#define  LIB1602A_WRITE_ENABLE()                       driver->io_rw_clr()
#define  LIB1602A_READ_ENABLE()                        driver->io_rw_set()

/*instruction or data */
#define  LIB1602A_SELECT_INSTRUCTION_REG()             driver->io_rs_clr()
#define  LIB1602A_SELECT_DATA_REG()                    driver->io_rs_set()


/*busy flag mask*/
#define  LIB1602A_BUSY_MASK                            (1 << 7)
/*cursor position flag mask*/
#define  LIB1602A_POSITION_MASK                        ((1 << 7) - 1)


/*读写*/
#define  LIB1602A_OUTPUT_DATA(x)                        driver->io_8bits_out(x)
#define  LIB1602A_INPUT_DATA()                          driver->io_8bits_in()

/*
* @brief 1602a写操作
* @param reg 寄存器类型
* @param reg_data 寄存器数据
* @return 无
* @note reg = 0 指令寄存器；reg = 1 数据寄存器
*/
static void lib1602a_write_reg(uint8_t reg,uint8_t reg_data)
{
    
    /*选择寄存器*/
    if (reg == 0) {
        driver->io_rs_clr();
    } else {
        driver->io_rs_set();
    }
    /*使能写*/
    driver->io_rw_clr();   
    
    /*等待*/
    driver->io_delay_ms(1);    
    /*芯片使能信号上升*/
    driver->io_e_set();  
    /*io输出数据*/
    driver->io_8bits_out(reg_data);
    /*等待*/
    driver->io_delay_ms(1);   
 
    /*芯片使能信号下降*/
    driver->io_e_clr();
    /*等待*/
    driver->io_delay_ms(1);   


}

/*
* @brief 1602a读操作
* @param reg 寄存器类型
* @return 读到的数据
* @note reg = 0 指令寄存器；reg = 1 数据寄存器
*/
static uint8_t lib1602a_read_reg(uint8_t reg)
{
    uint8_t read;
    
    /*选择寄存器*/
    if (reg == 0) {
        driver->io_rs_clr();
    } else {
        driver->io_rs_set();
    }
    /*使能读*/
    driver->io_rw_set();   
    
    /*等待*/
    driver->io_delay_ms(1);    
    /*芯片使能信号上升*/
    driver->io_e_set();
    /*等待*/
    driver->io_delay_ms(1);      
    /*io输入数据*/
    read = driver->io_8bits_in(); 
    /*芯片使能信号下降*/
    driver->io_e_clr();
    /*等待*/
    driver->io_delay_ms(1);  

    return read;
    
}


/*
* @brief 1602a模式设置 光标增长方向和是否自动移屏
* @param mode 模式指针
* @param 
* @return 无
* @note 
*/
static void lib1602a_set_mode(lib1602a_mode_t *mode)
{
    uint8_t icode;

    icode = LIB1602A_MODE_CTRL_I_CODE;

    if (mode->dir == LIB1602A_ADDR_DIR_INCREASE) {
	    icode |= (LIB1602A_ADDR_DIR_INCREASE << LIB1602A_MODE_ADDR_DIR_BIT_POS);
    } else {
	    icode |= (LIB1602A_ADDR_DIR_DECREASE << LIB1602A_MODE_ADDR_DIR_BIT_POS);
    }

    if (mode->auto_shift == LIB1602A_AUTO_SHIFT_SCREEN) {
        icode |= (LIB1602A_MODE_AUTO_SHIFT_SCREEN << LIB1602A_MODE_AUTO_SHIFT_SCREEN_BIT_POS);
    } else {
        icode |= (LIB1602A_MODE_NO_AUTO_SHIFT_SCREEN << LIB1602A_MODE_AUTO_SHIFT_SCREEN_BIT_POS);
    }

    lib1602a_write_reg(0,icode);

}

/*
* @brief 1602a设置默认功能属性
* @param 无
* @param 
* @return 无
* @note
*/
static void lib1602a_set_default_function(void)
{
    uint8_t icode;
 
    icode = LIB1602A_FUNCTION_CTRL_I_CODE;
    /*8位数据宽度 2行显示 5*8字体*/
    icode |= (LIB1602A_FUNCTION_DATA8 << LIB1602A_FUNCTION_DATA_CTRL_BIT_POS) | (LIB1602A_FUNCTION_LINE_2 << LIB1602A_FUNCTION_LINE_CTRL_BIT_POS) | (LIB1602A_FUNCTION_FONT_5X8 << LIB1602A_FUNCTION_FONT_CTRL_BIT_POS);
     lib1602a_write_reg(0,icode);
     
}


/*
* @brief 1602a设置显示功能
* @param 无
* @param 
* @return 无
* @note
*/
static void lib1602a_set_display(lib1602a_display_t *display)
{
    uint8_t icode;

    icode = LIB1602A_DISPLAY_ON_OFF_CTRL_I_CODE;
    
    /*屏幕显示*/
    if (display->screen_display == LIB1602A_SCREEN_DISPLAY_ON) {
        icode |= (LIB1602A_SCREEN_ON << LIB1602A_SCREEN_ON_OFF_CTRL_BIT_POS);
    } else {
        icode |= (LIB1602A_SCREEN_OFF << LIB1602A_SCREEN_ON_OFF_CTRL_BIT_POS);
    }
    /*光标显示*/
    if (display->cursor_display == LIB1602A_CURSOR_DISPLAY_ON) {
        icode |= (LIB1602A_CURSOR_ON << LIB1602A_CURSOR_ON_OFF_CTRL_BIT_POS);
    } else {
        icode |= (LIB1602A_CURSOR_OFF << LIB1602A_CURSOR_ON_OFF_CTRL_BIT_POS);
    }
    /*光标类型*/
    if (display->cursor_type == LIB1602A_CURSOR_TYPE_BLINK) {
        icode |= (LIB1602A_CURSOR_BLINK << LIB1602A_CURSOR_BLINK_CTRL_BIT_POS);
    } else {
        icode |= (LIB1602A_CURSOR_NO_BLINK << LIB1602A_CURSOR_BLINK_CTRL_BIT_POS);
    }
    
    lib1602a_write_reg(0,icode);

}

/*
* @brief 1602a注册io驱动
* @param io_driver 驱动指针
* @param
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_register_io_driver(lib1602a_io_driver_t *io_driver)
{
    if(io_driver == NULL) {
        return -1;
    }
 
    driver = io_driver;
    driver->is_registered = 1;
    return 0;
}


/*
* @brief 1602a是否忙
* @param 无
* @param
* @return 0：否 其他：是
* @note
*/
uint8_t lib1602a_is_busy(void)
{
    uint8_t busy;

    busy = lib1602a_read_reg(0);
    if (busy & LIB1602A_BUSY_MASK) {
        return 1;
    }

    return 0;
}


/*
* @brief 1602a配置参数
* @param 配置参数指针
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_config(lib1602a_config_t *config)
{
    if (config == NULL) {
        return -1;
    }

    if (!driver->is_registered) {
        return -1;
    }
    driver->io_delay_ms(15);
    lib1602a_set_default_function();
    driver->io_delay_ms(5);
    lib1602a_set_default_function();
    driver->io_delay_ms(5);
    lib1602a_set_default_function();
    driver->io_delay_ms(5);
    lib1602a_set_default_function();
    driver->io_delay_ms(1);
    lib1602a_set_mode(&config->mode);
    driver->io_delay_ms(1);
    lib1602a_set_display(&config->display);
    driver->io_delay_ms(1);

    return 0;
    
}


/*
* @brief 1602a屏幕左移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_scroll_screen_left(void)
{
    uint8_t icode;
 
    if (!driver->is_registered) {
        return -1;
    }
 
    icode = LIB1602A_CS_CTRL_I_CODE;
    icode |= (LIB1602A_CS_SELECT_SCREEN_SHIFT << LIB1602A_CS_SELECT_CTRL_BIT_POS) | (LIB1602A_CS_DIR_LEFT << LIB1602A_CS_DIR_CTRL_BIT_POS);
 
    lib1602a_write_reg(0,icode);
    
    return 0;
    
}

/*
* @brief 1602a屏幕右移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/

int lib1602a_scroll_screen_right(void)
{
    uint8_t icode;
    
    if (!driver->is_registered) {
        return -1;
    }
 
    icode = LIB1602A_CS_CTRL_I_CODE;
    icode |= (LIB1602A_CS_SELECT_SCREEN_SHIFT << LIB1602A_CS_SELECT_CTRL_BIT_POS) | (LIB1602A_CS_DIR_RIGHT << LIB1602A_CS_DIR_CTRL_BIT_POS);
 
    lib1602a_write_reg(0,icode);
    
    return 0;

}

/*
* @brief 1602a清屏
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_clear_screen(void)
{
    uint8_t icode;
 
    if (!driver->is_registered) {
        return -1;
    }
 
    icode = 0;
    icode |= LIB1602A_CLEAR_SCREEN_I_CODE;
 
    lib1602a_write_reg(0,icode);
    
    return 0;

}


/*
* @brief 1602a读取当前光标地址
* @param 无
* @param
* @return 光标地址
* @note
*/
uint8_t lib1602a_get_cursor_position(void)
{
    uint8_t pos;

    pos = lib1602a_read_reg(0);
    return pos & LIB1602A_POSITION_MASK;
    
}


/*
* @brief 1602a设置光标位置
* @param line 行号
* @param x 列号 
* @return 0：成功 -1：失败
* @note x最大值为LIB1602A_CHARACTERS_CNT_MAX_PER_LINE
*/
int lib1602a_set_cursor_position(lib1602a_pos_line_t line ,uint8_t x)
{
	uint8_t icode; 
	uint8_t ddram_addr;
	
    if (!driver->is_registered) {
        return -1;
    }

    if (x > (LIB1602A_CHARACTERS_CNT_MAX_PER_LINE - 1)) {
        x = LIB1602A_CHARACTERS_CNT_MAX_PER_LINE - 1;
    }
    
    if (line == LIB1602A_POS_LINE_1) {
	    ddram_addr = x;
	} else {
        ddram_addr = 0x40 + x ;
	}
	
	icode = LIB1602A_DDRAM_ADDR_CTRL_I_CODE;
	icode |= (ddram_addr & LIB1602A_DDRAM_ADDR_CTRL_MASK);
	
	lib1602a_write_reg(0,icode);
    
    return 0;
    
}


/*
* @brief 1602a光标左移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_left(void)
{
    uint8_t icode;
 
    if (!driver->is_registered) {
        return -1;
    }

    icode = LIB1602A_CS_CTRL_I_CODE;
	icode |= (LIB1602A_CS_SELECT_CURSOR << LIB1602A_CS_SELECT_CTRL_BIT_POS) | (LIB1602A_CS_DIR_LEFT << LIB1602A_CS_DIR_CTRL_BIT_POS);
	   
    lib1602a_write_reg(0,icode);
    
    return 0;

}

/*
* @brief 1602a光标右移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_right(void)
{
    uint8_t icode;
 
    if (!driver->is_registered) {
        return -1;
    }

    icode = LIB1602A_CS_CTRL_I_CODE;
	icode |= (LIB1602A_CS_SELECT_CURSOR << LIB1602A_CS_SELECT_CTRL_BIT_POS) | (LIB1602A_CS_DIR_RIGHT << LIB1602A_CS_DIR_CTRL_BIT_POS);
	   
    lib1602a_write_reg(0,icode);
    
    return 0;

}

/*
* @brief 1602a光标上移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_up(void)
{
    uint8_t pos;
    
    if (!driver->is_registered) {
        return -1;
    }

    pos = lib1602a_get_cursor_position();
    /*如果目前在第二行*/
    if (pos > LIB1602A_CHARACTERS_ADDR_MAX_LINE1) {
        pos -= LIB1602A_CHARACTERS_CNT_MAX_PER_LINE;
        lib1602a_set_cursor_position(LIB1602A_POS_LINE_1,pos);
    }
      
    return 0;   
}

/*
* @brief 1602a光标下移
* @param 无
* @param 
* @return 0：成功 -1：失败
* @note
*/
int lib1602a_move_cursor_down(void)
{
    uint8_t pos;
    
    if (!driver->is_registered) {
        return -1;
    }

    pos = lib1602a_get_cursor_position();
    /*如果目前在第一行*/
    if (pos <= LIB1602A_CHARACTERS_ADDR_MAX_LINE1) {
        lib1602a_set_cursor_position(LIB1602A_POS_LINE_2,pos);
    }
      
    return 0;  

}


/*
* @brief 1602a在指定位置显示字符串
* @param line 行号
* @param x 列号 
* @return 0：成功 -1：失败
* @note x最大值为LIB1602A_CHARACTERS_CNT_MAX_PER_LINE
*/
int lib1602a_display_string(lib1602a_pos_line_t line,uint8_t x,const char *str)
{
    if (str == NULL) {
        return -1;
    }
    
    if (!driver->is_registered) {
        return -1;
    }

	/*光标到指定位置*/
	lib1602a_set_cursor_position(line,x);

	while (*str != '\0')
	{
        lib1602a_write_reg(1,*str++);
	}
    
    return 0;
    
}





