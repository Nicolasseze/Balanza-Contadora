/*
 * lcd_port.h
 *
 *  Created on: Apr 12, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_LCD_PORT_H_
#define API_INC_LCD_PORT_H_

#include <stdint.h>
#include <stdbool.h>

//Dirreccion I2C LCD
#define LCD_ADDRESS 	0x40U

//Definicion Bits en I2C
#define LCD_EN        	0x04U
#define LCD_RW        	0x02U
#define LCD_RS_CMD    	0x00U
#define LCD_RS_TXT    	0x01U
#define LCD_BACKLIGHT 	0x80U

//Manejo Nibbles
#define LCD_HIGH_NIBBLE_MASK	0xF0
#define LCD_LOW_NIBBLE_MASK		0x0F
#define LCD_HIGH_NIBBLE_SHIFT	1
#define LCD_LOW_NIBBLE_SHIFT	3

typedef bool bool_t;

extern void Error_Handler(void);

void lcdPortInit ( void );

bool_t lcdPortWrite ( uint8_t data, bool_t rs );

#endif /* API_INC_LCD_PORT_H_ */
