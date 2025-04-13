/*
 * lcd_port.c
 *
 *  Created on: Apr 12, 2025
 *      Author: nicolas-porco
 */
#include "lcd_port.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

static void lcdStrobe(uint8_t data) {
    uint8_t buf = data | LCD_EN | LCD_BACKLIGHT;
    if( HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &buf, 1, HAL_MAX_DELAY) != HAL_OK )
    	Error_Handler();

    HAL_Delay(1);
    buf = (data & ~LCD_EN) | LCD_BACKLIGHT;
    if ( HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &buf, 1, HAL_MAX_DELAY) != HAL_OK )
    	Error_Handler();

    HAL_Delay(1);
}

void lcdPortInit(void) {
	HAL_Delay(50); // >15 ms desde power-up

	// Fase de inicialización especial (mandar nibble alto directamente 3 veces)
	lcdStrobe(0x30 >> 1); HAL_Delay(5);
	lcdStrobe(0x30 >> 1); HAL_Delay(5);
	lcdStrobe(0x30 >> 1); HAL_Delay(5);
	lcdStrobe(0x20 >> 1); HAL_Delay(5); // Habilita modo 4 bits
}


bool_t lcdPortWrite(uint8_t data, bool_t rs) {

    uint8_t upper = ( data & LCD_HIGH_NIBBLE_MASK ) >> LCD_HIGH_NIBBLE_SHIFT;

    uint8_t lower = (data & LCD_LOW_NIBBLE_MASK ) << LCD_LOW_NIBBLE_SHIFT;

    uint8_t control = rs ? LCD_RS_TXT : LCD_RS_CMD;

    // Transmit upper nibble
    lcdStrobe(upper | control);
    // Transmit lower nibble
    lcdStrobe(lower | control);

    return true;
}

