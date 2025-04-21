/**
 * @file keypad_driver.c
 * @brief Implementación de la lógica de escaneo de un teclado matricial genérico.
 *
 * Este módulo implementa las funciones de alto nivel para inicializar y escanear
 * un teclado matricial utilizando una capa de abstracción de hardware (`keypad_port`).
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

/**
 * @brief Mapa de caracteres del teclado matricial.
 *
 * Esta variable debe ser definida en la capa de aplicación. Representa el
 * contenido visible de cada tecla (por ejemplo: '1', '2', '3', ..., 'A', '*', etc.).
 * El índice `[fila][columna]` debe coincidir con la disposición física del teclado.
 */
extern const char keypadKeymap[KEYPAD_ROWS][KEYPAD_COLS];

/**
 * @brief Inicializa el teclado matricial.
 *
 * Asocia los pines físicos a las filas y columnas del teclado utilizando la capa `keypad_port`.
 * Esta función debe ser llamada una vez antes de usar cualquier otra función del driver.
 */
void keypadInit(void) {
    // Asignación de pines (esto puede moverse a una configuración externa si se desea mayor portabilidad)
    keypadPortAttachRow(0, GPIOC, GPIO_PIN_7);
    keypadPortAttachRow(1, GPIOA, GPIO_PIN_9);
    keypadPortAttachRow(2, GPIOA, GPIO_PIN_8);
    keypadPortAttachRow(3, GPIOB, GPIO_PIN_10);

    keypadPortAttachCol(0, GPIOB, GPIO_PIN_4);
    keypadPortAttachCol(1, GPIOB, GPIO_PIN_5);
    keypadPortAttachCol(2, GPIOB, GPIO_PIN_3);
    keypadPortAttachCol(3, GPIOA, GPIO_PIN_10);

    keypadPortInit(KEYPAD_ROWS, KEYPAD_COLS);
}

/**
 * @brief Lee el estado de una tecla específica del teclado matricial.
 *
 * Esta función activa la fila indicada, lee el estado de la columna correspondiente,
 * y luego desactiva la fila. Se recomienda para verificar el estado puntual de una tecla.
 *
 * @param row Índice de la fila (0 a KEYPAD_ROWS - 1).
 * @param col Índice de la columna (0 a KEYPAD_COLS - 1).
 * @retval true Si la tecla está presionada.
 * @retval false Si la tecla no está presionada.
 */
bool_t keypadReadKey(uint8_t row, uint8_t col){
    bool_t btn;

    keypadPortSetRow(row, true);
    btn = keypadPortReadCol(col);
    keypadPortSetRow(row, false);

    return btn;
}

/**
 * @brief Detecta la primera tecla presionada en el teclado matricial.
 *
 * Escanea el teclado fila por fila y columna por columna. Si detecta una tecla presionada,
 * guarda el carácter correspondiente en el puntero `key` y finaliza el escaneo.
 *
 * @param[out] key Puntero donde se almacenará el carácter de la tecla presionada.
 * @retval true Si se detectó alguna tecla presionada.
 * @retval false Si no se detectó ninguna tecla.
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
 * @brief Escanea todo el teclado y detecta múltiples teclas presionadas simultáneamente.
 *
 * Esta función recorre todas las filas y columnas, y actualiza la matriz `keyStates`
 * con el estado de cada tecla (presionada o no).
 *
 * @param[out] keyStates Matriz de tamaño `[KEYPAD_ROWS][KEYPAD_COLS]` donde:
 *                       - `true` indica que la tecla está presionada.
 *                       - `false` indica que la tecla no está presionada.
 * @retval true Si se detectó al menos una tecla presionada.
 * @retval false Si no hay ninguna tecla presionada.
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
