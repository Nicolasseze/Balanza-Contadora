/**
 * @file API_delay.h
 * @brief Módulo para manejo de retardos no bloqueantes.
 * @author Nicolás
 * @date 2025
 * @details Este módulo permite la gestión de retardos mediante el uso de estructuras,
 * evitando el uso de `delay()` y mejorando el rendimiento en sistemas embebidos.
 */
#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"

/* Typedef -------------------------------------------------------------------*/
typedef uint32_t tick_t;

typedef bool bool_t;

/**
 * @brief Estructura para manejar retardos no bloqueantes.
 * @details Contiene el tiempo de inicio, la duración y un flag de estado.
 */
typedef struct{
   tick_t startTime; // Marca de tiempo inicial
   tick_t duration;  // Duracion del retardo en ms
   bool_t running;   // Indica si el retardo esta en ejecucion
} delay_t;


/* Function prototypes -------------------------------------------------------*/
/**
 * @brief Inicializa la estructura de retardo con una duración específica.
 * @param delay Puntero a la estructura delay_t.
 * @param duration Duración del retardo en milisegundos.
 */
void delayInit( delay_t * delay, tick_t duration );

/**
 * @brief Verifica si el retardo ha finalizado.
 * @param delay Puntero a la estructura delay_t.
 * @return `true` si el retardo ha finalizado, `false` en caso contrario.
 */
bool_t delayRead( delay_t * delay );

/**
 * @brief Modifica la duración de un retardo en ejecución.
 * @param delay Puntero a la estructura delay_t.
 * @param duration Nueva duración en milisegundos.
 */
void delayWrite( delay_t * delay, tick_t duration );

/**
 * @brief Verifica si el retardo está en ejecución.
 * @param delay Puntero a la estructura delay_t.
 * @return `true` si el retardo está en ejecución, `false` si no lo está.
 * @note La función utiliza `assert_param()` para validar el puntero.
 */
bool_t delayIsRunning (delay_t *delay);


void DWT_Delay_Init(void);
void delay_us(uint32_t us);

#endif /* API_INC_API_DELAY_H_ */
