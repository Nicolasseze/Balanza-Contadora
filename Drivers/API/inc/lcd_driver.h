/*
 * lcd_driver.h
 *
 *  Created on: Apr 12, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define LCD_LINEA1	0x00
#define LCD_LINEA2	0x40
#define LCD_LINEA3	0x14
#define LCD_LINEA4	0x54

typedef bool bool_t;

typedef enum {
	LCD_OK = 0x00U,
	LCD_ERROR = 0x01U,
} LCD_Status_t;

bool_t lcdInit(void);
void lcdClear(void);
void lcdSetCursor(uint8_t row, uint8_t col);
void lcdPrint(const char *str);
void lcdCommand(uint8_t cmd);
void lcdData(uint8_t data);

#endif // LCD_DRIVER_H

