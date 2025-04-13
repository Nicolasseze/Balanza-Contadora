/*
 * lcd_driver.c
 *
 *  Created on: Apr 12, 2025
 *      Author: nicolas-porco
 */

#include "lcd_driver.h"
#include "lcd_port.h"
#include "stm32f4xx_hal.h"

static void lcdSendCmd(uint8_t cmd) {
    lcdPortWrite(cmd, false);
    HAL_Delay(2);
}

static void lcdSendData(uint8_t data) {
    lcdPortWrite(data, true);
    HAL_Delay(2);
}

void lcdCommand(uint8_t cmd) {
    lcdSendCmd(cmd);
}

void lcdData(uint8_t data) {
    lcdSendData(data);
}

bool_t lcdInit(void) {

    lcdPortInit(); //Power on and delay

    lcdSendCmd(0x28); // 4-bit, 2 line, 5x8 dots
    lcdSendCmd(0x0C); // Display ON, Cursor OFF
    lcdSendCmd(0x06); // Entry mode
    lcdClear(); // Clear display
    HAL_Delay(2);

    return true;
}

void lcdClear(void) {
    lcdSendCmd(0x01);
    HAL_Delay(2);
}

void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t address[] = {0x00, 0x40, 0x14, 0x54};
    lcdSendCmd(0x80 | (address[row] + col));
}


void lcdPrint(const char *str) {

	assert_param( str != NULL );

    while (*str) {
        lcdSendData(*str++);
    }
}


