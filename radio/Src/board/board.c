#include "board.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "i2c.h"

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
int iic_write(uint8_t addr,uint8_t *buff,uint8_t cnt)
{
    HAL_StatusTypeDef status;
    status =  HAL_I2C_Master_Transmit(&hi2c1,addr,buff,cnt,0xff);
    if (status != HAL_OK) {
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
int iic_read(uint8_t addr,uint8_t *buff,uint8_t cnt)
{
    HAL_StatusTypeDef status;
    status =  HAL_I2C_Master_Receive(&hi2c1,addr,buff,cnt,0xff);
    if (status != HAL_OK) {
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
uint32_t log_time()
{
    return osKernelSysTick();
}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_e_set(void)
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
void bsp_1602a_e_clr(void)
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
void bsp_1602a_rs_set(void)
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
void bsp_1602a_rs_clr(void)
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
void bsp_1602a_rw_set(void)
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
void bsp_1602a_rw_clr(void)
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
void bsp_1602a_write_8bits(uint8_t write)
{
    uint16_t pin_set = 0;
    uint16_t pin_clr = 0;
    uint16_t pin_all = D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

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
    HAL_GPIO_WritePin(D0_GPIO_Port,pin_set,GPIO_PIN_SET);   
    HAL_GPIO_WritePin(D0_GPIO_Port,pin_clr,GPIO_PIN_RESET);  

}

/*
* @brief 
* @param
* @param
* @return 
* @note
*/
uint8_t bsp_1602a_read_8bits(void)
{
    uint16_t read = 0;
    uint16_t pin_all = D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*拉高*/
    HAL_GPIO_WritePin(D0_GPIO_Port,pin_all,GPIO_PIN_SET);
 
    /*输入模式*/
    GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin 
                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /*一位一位读出*/
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D0_Pin) == GPIO_PIN_SET) {
        read |= 1 << 0;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D1_Pin) == GPIO_PIN_SET) {
        read |= 1 << 1;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D2_Pin) == GPIO_PIN_SET) {
        read |= 1 << 2;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D3_Pin) == GPIO_PIN_SET) {
        read |= 1 << 3;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D4_Pin) == GPIO_PIN_SET) {
        read |= 1 << 4;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D5_Pin) == GPIO_PIN_SET) {
        read |= 1 << 5;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D6_Pin) == GPIO_PIN_SET) {
        read |= 1 << 6;
    }
    if (HAL_GPIO_ReadPin(D0_GPIO_Port,D7_Pin) == GPIO_PIN_SET) {
        read |= 1 << 7;
    }

    return read;
}

