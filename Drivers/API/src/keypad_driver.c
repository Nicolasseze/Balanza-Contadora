/**
 * @file keypad_driver.c
 * @brief Implementación de la lógica de escaneo de un teclado matricial genérico.
 *
 * Este módulo implementa las funciones de alto nivel para inicializar y escanear
 * un teclado matricial utilizando una capa de abstracción de hardware (keypad_port).
 * Soporta la detección de una tecla presionada (`keypadGetKey`) y de múltiples teclas simultáneas (`keypadScanAll`).
 *
 * El mapa de teclas (`keypadKeymap`) debe ser definido externamente por el usuario
 * en la capa de aplicación, de acuerdo a la cantidad de filas y columnas configuradas.
 */

#include "keypad_driver.h"
#include "keypad_port.h"

#ifndef KEYPAD_ROWS
#error "Debes definir KEYPAD_ROWS antes de compilar"
#endif

#ifndef KEYPAD_COLS
#error "Debes definir KEYPAD_COLS antes de compilar"
#endif

extern const char keypadKeymap[KEYPAD_ROWS][KEYPAD_COLS];

/**
 * @brief Inicializa el teclado matricial.
 *
 * Configura los pines del teclado a través del módulo `keypad_port`.
 * Debe ser llamada una vez antes de usar cualquier otra función del driver.
 */
void keypadInit(void) {
    keypadPortInit(KEYPAD_ROWS, KEYPAD_COLS);
}

/**
 * @brief Detecta la primera tecla presionada.
 *
 * Recorre la matriz activando cada fila y leyendo todas las columnas. Si detecta
 * una tecla presionada, la devuelve en el parámetro `key` y finaliza el escaneo.
 *
 * @param[out] key Puntero donde se guarda el carácter correspondiente a la tecla presionada.
 * @return true si se detectó una tecla, false si no hay ninguna presionada.
 */
bool_t keypadGetKey(char* key) {
    for (uint8_t row = 0; row < KEYPAD_ROWS; row++) {
        keypadPortSetRow(row, true);

        for (uint8_t col = 0; col < KEYPAD_COLS; col++) {
            if (keypadPortReadCol(col)) {
                *key = keypadKeymap[row][col];
                keypadPortSetRow(row, false);
                return true;
            }
        }

        keypadPortSetRow(row, false);
    }

    return false;
}

/**
 * @brief Escanea todo el teclado y detecta múltiples teclas presionadas.
 *
 * Recorre todas las filas y columnas del teclado y guarda el estado
 * de cada tecla en la matriz booleana proporcionada.
 *
 * @param[out] keyStates Matriz [KEYPAD_ROWS][KEYPAD_COLS] donde se almacena
 *                       `true` si la tecla está presionada, `false` en caso contrario.
 * @return true si al menos una tecla está presionada, false si ninguna lo está.
 */
bool_t keypadScanAll(bool_t keyStates[KEYPAD_ROWS][KEYPAD_COLS]) {
    bool_t anyPressed = false;

    for (uint8_t row = 0; row < KEYPAD_ROWS; row++) {
        keypadPortSetRow(row, true);

        for (uint8_t col = 0; col < KEYPAD_COLS; col++) {
            bool_t pressed = keypadPortReadCol(col);
            keyStates[row][col] = pressed;

            if (pressed) {
                anyPressed = true;
            }
        }

        keypadPortSetRow(row, false);
    }

    return anyPressed;
}
