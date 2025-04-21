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
#include "API_delay.h"

/**
 * @def LCD_ADDRESS
 * @brief Dirección I2C del dispositivo PCF8574 conectado al LCD.
 */
#define LCD_ADDRESS 	0x40U

/** @name Bits de control del LCD */
/**@{*/
#define LCD_EN        	0x20U  /**< Bit de habilitación (Enable) */
#define LCD_RW        	0x40U  /**< Bit de lectura/escritura. No se usa, siempre 0 */
#define LCD_RS_CMD    	0x00U  /**< Bit RS en modo comando */
#define LCD_RS_TXT    	0x80U  /**< Bit RS en modo texto/datos */
#define LCD_BACKLIGHT 	0x00U  /**< Bit para controlar retroiluminación (deshabilitado) */
/**@}*/

/** @name Manejo de nibbles para transmisión en modo 4 bits */
/**@{*/
#define LCD_HIGH_NIBBLE_MASK	0xF0  /**< Máscara para nibble alto */
#define LCD_LOW_NIBBLE_MASK		0x0F  /**< Máscara para nibble bajo */
#define LCD_HIGH_NIBBLE_SHIFT	4     /**< Desplazamiento del nibble alto */
#define LCD_LOW_NIBBLE_SHIFT	0     /**< Desplazamiento del nibble bajo */
/**@}*/

/**
 * @def LCD_INIT
 * @brief Comando de inicialización (modo 8 bits) desplazado.
 */
#define LCD_INIT	(0x0CU)

/**
 * @def LCD_INIT2
 * @brief Segundo comando de inicialización requerido.
 */
#define LCD_INIT2	(0x04U)

/**
 * @typedef bool_t
 * @brief Alias de tipo booleano, usado para consistencia con HAL y claridad semántica.
 */
typedef bool bool_t;

/**
 * @brief Manejador de error general del sistema.
 *
 * Esta función debe ser implementada por la aplicación para manejar errores críticos.
 */
extern void Error_Handler(void);

/**
 * @brief Inicializa el bus I2C y la interfaz con el LCD.
 *
 * Esta función debe llamarse antes de utilizar cualquier función del driver.
 */
void lcdPortInit(void);

/**
 * @brief Envía un byte al LCD por I2C con control del bit RS.
 *
 * @param data Dato a enviar al PCF8574 (nibble alto o bajo con bits de control).
 * @param rs   Bit que indica si se trata de un comando (false) o dato (true).
 * @retval true  Si la transmisión fue exitosa.
 * @retval false Si hubo un error de comunicación.
 */
bool_t lcdPortWrite(uint8_t data, bool_t rs);

#endif /* API_INC_LCD_PORT_H_ */
