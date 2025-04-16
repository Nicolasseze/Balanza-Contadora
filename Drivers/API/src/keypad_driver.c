/*
 * keypad_driver.c
 *
 *  Created on: Apr 15, 2025
 *      Author: nicolas-porco
 */


#include "keypad_driver.h"
#include "keypad_port.h"

static const char keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypadInit(void) {
    keypadPortInit();
}

bool keypadGetKey(char* key) {
    for (uint8_t row = 0; row < KEYPAD_ROWS; row++) {
        keypadPortSetRow(row, true);

        for (uint8_t col = 0; col < KEYPAD_COLS; col++) {
            if (keypadPortReadCol(col)) {
                *key = keymap[row][col];
                keypadPortSetRow(row, false);
                return true;
            }
        }

        keypadPortSetRow(row, false);
    }

    return false;
}
