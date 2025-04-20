/**
 * @file lcd_driver.h
 * @brief Interfaz de alto nivel para controlar un LCD basado en el controlador HD44780.
 *
 * @details
 * Este módulo proporciona funciones de control para pantallas LCD compatibles con el controlador HD44780
 * (comúnmente 16x2, 20x4, etc.), operando en modo de 4 bits. Las funciones permiten inicializar el módulo,
 * controlar la posición del cursor, limpiar el display y escribir texto. Está diseñado para ser independiente
 * del hardware físico, utilizando una capa de abstracción para los pines (ver lcd_port.h).
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "string.h"

/** @def LCD_WIDTH
 *  @brief Cantidad de caracteres por línea del LCD.
 */
#define LCD_WIDTH 	20

/** @def LCD_ROWS
 *  @brief Cantidad de líneas del LCD.
 */
#define LCD_ROWS	4

/** @name Comandos básicos del LCD */
/**@{*/
#define LCD_CLEAR_DISPLAY       	0x01  /**< Limpia el display y retorna el cursor a (0,0). */
#define LCD_RETURN_HOME         	0x02  /**< Retorna el cursor a (0,0) sin modificar el contenido. */
/**@}*/

/** @name Modo de entrada (Entry Mode Set) */
/**@{*/
#define LCD_ENTRY_MODE_INC	        0x06  /**< Cursor incrementa, sin desplazamiento del display. */
#define LCD_ENTRY_MODE_DEC  	    0x04  /**< Cursor decrementa, sin desplazamiento del display. */
#define LCD_ENTRY_MODE_INC_SHIFT 	0x07  /**< Cursor incrementa, con desplazamiento del display. */
#define LCD_ENTRY_MODE_DEC_SHIFT 	0x05  /**< Cursor decrementa, con desplazamiento del display. */
/**@}*/

/** @name Control del display (Display Control) */
/**@{*/
#define LCD_DISPLAY_OFF              	0x08  /**< Apaga display, cursor y parpadeo. */
#define LCD_DISPLAY_ON               	0x0C  /**< Enciende display, sin cursor ni parpadeo. */
#define LCD_DISPLAY_ON_CURSOR        	0x0E  /**< Enciende display y cursor. */
#define LCD_DISPLAY_ON_BLINK         	0x0D  /**< Enciende display y parpadeo. */
#define LCD_DISPLAY_ON_CURSOR_BLINK 	0x0F  /**< Enciende display, cursor y parpadeo. */
/**@}*/

/** @name Desplazamiento del cursor o display */
/**@{*/
#define LCD_CURSOR_SHIFT_LEFT   	0x10  /**< Desplaza el cursor a la izquierda. */
#define LCD_CURSOR_SHIFT_RIGHT  	0x14  /**< Desplaza el cursor a la derecha. */
#define LCD_DISPLAY_SHIFT_LEFT  	0x18  /**< Desplaza el contenido del display a la izquierda. */
#define LCD_DISPLAY_SHIFT_RIGHT 	0x1C  /**< Desplaza el contenido del display a la derecha. */
/**@}*/

/** @name Configuración del LCD (Function Set) */
/**@{*/
#define LCD_FUNCTION_8BIT_1LINE_5x8   0x30 /**< Modo 8 bits, 1 línea, caracteres 5x8. */
#define LCD_FUNCTION_8BIT_2LINE_5x8   0x38 /**< Modo 8 bits, 2 líneas, caracteres 5x8. */
#define LCD_FUNCTION_4BIT_1LINE_5x8   0x20 /**< Modo 4 bits, 1 línea, caracteres 5x8. */
#define LCD_FUNCTION_4BIT_2LINE_5x8   0x28 /**< Modo 4 bits, 2 líneas, caracteres 5x8. */
/**@}*/

/** @name Direcciones base de líneas del LCD */
/**@{*/
#define LCD_LINEA1	(0x80 + 0x00) /**< Dirección DDRAM de la línea 1. */
#define LCD_LINEA2	(0x80 + 0x40) /**< Dirección DDRAM de la línea 2. */
#define LCD_LINEA3	(0x80 + 0x14) /**< Dirección DDRAM de la línea 3 (si existe). */
#define LCD_LINEA4	(0x80 + 0x54) /**< Dirección DDRAM de la línea 4 (si existe). */
/**@}*/

/**
 * @typedef bool_t
 * @brief Tipo booleano alternativo para mantener consistencia en la interfaz.
 */
typedef bool bool_t;

/**
 * @enum LCD_Status_t
 * @brief Estado de retorno estándar para funciones del LCD.
 */
typedef enum {
	LCD_OK = 0x00U,   /**< Operación realizada correctamente. */
	LCD_ERROR = 0x01U /**< Ocurrió un error al realizar la operación. */
} LCD_Status_t;

/**
 * @brief Inicializa el LCD.
 *
 * Debe llamarse una única vez al inicio del programa para configurar el LCD en modo 4 bits.
 *
 * @return `true` si la inicialización fue exitosa, `false` si hubo un error.
 */
bool_t lcdInit(void);

/**
 * @brief Limpia completamente el contenido del LCD.
 *
 * También posiciona el cursor en la posición inicial (0,0).
 */
void lcdClear(void);

/**
 * @brief Posiciona el cursor en una ubicación específica.
 *
 * @param row Número de fila (0 a 3 según el modelo de LCD).
 * @param col Número de columna (0 a 19 en un LCD 20x4).
 */
void lcdSetCursor(uint8_t row, uint8_t col);

/**
 * @brief Muestra una cadena de texto a partir de la posición actual del cursor.
 *
 * Si la cadena sobrepasa el final de la línea, no se realiza un salto automático.
 *
 * @param str Cadena de caracteres terminada en null.
 */
void lcdPrint(const char *str);

/**
 * @brief Envía un comando de control directamente al LCD.
 *
 * Utilizado para operaciones como limpiar pantalla o mover cursor.
 *
 * @param cmd Comando a enviar (por ejemplo `LCD_CLEAR_DISPLAY`).
 */
void lcdCommand(uint8_t cmd);

/**
 * @brief Envía un byte de datos al LCD (por ejemplo un carácter).
 *
 * @param data Byte ASCII o carácter personalizado a mostrar.
 */
void lcdData(uint8_t data);

/**
 * @brief Muestra una cadena centrada horizontalmente en la línea especificada.
 *
 * @param line Línea del LCD (0 a 3).
 * @param str  Cadena de texto a centrar.
 */
void lcdShowCentered(uint8_t line, const char* str);

/**
 * @brief Imprime una cadena de texto alineada a la izquierda en una línea específica del LCD.
 *
 * @param row Línea en la que se imprimirá el texto (0 a 3).
 * @param str Cadena a mostrar.
 */
void lcdShowLine(uint8_t row, const char *str);


/**
 * @brief Borra un rango específico de columnas en una fila del LCD.
 *
 * Esta función sobrescribe con espacios el rango de columnas especificado en la fila indicada.
 * Es útil para limpiar parcialmente una línea sin tener que borrar todo el display.
 *
 * @param row      Número de fila donde se desea limpiar (comenzando en 0).
 * @param colStart Columna inicial del rango a limpiar (inclusive).
 * @param colEnd   Columna final del rango a limpiar (inclusive).
 *
 * @note Si colEnd >= LCD_WIDTH o colStart > colEnd, la función no realiza ninguna operación.
 */
void lcdClearRange(uint8_t row, uint8_t colStart, uint8_t colEnd);


#endif // API_INC_LCD_DRIVER_H_


