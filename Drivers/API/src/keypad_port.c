/**
 * @file keypad_port.c
 * @brief Implementación del acceso a hardware para teclado matricial.
 *
 * Este módulo permite configurar e interactuar con los pines GPIO conectados
 * a un teclado matricial de forma dinámica. Las filas se configuran como salidas
 * open-drain, y las columnas como entradas con pull-up.
 */

#include "keypad_port.h"

/**
 * @brief Estructura interna para almacenar la configuración del hardware del teclado.
 */
typedef struct {
    GPIO_TypeDef* row_ports[MAX_ROW_PORT];  /**< Array de punteros a puertos de filas */
    uint16_t row_pins[MAX_ROW_PORT];        /**< Array de pines de filas */
    GPIO_TypeDef* col_ports[MAX_COL_PORT];  /**< Array de punteros a puertos de columnas */
    uint16_t col_pins[MAX_COL_PORT];        /**< Array de pines de columnas */
    uint8_t rows;                /**< Cantidad de filas registradas */
    uint8_t cols;                /**< Cantidad de columnas registradas */
    bool_t initialized;          /**< Bandera de inicialización */
} KeypadHardware_t;

/**
 * @brief Instancia estática de la configuración de pines del teclado.
 */
static KeypadHardware_t hw = {0};

void keypadPortAttachRow(uint8_t index, GPIO_TypeDef* port, uint16_t pin) {
	assert_param(index < MAX_ROW_PORT);
	assert_param(port != NULL);

    hw.row_ports[index] = port;
    hw.row_pins[index] = pin;
}

void keypadPortAttachCol(uint8_t index, GPIO_TypeDef* port, uint16_t pin) {
	assert_param(index < MAX_COL_PORT);
	assert_param(port != NULL);

    hw.col_ports[index] = port;
    hw.col_pins[index] = pin;
}

void keypadPortInit(uint8_t row_count, uint8_t col_count) {
	assert_param(row_count <= MAX_ROW_PORT);
	assert_param(col_count < MAX_COL_PORT);

    hw.rows = row_count;
    hw.cols = col_count;
    hw.initialized = true;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configuración de filas: salida open-drain, sin pull-up
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (uint8_t i = 0; i < hw.rows; i++) {
    	GPIO_InitStruct.Pin = hw.row_pins[i];
        HAL_GPIO_WritePin(hw.row_ports[i], hw.row_pins[i], GPIO_PIN_SET); // high-Z (no activo)
        HAL_GPIO_Init(hw.row_ports[i], &GPIO_InitStruct);
    }

    // Configuración de columnas: entrada con pull-up
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    for (uint8_t i = 0; i < hw.cols; i++) {
    	GPIO_InitStruct.Pin = hw.col_pins[i];
        HAL_GPIO_Init(hw.col_ports[i], &GPIO_InitStruct);
    }
}

void keypadPortSetRow(uint8_t row, bool_t state) {
    if (!hw.initialized) return;

    // true = activo (LOW), false = inactivo (high-Z)
    HAL_GPIO_WritePin(hw.row_ports[row], hw.row_pins[row], state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool_t keypadPortReadCol(uint8_t col) {
    if (!hw.initialized) return false;

    // Detecta si hay LOW en la columna (tecla presionada)
    return (HAL_GPIO_ReadPin(hw.col_ports[col], hw.col_pins[col]) == GPIO_PIN_RESET);
}
