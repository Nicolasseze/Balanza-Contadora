/**
 * @file hx711_port.h
 * @brief Interfaz de la capa de puerto (acceso al hardware) para el sensor HX711.
 *
 * Este archivo contiene las definiciones de tipos, macros y prototipos de funciones
 * para manejar la comunicación directa con el ADC HX711. Proporciona funciones para
 * inicialización, configuración y lectura del sensor utilizando pines GPIO.
 *
 * @author Nicolás Porco
 */

#ifndef API_INC_HX711_PORT_H_
#define API_INC_HX711_PORT_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"

// Reemplazá estos defines por los pines reales que uses
#define HX_DOUT_PORT   		GPIOA
#define HX_DOUT_PIN    		GPIO_PIN_6

#define HX_SCK_PORT    		GPIOA
#define HX_SCK_PIN     		GPIO_PIN_7

#define HX_RATE_PORT   		NULL //nO SE USA
#define HX_RATE_PIN    		0xFFFF

#define HX711_GAIN_128  	128
#define HX711_GAIN_64   	64
#define HX711_GAIN_32   	32
#define HX711_GAIN_DEFAULT	HX711_GAIN_128

#define HX711_DELAY_US 		1

/**
 * @brief Definición de tipo booleano para compatibilidad.
 */
typedef bool bool_t;

/**
 * @brief Frecuencias de muestreo disponibles del HX711.
 */
typedef enum {
    fq_10hz,   /**< Frecuencia de muestreo: 10 Hz */
    fq_80hz    /**< Frecuencia de muestreo: 80 Hz */
} FreqRate_t;


/**
 * @brief Inicializa los pines necesarios para el HX711.
 *
 * Configura los pines GPIO del microcontrolador para manejar los
 * pines DOUT, SCK y, si corresponde, RATE del HX711.
 */
void HX711PortInit(void);


/**
 * @brief Lee una medición cruda del HX711 directamente.
 *
 * Realiza el ciclo de reloj necesario para obtener los 24 bits
 * de salida del ADC y devuelve el valor como entero con signo.
 *
 * @return Valor entero de 24 bits en complemento a dos, extendido a 32 bits.
 */
int32_t HX711PortReadRaw(void);

/**
 * @brief Configura la ganancia del HX711.
 *
 * Permite seleccionar entre las ganancias disponibles para el canal A o B.
 * La configuración se aplica en la próxima lectura válida.
 *
 * @param gain Valor de ganancia (64, 128 o 32).
 * @return Valor de ganancia confirmado tras la configuración.
 */
uint8_t HX711PortSetGain(uint8_t gain);

/**
 * @brief Configura la frecuencia de muestreo del HX711.
 *
 * Establece la frecuencia de salida de datos del HX711, si el pin RATE
 * está conectado y configurado. De lo contrario, no tiene efecto.
 *
 * @param rate Frecuencia deseada (10 Hz o 80 Hz).
 * @return Frecuencia que se ha configurado efectivamente.
 */
FreqRate_t HX711PortSetRate(FreqRate_t rate);

#endif /* API_INC_HX711_PORT_H_ */

