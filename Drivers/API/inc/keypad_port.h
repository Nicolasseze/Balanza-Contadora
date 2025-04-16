/*
 * keypad_port.h
 *
 *  Created on: Apr 15, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_KEYPAD_PORT_H_
#define API_INC_KEYPAD_PORT_H_


#include <stdint.h>
#include <stdbool.h>

void keypadPortInit(void);
void keypadPortSetRow(uint8_t row, bool state); // true: LOW, false: HIGH-Z
bool keypadPortReadCol(uint8_t col); // true si está presionado (LOW)

#endif /* API_INC_KEYPAD_PORT_H_ */
