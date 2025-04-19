/**
 * @file keypad_driver.h
 * @brief Módulo de alto nivel para manejo de teclados matriciales.
 *
 * Este driver permite inicializar, escanear y leer teclas presionadas
 * de una matriz de teclado, utilizando una capa de acceso al hardware (keypad_port).
 * Soporta detección de múltiples teclas mediante `keypadScanAll`, y detección simple
 * de una sola tecla mediante `keypadGetKey`.
 *
 * El mapa de teclas (`keypadKeymap`) debe ser definido por la aplicación, y debe
 * coincidir con las dimensiones definidas por `KEYPAD_ROWS` y `KEYPAD_COLS`.
 *
 * Ejemplo de uso:
 * @code
 *  const char keypadKeymap[KEYPAD_ROWS][KEYPAD_COLS] = {
 *      {'1','2','3','A'},
 *      {'4','5','6','B'},
 *      {'7','8','9','C'},
 *      {'*','0','#','D'}
 *  };
 *
 *  void main(void) {
 *      keypadInit();
 *      char key;
 *      if (keypadGetKey(&key)) {
 *          // hacer algo con key
 *      }
 *  }
 * @endcode
 */

#ifndef API_INC_KEYPAD_DRIVER_H_
#define API_INC_KEYPAD_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define DELAY_MS_

typedef bool bool_t;

/**
 * @brief Inicializa el teclado matricial.
 *
 * Configura internamente la capa de puerto para manejar los pines definidos por la aplicación.
 * Debe llamarse una única vez antes de usar las demás funciones del driver.
 */
void keypadInit(void);

/**
 * @brief Lee el estado de una única tecla del teclado matricial.
 *
 * Activa la fila indicada, lee la columna correspondiente, y luego
 * desactiva nuevamente la fila. Útil para detectar el estado puntual
 * de una tecla específica.
 *
 * @param row Índice de la fila (0 a KEYPAD_ROWS - 1)
 * @param col Índice de la columna (0 a KEYPAD_COLS - 1)
 * @return true si la tecla está presionada, false si no.
 */
bool_t keypadReadKey(uint8_t row, uint8_t col);

/**
 * @brief Escanea el teclado y detecta si hay una tecla presionada.
 *
 * Esta función recorre todas las filas activando una a la vez y leyendo
 * las columnas. Si encuentra una tecla presionada, detiene el escaneo y
 * la devuelve a través del parámetro `key`.
 *
 * @param[out] key Puntero a variable donde se guarda la tecla leída.
 * @return true si se detectó una tecla, false si no hay ninguna presionada.
 */
bool_t keypadGetKey(char* key);

/**
 * @brief Escanea todas las teclas del teclado matricial.
 *
 * Esta función recorre la matriz completa, activando cada fila y leyendo
 * todas las columnas. Llena una matriz booleana donde cada celda representa
 * si la tecla correspondiente está presionada o no.
 *
 * @param[out] keyStates Matriz de [KEYPAD_ROWS][KEYPAD_COLS] donde se guardan los estados.
 *                       Cada elemento es `true` si la tecla está presionada, `false` si no.
 * @return true si al menos una tecla está presionada, false si no hay ninguna.
 */
bool_t keypadScanAll(bool_t keyStates[KEYPAD_ROWS][KEYPAD_COLS]);

#endif /* API_INC_KEYPAD_DRIVER_H_ */

