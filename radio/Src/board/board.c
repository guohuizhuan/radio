#include "board.h"
#include "gpio.h"

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_e_set()
{
    HAL_GPIO_WritePin(E_GPIO_Port,E_Pin,GPIO_PIN_SET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_e_clr()
{
    HAL_GPIO_WritePin(E_GPIO_Port,E_Pin,GPIO_PIN_RESET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rs_set()
{
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_SET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rs_clr()
{
    HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rw_set()
{
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rw_clr()
{
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);
}
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_read_8bits()
{
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET);
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_write_8bits(uint8_t write)
{
    uint16_t pin_set = 0;
    uint16_t pin_clr = 0;
    uint16_t pin_all = D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin;

    /*Configure GPIO pins : PAPin PAPin PAPin PAPin 
                            PAPin PAPin PAPin */
    GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin 
                          |D4_Pin|D5_Pin|D6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    if (write & (1 << 0)) {
        pin_set |= D0_Pin;
    }
    if (write & (1 << 1)) {
        pin_set |= D1_Pin;
    }
    if (write & (1 << 2)) {
        pin_set |= D2_Pin;
    }
    if (write & (1 << 3)) {
        pin_set |= D3_Pin;
    }
    if (write & (1 << 4)) {
        pin_set |= D4_Pin;
    }
    if (write & (1 << 5)) {
        pin_set |= D5_Pin;
    }   
    if (write & (1 << 6)) {
        pin_set |= D6_Pin;
    }
    if (write & (1 << 7)) {
        pin_set |= D7_Pin;
    }    
    
    pin_clr = pin_all ^ pin_set;
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET);   
    HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET);  

}