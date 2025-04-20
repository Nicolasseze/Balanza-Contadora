/**
 * @file hx711_driver.h
 * @brief Interfaz de alto nivel para procesamiento de datos del HX711.
 */

#ifndef API_INC_HX711_DRIVER_H_
#define API_INC_HX711_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define WEIGHT_GAIN_DEFAULT  128
#define WEIGHT_RATE_DEFAULT  fq_10hz

/**
 * @brief Estructura de configuración de pines y hardware del HX711.
 *        Esta estructura debe ser compatible con la capa hx711_port.
 */
typedef struct {
    void* din_port;
    uint16_t din_pin;
    void* sck_port;
    uint16_t sck_pin;
    void* rate_port;
    uint16_t rate_pin;
    uint8_t gain;
    uint8_t rate;
} Weight_HWConfig_t;

/**
 * @brief Estructura de configuración para inicializar el driver HX711.
 */
typedef struct {
    Weight_HWConfig_t port_config;   /**< Configuración de pines y hardware */
    float scale;                    /**< Escala de conversión: valores raw → kg */
    int32_t offset;                 /**< Offset a restar a la lectura raw */
} Weight_Config_t;

/**
 * @brief Contexto interno del sensor. Almacena datos de calibración y lectura actual.
 */
typedef struct {
    float scale;           /**< Escala de conversión */
    int32_t offset;        /**< Offset de tara */
    float weight;          /**< Última lectura en kilogramos */
} WeightSensor_t;

void   WeightDriverInit(WeightSensor_t* ctx, Weight_Config_t* cfg);
float  WeightRead(void);
void   Weight_SetScale(float scale);
void   Weight_SetOffset(int32_t offset);
float  Weight_GetScale(void);
int32_t Weight_GetOffset(void);

/**
 * @brief Calibra la escala del sensor usando una masa conocida.
 *
 * @param peso_known Peso real en kg de la masa colocada.
 * @param offset Valor de offset previamente medido (tara).
 * @param raw Valor raw leído con la masa colocada.
 * @return Escala calculada (cuentas por kg).
 */
float Weight_CalibrateScale(float peso_known, int32_t offset, int32_t raw);

#endif /* API_INC_HX711_DRIVER_H_ */
