/**
 * @file lcd_port.h
 * @brief Interfaz de bajo nivel para el control de un LCD 20x4 mediante I2C utilizando PCF8574.
 *
 * Este módulo proporciona funciones para inicializar el puerto de comunicación
 * y escribir datos o comandos al display LCD a través del expansor de I/O I2C.
 */

#ifndef API_INC_LCD_PORT_H_
#define API_INC_LCD_PORT_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @def LCD_ADDRESS
 * @brief Dirección I2C del dispositivo PCF8574 conectado al LCD.
 */
#define LCD_ADDRESS 	0x40U

/** @name Definición de bits para el control del LCD */
/**@{*/
#define LCD_EN        	0x20U  /**< Bit de habilitación (Enable) */
#define LCD_RW        	0x40U  /**< Bit de lectura/escritura (RW). No se utiliza, se deja en 0 */
#define LCD_RS_CMD    	0x00U  /**< Bit RS en modo comando */
#define LCD_RS_TXT    	0x80U  /**< Bit RS en modo texto/datos */

//El Backlight esta seteado en "0x00" xq esta deshabilitado
#define LCD_BACKLIGHT 	0x00U  /**< Bit para activar retroiluminación */
/**@}*/

/** @name Manejo de nibbles para la transmisión en 4 bits */
/**@{*/
#define LCD_HIGH_NIBBLE_MASK	0xF0  /**< Máscara para nibble alto */
#define LCD_LOW_NIBBLE_MASK		0x0F  /**< Máscara para nibble bajo */
#define LCD_HIGH_NIBBLE_SHIFT	4     /**< Shift para nibble alto en el byte transmitido */
#define LCD_LOW_NIBBLE_SHIFT	0     /**< Shift para nibble bajo en el byte transmitido */
/**@}*/

/**
 * @def LCD_INIT
 * @brief Comando de inicialización (modo 8 bits) desplazado para transmisión.
 */
#define LCD_INIT	(0x0CU)
#define LCD_INIT2	(0x04U)

/**
 * @typedef bool_t
 * @brief Tipo booleano usado para compatibilidad con HAL y mayor claridad.
 */
typedef bool bool_t;

/**
 * @brief Manejador de errores general del sistema.
 *
 * Esta función se debe implementar en la aplicación para gestionar errores críticos.
 */
extern void Error_Handler(void);

/**
 * @brief Inicializa el puerto I2C y configura el entorno necesario para el LCD.
 *
 * Debe llamarse una vez al inicio antes de enviar cualquier comando o dato.
 */
void lcdPortInit(void);

/**
 * @brief Envía un byte al LCD a través del bus I2C.
 *
 * @param data Dato a enviar (nibble alto o bajo).
 * @param rs Indica si el dato es comando (false) o carácter de texto (true).
 * @return true si la transmisión fue exitosa, false en caso de error.
 */
bool_t lcdPortWrite(uint8_t data, bool_t rs);

#endif /* API_INC_LCD_PORT_H_ */
