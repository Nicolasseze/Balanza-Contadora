/**
 * @file lcd_driver.h
 * @brief Interfaz de alto nivel para el control de un LCD mediante comandos compatibles con HD44780.
 *
 * Este módulo encapsula el control lógico del LCD, permitiendo enviar comandos,
 * posicionar el cursor y mostrar texto, todo sobre una interfaz abstracta independiente del puerto físico.
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

/** @name Comandos básicos del LCD */
/**@{*/
#define LCD_CLEAR_DISPLAY       	0x01  /**< Limpia el display y vuelve el cursor a la posición 0 */
#define LCD_RETURN_HOME         	0x02  /**< Retorna el cursor a la posición inicial sin borrar contenido */
/**@}*/

/** @name Modo de entrada (Entry Mode Set) */
/**@{*/
#define LCD_ENTRY_MODE_INC	        0x06  /**< Cursor incrementa, sin desplazamiento de display */
#define LCD_ENTRY_MODE_DEC  	    0x04  /**< Cursor decrementa, sin desplazamiento de display */
#define LCD_ENTRY_MODE_INC_SHIFT 	0x07  /**< Cursor incrementa, con desplazamiento de display */
#define LCD_ENTRY_MODE_DEC_SHIFT 	0x05  /**< Cursor decrementa, con desplazamiento de display */
/**@}*/

/** @name Control de display (Display Control) */
/**@{*/
#define LCD_DISPLAY_OFF              	0x08  /**< Apaga display, cursor y parpadeo */
#define LCD_DISPLAY_ON               	0x0C  /**< Enciende display, sin cursor ni parpadeo */
#define LCD_DISPLAY_ON_CURSOR        	0x0E  /**< Enciende display y cursor */
#define LCD_DISPLAY_ON_BLINK         	0x0D  /**< Enciende display y parpadeo */
#define LCD_DISPLAY_ON_CURSOR_BLINK 	0x0F  /**< Enciende display, cursor y parpadeo */
/**@}*/

/** @name Desplazamiento del cursor/display */
/**@{*/
#define LCD_CURSOR_SHIFT_LEFT   	0x10  /**< Desplaza cursor a la izquierda */
#define LCD_CURSOR_SHIFT_RIGHT  	0x14  /**< Desplaza cursor a la derecha */
#define LCD_DISPLAY_SHIFT_LEFT  	0x18  /**< Desplaza todo el contenido a la izquierda */
#define LCD_DISPLAY_SHIFT_RIGHT 	0x1C  /**< Desplaza todo el contenido a la derecha */
/**@}*/

/** @name Configuración del LCD (Function Set) */
/**@{*/
#define LCD_FUNCTION_8BIT_1LINE_5x8   0x30 /**< 8 bits, 1 línea, matriz 5x8 */
#define LCD_FUNCTION_8BIT_2LINE_5x8   0x38 /**< 8 bits, 2 líneas, matriz 5x8 */
#define LCD_FUNCTION_4BIT_1LINE_5x8   0x20 /**< 4 bits, 1 línea, matriz 5x8 */
#define LCD_FUNCTION_4BIT_2LINE_5x8   0x28 /**< 4 bits, 2 líneas, matriz 5x8 */
/**@}*/

/** @name Direcciones base para las líneas del LCD */
/**@{*/
#define LCD_LINEA1	(0x80 + 0x00) /**< Dirección inicio línea 1 */
#define LCD_LINEA2	(0x80 + 0x40) /**< Dirección inicio línea 2 */
#define LCD_LINEA3	(0x80 + 0x14) /**< Dirección inicio línea 3 (si existe) */
#define LCD_LINEA4	(0x80 + 0x54) /**< Dirección inicio línea 4 (si existe) */
/**@}*/

/**
 * @typedef bool_t
 * @brief Tipo booleano (alias de `bool`) usado por consistencia en la interfaz.
 */
typedef bool bool_t;

/**
 * @enum LCD_Status_t
 * @brief Estado de retorno para funciones del LCD.
 */
typedef enum {
	LCD_OK = 0x00U,   /**< Operación exitosa */
	LCD_ERROR = 0x01U /**< Error en la operación */
} LCD_Status_t;

/**
 * @brief Inicializa el controlador del LCD.
 *
 * Debe llamarse una vez antes de utilizar cualquier otra función.
 *
 * @return true si se inicializó correctamente, false en caso de error.
 */
bool_t lcdInit(void);

/**
 * @brief Limpia el contenido del display.
 */
void lcdClear(void);

/**
 * @brief Posiciona el cursor en una fila y columna específica.
 *
 * @param row Fila (0 a 3, según el modelo de LCD)
 * @param col Columna (0 a 19 en LCD 20x4)
 */
void lcdSetCursor(uint8_t row, uint8_t col);

/**
 * @brief Imprime una cadena de texto en la posición actual del cursor.
 *
 * @param str Puntero a la cadena de texto terminada en null.
 */
void lcdPrint(const char *str);

/**
 * @brief Envía un comando directo al LCD.
 *
 * @param cmd Comando a enviar (por ejemplo `LCD_CLEAR_DISPLAY`).
 */
void lcdCommand(uint8_t cmd);

/**
 * @brief Envía un dato al LCD (usado para imprimir un carácter).
 *
 * @param data Byte de datos (código ASCII u otro carácter).
 */
void lcdData(uint8_t data);

#endif // API_INC_LCD_DRIVER_H_


