/*
 * keypad_driver.h
 *
 *  Created on: Apr 15, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_KEYPAD_DRIVER_H_
#define API_INC_KEYPAD_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// Devuelve true si se presionó una tecla, y la guarda en `key`
bool keypadGetKey(char* key);

// Inicializa los GPIO del keypad (llama a keypadPortInit internamente)
void keypadInit(void);


#endif /* API_INC_KEYPAD_DRIVER_H_ */
