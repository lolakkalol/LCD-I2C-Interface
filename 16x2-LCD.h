/*
 * 16x2-LCD.h
 *
 *  Created on: Feb 13, 2022
 *      Author: Alexander Stenlund
 */

#ifndef INC_16X2_LCD_H_
#define INC_16X2_LCD_H_
#include "stdint.h"
#include "stm32l4xx_hal.h"

enum LCD_PIN_VALUES{LCD_RS  = 0x01,
					LCD_RW  = 0x02,
					LCD_E   = 0x04,
					LCD_VDD = 0x08,
					LCD_D4  = 0x10,
					LCD_D5  = 0x20,
					LCD_D6  = 0x40,
					LCD_D7  = 0x80};

enum LCD_ADRESS{LCD_ADDR_WRITE = 0x4E,
				LCD_ADDR_READ  = 0x4F};

uint8_t LCD_Init();
uint8_t LCD_Transmit(I2C_HandleTypeDef *hI2C, uint8_t data1, uint8_t data2);


#endif /* INC_16X2_LCD_H_ */
