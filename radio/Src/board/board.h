#ifndef  __BOARD_H__
#define  __BOARD_H__
#include "stdint.h"
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_e_set(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_e_clr(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rs_set(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rs_clr(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rw_set(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_rw_clr(void);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
void bsp_1602a_write_8bits(uint8_t write);
/*
* @brief 
* @param
* @param
* @return 
* @note
*/
uint8_t bsp_1602a_read_8bits(void);







#endif