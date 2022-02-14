/*
 * 16x2-LCD.c
 *
 *  Created on: Feb 13, 2022
 *      Author: Alexander Stenlund
 */

#include "16x2-LCD.h"

// Transmits only 4 bits to lcd in write mode
uint8_t LCD_Transmit_W4Bit(I2C_HandleTypeDef *hI2C, uint8_t data) {
	HAL_StatusTypeDef ret = 0;
	data = data | LCD_E; // Sets E (Enable) pin high

	// Sends data with E (Enable) pin high
	ret += HAL_I2C_Master_Transmit(hI2C, LCD_ADDR_WRITE, &data, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	// Sends the same data but with E (Enable) pin low
	data = data & (~LCD_E);
	ret += HAL_I2C_Master_Transmit(hI2C, LCD_ADDR_WRITE, &data, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	return ret;
}

// Transmits command to LCD (in write mode) via I2C interface.
// data1: The higher order bits of data (4 bits)
// data2: The lower order bits of data (4 bits)
uint8_t LCD_Transmit(I2C_HandleTypeDef *hI2C, uint8_t data1, uint8_t data2) {
	return  LCD_Transmit_W4Bit(hI2C, data1) +
			LCD_Transmit_W4Bit(hI2C, data2);

}

uint8_t LCD_Transmit_Letter(I2C_HandleTypeDef *hI2C, char *ch) {
	uint8_t data1 = LCD_RS | LCD_RW | ( (*ch) >> 4 );
	uint8_t data2 = LCD_RS | LCD_RW | ( 0xF & (*ch) );

	return LCD_Transmit(hI2C, data1, data2);

}

uint8_t LCD_Transmit_String(I2C_HandleTypeDef *hI2C, char string[], uint8_t size) {

	for ( uint8_t i = 0; i < size; i++ ) {
		LCD_Transmit_Letter( hI2C, (char*) &(string[i]) );
	}
	return 0;
}

uint8_t LCD_Clear(I2C_HandleTypeDef *hI2C) {
	return LCD_Transmit(hI2C, 0x00, LCD_D4);
}

uint8_t LCD_Return_home(I2C_HandleTypeDef *hI2C) {
	return LCD_Transmit(hI2C, 0x00, LCD_D5);
}

/* Borde vara rätt...
 * cmd:
 * 		Display off: 	  LCD_D6
 * 		Cursor  off: 	  LCD_D5
 * 		Cursor blink off: LCD_D4
 */
uint8_t LCD_Display_Ctrl(I2C_HandleTypeDef *hI2C, uint8_t cmd) {
	return LCD_Transmit(hI2C, 0x00, LCD_D7 | cmd);
}

// Initialize/Reset the LCD into 4-bit mode
uint8_t LCD_Init(I2C_HandleTypeDef *hI2C) {
	HAL_StatusTypeDef ret = 0;

	HAL_Delay(100);
	// Sets transmission to 8-bits 3 times in case it is in 4-bit mode
	// Sets transmission to 8-bits
	ret += LCD_Transmit_W4Bit(hI2C, LCD_D5 + LCD_D4);

	HAL_Delay(10);
	// Sets transmission to 8-bits
	ret += LCD_Transmit_W4Bit(hI2C, LCD_D5 + LCD_D4);

	HAL_Delay(10);
	// Sets transmission to 8-bits
	ret += LCD_Transmit_W4Bit(hI2C, LCD_D5 + LCD_D4);

	// Sets transmission to 4-bits (Was 8-bit)
	ret += LCD_Transmit_W4Bit(hI2C, LCD_D5);

	// Sets char font to 5x8 and display to 1 line
	ret += LCD_Transmit(hI2C, LCD_D5, 0x00);

	// Display off
	ret += LCD_Transmit(hI2C, 0x00, LCD_D7);

	// Clear display
	ret += LCD_Transmit(hI2C, 0x00, LCD_D4);
	HAL_Delay(2); // LCD execution time: ~1.5ms

	// Sets mode to increment address and shift the cursor when writing to DD/CGRAM
	ret += LCD_Transmit(hI2C, 0x00, LCD_D5 + LCD_D6);

	// Return home
	ret += LCD_Transmit(hI2C, 0x00, LCD_D5);
	HAL_Delay(2); // LCD execution time: ~1.5ms

	// Turns on display and cursor
	ret += LCD_Transmit(hI2C, 0x00, LCD_D7 + LCD_D6 + LCD_D5);

	HAL_Delay(1000);

	// Turn on display and turn cursor off
	ret += LCD_Transmit(hI2C, 0x00, LCD_D7 + LCD_D6);

	return ret;
}
