/**
 * @file lcd_driver.c
 * @brief Implementación del controlador de alto nivel para un LCD tipo HD44780 vía I2C.
 *
 * Este archivo contiene las funciones necesarias para inicializar el display,
 * enviar comandos, imprimir texto y posicionar el cursor. Utiliza la capa `lcd_port`
 * para acceder al hardware físico mediante I2C.
 */

#include "lcd_driver.h"
#include "lcd_port.h"
#include "stm32f4xx_hal.h"

/**
 * @brief Envía un comando al LCD.
 *
 * Esta función es interna y envía el comando utilizando la función de bajo nivel `lcdPortWrite`.
 * Se agrega un pequeño retardo requerido por la hoja de datos del LCD.
 *
 * @param cmd Byte de comando a enviar.
 */
static void lcdSendCmd(uint8_t cmd) {
    lcdPortWrite(cmd, false);
    HAL_Delay(2);
}

/**
 * @brief Envía un dato (carácter) al LCD.
 *
 * Esta función es interna y envía un carácter para mostrar en pantalla.
 * Usa `lcdPortWrite` con el bit RS activado.
 *
 * @param data Byte de datos (por ejemplo, un código ASCII).
 */
static void lcdSendData(uint8_t data) {
    lcdPortWrite(data, true);
//    delay_us(2);
//    HAL_Delay(2);
}

/**
 * @brief Envía un comando al LCD (interfaz pública).
 *
 * @param cmd Comando del conjunto de instrucciones HD44780.
 */
void lcdCommand(uint8_t cmd) {
    lcdSendCmd(cmd);
}

/**
 * @brief Envía un carácter al LCD (interfaz pública).
 *
 * @param data Carácter (byte) a mostrar en pantalla.
 */
void lcdData(uint8_t data) {
    lcdSendData(data);
}

/**
 * @brief Inicializa el LCD en modo 4 bits y configuración estándar.
 *
 * Configura el display con dos líneas, fuente de 5x8, entrada incremental y display encendido.
 *
 * @return `true` si la inicialización fue exitosa.
 */
bool_t lcdInit(void) {

    lcdPortInit(); // Inicializa capa de bajo nivel y espera inicial

    lcdSendCmd(LCD_FUNCTION_4BIT_2LINE_5x8);   // Configura modo 4 bits, 2 líneas
    lcdSendCmd(LCD_DISPLAY_ON);               // Enciende display sin cursor ni blink
    lcdSendCmd(LCD_ENTRY_MODE_INC);           // Cursor avanza a derecha, sin scroll
    lcdClear();                               // Limpia display
    HAL_Delay(2);

    return true;
}

/**
 * @brief Limpia completamente el contenido del LCD.
 */
void lcdClear(void) {
    lcdSendCmd(LCD_CLEAR_DISPLAY);
    HAL_Delay(2);
}

/**
 * @brief Posiciona el cursor en una coordenada específica del display.
 *
 * @param row Número de fila (0 a 3).
 * @param col Número de columna (0 a 19).
 */
void lcdSetCursor(uint8_t row, uint8_t col) {
    uint8_t address[] = {LCD_LINEA1, LCD_LINEA2, LCD_LINEA3, LCD_LINEA4};

    if (row > 3 || col > 19)
        assert_param(0); // Verifica que la posición sea válida

    lcdSendCmd(address[row] + col);
}

/**
 * @brief Muestra una cadena de texto en el LCD desde la posición actual del cursor.
 *
 * @param str Puntero a la cadena terminada en null.
 */
void lcdPrint(const char *str) {

    assert_param(str != NULL); // Asegura que el puntero no sea nulo

    while (*str) {
        lcdSendData(*str++);
    }
}

void lcdShowLine(uint8_t row, const char *str){
	assert_param(str != NULL);

	lcdSetCursor(row, 0);
	lcdPrint(str);

}

void lcdShowCentered(uint8_t line, const char* str) {
    char buffer[LCD_WIDTH + 1]; // +1 para el '\0'
    size_t len = strlen(str);

    if (len > LCD_WIDTH) {
        strncpy(buffer, str, LCD_WIDTH);
        buffer[LCD_WIDTH] = '\0';
    } else {
        size_t padding = (LCD_WIDTH - len) / 2;
        memset(buffer, ' ', LCD_WIDTH);
        memcpy(buffer + padding, str, len);
        buffer[LCD_WIDTH] = '\0';
    }

    lcdShowLine(line, buffer);
}

void lcdClearRange(uint8_t row, uint8_t colStart, uint8_t colEnd) {
    if (colEnd >= LCD_WIDTH || colStart > colEnd)
        return;

    char buf[LCD_WIDTH + 1];
    memset(buf, ' ', LCD_WIDTH);
    buf[LCD_WIDTH] = '\0';

    lcdSetCursor(row, colStart);
    buf[colEnd - colStart + 1] = '\0';  // solo imprime lo necesario
    lcdPrint(&buf[colStart]);
}

