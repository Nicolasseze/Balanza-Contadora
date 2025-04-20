/**
 * @file hx711_port.h
 * @brief Interfaz de bajo nivel para el control físico del HX711.
 *
 * Este módulo implementa el acceso directo a los pines y temporización del HX711.
 */

#ifndef API_INC_HX711_PORT_H_
#define API_INC_HX711_PORT_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"

#define HX711_GAIN_128  128
#define HX711_GAIN_64   64

#define HX711_DELAY_US 	1  // delay entre flancos, ajustar si hace falta

// Definiciones del tipo booleano (si no tenés stdbool)
typedef bool bool_t;

/**
 * @brief Frecuencias de muestreo disponibles del HX711.
 */
typedef enum {
    fq_10hz,
    fq_80hz
} FreqRate_t;

/**
 * @brief Estructura de configuración del sensor HX711.
 */
typedef struct {
    GPIO_TypeDef* din_port;
    uint16_t din_pin;

    GPIO_TypeDef* sck_port;
    uint16_t sck_pin;

    GPIO_TypeDef* rate_port;
    uint16_t rate_pin;

    uint8_t gain;      /**< Ganancia: 128, 64 */
    FreqRate_t rate;   /**< Frecuencia de muestreo */

    int32_t data;      /**< Última lectura raw */
} HX711_t;

/**
 * @brief Inicializa los pines y parámetros del HX711.
 *
 * @param hx Puntero a estructura de configuración.
 */
void HX711_Init(HX711_t *hx);

/**
 * @brief Realiza una lectura bruta de 24 bits desde el HX711.
 *
 * @return Valor raw leído del conversor.
 */
int32_t HX711_ReadRaw(void);

/**
 * @brief Configura la ganancia del canal A del HX711 (128 o 64).
 *
 * @param gain Valor de ganancia (128 o 64). Si se pasa otro valor, se fuerza a 128.
 */
void HX711_SetGain(uint8_t gain);

/**
 * @brief Configura la frecuencia de muestreo del HX711 (10Hz o 80Hz).
 *
 * @param rate Frecuencia deseada.
 */
void HX711_SetRate(FreqRate_t rate);

#endif /* API_INC_HX711_PORT_H_ */

