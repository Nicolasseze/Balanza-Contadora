/**
 * @file keypad_port.h
 * @brief Interfaz de bajo nivel para acceso al hardware de un teclado matricial.
 *
 * Esta capa se encarga de configurar los pines de filas y columnas del teclado,
 * y de abstraer la manipulación de los GPIO para que la lógica del driver
 * de escaneo sea independiente del hardware específico.
 *
 * La arquitectura está pensada para ser dinámica: se pueden registrar los pines
 * de filas y columnas con funciones `Attach`, y luego inicializar todo con `Init`.
 *
 * Las filas deben configurarse como salidas open-drain, y las columnas como
 * entradas con pull-up, para evitar conflictos eléctricos al presionar múltiples teclas.
 *
 * Al presionarse una tecla se entiende como "true" en la columna aunque el nivel logico
 * sea bajo.
 */
#ifndef API_INC_KEYPAD_PORT_H_
#define API_INC_KEYPAD_PORT_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define MAX_ROW_PORT	4
#define MAX_COL_PORT	4

typedef bool bool_t;

/**
 * @brief Asocia un pin de fila con su índice correspondiente.
 *
 * Esta función debe llamarse antes de `keypadPortInit()`.
 *
 * @param index Índice de la fila (0, 1, ..., N-1)
 * @param port  Puerto GPIO (por ejemplo, GPIOA, GPIOB, etc.)
 * @param pin   Pin GPIO (por ejemplo, GPIO_PIN_0, GPIO_PIN_1, etc.)
 */
void keypadPortAttachRow(uint8_t index, GPIO_TypeDef* port, uint16_t pin);

/**
 * @brief Asocia un pin de columna con su índice correspondiente.
 *
 * Esta función debe llamarse antes de `keypadPortInit()`.
 *
 * @param index Índice de la columna (0, 1, ..., M-1)
 * @param port  Puerto GPIO (por ejemplo, GPIOA, GPIOB, etc.)
 * @param pin   Pin GPIO (por ejemplo, GPIO_PIN_4, GPIO_PIN_5, etc.)
 */
void keypadPortAttachCol(uint8_t index, GPIO_TypeDef* port, uint16_t pin);

/**
 * @brief Inicializa los pines de filas y columnas del teclado.
 *
 * Esta función configura los pines previamente registrados como:
 * - Filas: salida open-drain, sin pull-up interno.
 * - Columnas: entrada con pull-up interno.
 *
 * @param row_count Número total de filas registradas.
 * @param col_count Número total de columnas registradas.
 */
void keypadPortInit(uint8_t row_count, uint8_t col_count);

/**
 * @brief Activa o desactiva eléctricamente una fila.
 *
 * @param row   Índice de la fila a modificar.
 * @param state true para activarla (LOW), false para desactivarla (HIGH-Z).
 */
void keypadPortSetRow(uint8_t row, bool_t state);

/**
 * @brief Lee el estado lógico de una columna del teclado.
 *
 * @param col Índice de la columna a leer (debe ser menor que el número de columnas configuradas).
 * @return true si la columna está en LOW (tecla presionada), false si está en HIGH (ninguna tecla presionada).
 */
bool_t keypadPortReadCol(uint8_t col);

#endif

