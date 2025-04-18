/**
 * @file lcd_port.c
 * @brief Implementación de funciones de bajo nivel para manejar un LCD 20x4 mediante I2C usando PCF8574.
 *
 * Este archivo contiene la lógica de inicialización del LCD y el envío de datos o comandos
 * a través de una interfaz I2C, utilizando el periférico configurado con HAL.
 */

#include "lcd_port.h"
#include "stm32f4xx_hal.h"

/**
 * @brief Manejador de I2C definido externamente.
 *
 * Este handler debe estar inicializado en otro módulo antes de utilizar estas funciones.
 */
extern I2C_HandleTypeDef hi2c1;

static uint8_t reverse_nibble(uint8_t nibble);

/**
 * @brief Envía un pulso de habilitación al LCD para capturar el nibble actual.
 *
 * Esta función genera el strobe requerido para que el LCD capture los datos en el bus.
 * Se envían dos transmisiones: una con el bit `EN` en alto y otra con `EN` en bajo.
 *
 * @param data Byte con los bits de control y datos a enviar.
 */
static void lcdStrobe(uint8_t data) {
    uint8_t buf = data | LCD_EN | LCD_BACKLIGHT;

    if (HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &buf, 1, HAL_MAX_DELAY) != HAL_OK)
        Error_Handler();

    HAL_Delay(1);

    buf = (data & ~LCD_EN) | LCD_BACKLIGHT;

    if (HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, &buf, 1, HAL_MAX_DELAY) != HAL_OK)
        Error_Handler();

    HAL_Delay(1);
}

/**
 * @brief Inicializa el LCD en modo de operación de 4 bits.
 *
 * Debe ser llamada una vez tras el encendido del sistema. Ejecuta la secuencia de
 * inicialización especial exigida por la hoja de datos del controlador del LCD.
 */
void lcdPortInit(void) {
    HAL_Delay(50); // Espera mínima >15 ms desde encendido

    // Fase especial: envío directo del nibble alto del comando 0x30 varias veces
    lcdStrobe(LCD_INIT);
    HAL_Delay(5);

    lcdStrobe(LCD_INIT);
    HAL_Delay(5);

    lcdStrobe(LCD_INIT);
    HAL_Delay(5);

    lcdStrobe(LCD_INIT2);
    HAL_Delay(5);
    // Modo 4 bits habilitado
}

/**
 * @brief Escribe un byte en el LCD, dividiéndolo en nibbles.
 *
 * Transmite un byte de datos o comando al LCD en modo de 4 bits. El byte se divide
 * en nibble alto y bajo, los cuales se transmiten por separado mediante la función `lcdStrobe()`.
 *
 * @param data Byte a escribir en el LCD.
 * @param rs   Si es `true`, se trata de datos (texto); si es `false`, es un comando.
 * @return `true` si la operación fue exitosa.
 */
bool_t lcdPortWrite(uint8_t data, bool_t rs) {
    uint8_t upper = (data & LCD_HIGH_NIBBLE_MASK) >> LCD_HIGH_NIBBLE_SHIFT;
    upper = reverse_nibble(upper);

    uint8_t lower = (data & LCD_LOW_NIBBLE_MASK) << LCD_LOW_NIBBLE_SHIFT;
    lower = reverse_nibble(lower);

    uint8_t control = rs ? LCD_RS_TXT : LCD_RS_CMD;

    lcdStrobe(upper | control);
    lcdStrobe(lower | control);

    return true;
}

static uint8_t reverse_nibble(uint8_t nibble) {
	static const uint8_t reverse_table[16] = {
			0x0, 0x8, 0x4, 0xC,
			0x2, 0xA, 0x6, 0xE,
			0x1, 0x9, 0x5, 0xD,
			0x3, 0xB, 0x7, 0xF
	};
	return reverse_table[nibble & 0x0F];

}

