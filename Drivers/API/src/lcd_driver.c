#include "lcd_driver.h"
#include "lcd_port.h"
#include "assert.h"
#include "string.h"

/**
 * @brief Envía un comando al LCD.
 *
 * Esta función interna envía el comando utilizando `lcdPortWrite`.
 * Se agrega un retardo requerido por la hoja de datos.
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
 * Esta función interna envía un carácter para mostrar en pantalla,
 * usando `lcdPortWrite` con el bit RS activado.
 *
 * @param data Byte de datos (por ejemplo, un carácter ASCII).
 */
static void lcdSendData(uint8_t data) {
    lcdPortWrite(data, true);
}

/**
 * @brief Envía un comando de control al LCD.
 *
 * @param cmd Comando del conjunto de instrucciones HD44780.
 * @retval LCD_OK si se envió correctamente.
 */
LCD_Status_t lcdCommand(uint8_t cmd) {
    lcdSendCmd(cmd);
    return LCD_OK;
}

/**
 * @brief Envía un carácter al LCD.
 *
 * @param data Carácter (byte) a mostrar.
 * @retval LCD_OK si fue exitoso.
 */
LCD_Status_t lcdData(uint8_t data) {
    lcdSendData(data);
    return LCD_OK;
}

/**
 * @brief Inicializa el LCD en modo 4 bits.
 *
 * Configura el display con dos líneas, fuente 5x8, entrada incremental y display encendido.
 *
 * @retval LCD_OK si la inicialización fue exitosa.
 */
LCD_Status_t lcdInit(void) {
    lcdPortInit();  // Inicializa la capa de bajo nivel

    lcdSendCmd(LCD_FUNCTION_4BIT_2LINE_5x8);
    lcdSendCmd(LCD_DISPLAY_ON);
    lcdSendCmd(LCD_ENTRY_MODE_INC);
    lcdClear();
    HAL_Delay(2);

    return LCD_OK;
}

/**
 * @brief Limpia completamente el contenido del LCD.
 *
 * @retval LCD_OK si fue exitoso.
 */
LCD_Status_t lcdClear(void) {
    lcdSendCmd(LCD_CLEAR_DISPLAY);
    HAL_Delay(2);
    return LCD_OK;
}

/**
 * @brief Posiciona el cursor en una coordenada específica.
 *
 * @param row Fila (0 a 3).
 * @param col Columna (0 a 19).
 * @retval LCD_OK si se posicionó correctamente.
 */
LCD_Status_t lcdSetCursor(uint8_t row, uint8_t col) {
    static const uint8_t address[] = {LCD_LINEA1, LCD_LINEA2, LCD_LINEA3, LCD_LINEA4};

    if (row > 3 || col > 19)
        return LCD_ERROR;

    lcdSendCmd(address[row] + col);
    return LCD_OK;
}

/**
 * @brief Muestra una cadena desde la posición actual del cursor.
 *
 * @param str Cadena terminada en null.
 * @retval LCD_OK si se imprimió correctamente.
 * @retval LCD_ERROR si el puntero es nulo.
 */
LCD_Status_t lcdPrint(const char *str) {
    if (str == NULL)
        return LCD_ERROR;

    while (*str) {
        lcdSendData((uint8_t)*str++);
    }
    return LCD_OK;
}

/**
 * @brief Imprime una cadena alineada a la izquierda en una línea.
 *
 * @param row Fila (0 a 3).
 * @param str Cadena terminada en null.
 * @retval LCD_OK si se imprimió correctamente.
 * @retval LCD_ERROR si los parámetros son inválidos.
 */
LCD_Status_t lcdShowLine(uint8_t row, const char *str) {
    if (str == NULL || row > 3)
        return LCD_ERROR;

    if (lcdSetCursor(row, 0) != LCD_OK)
        return LCD_ERROR;

    return lcdPrint(str);
}

/**
 * @brief Apaga el cursor visible del LCD.
 *
 * @retval LCD_OK si se desactivó correctamente.
 */
LCD_Status_t lcdOffCursor(void) {
    return lcdCommand(LCD_DISPLAY_ON);
}

/**
 * @brief Imprime una cadena desde una posición y deja el cursor visible.
 *
 * @param row Fila (0 a 3).
 * @param col Columna (0 a 19).
 * @param str Cadena terminada en null.
 * @retval LCD_OK si fue exitoso.
 * @retval LCD_ERROR si los parámetros son inválidos.
 */
LCD_Status_t lcdPrintConCursor(uint8_t row, uint8_t col, const char *str) {
    if (str == NULL || row > 3 || col > 19)
        return LCD_ERROR;

    if (lcdSetCursor(row, col) != LCD_OK)
        return LCD_ERROR;

    if (lcdPrint(str) != LCD_OK)
        return LCD_ERROR;

    size_t len = strlen(str);
    if (lcdSetCursor(row, col + len) != LCD_OK)
        return LCD_ERROR;

    return lcdCommand(LCD_DISPLAY_ON_CURSOR_BLINK);
}

/**
 * @brief Muestra una cadena centrada horizontalmente.
 *
 * @param line Línea del LCD (0 a 3).
 * @param str Cadena terminada en null.
 * @retval LCD_OK si fue exitoso.
 * @retval LCD_ERROR si los parámetros son inválidos.
 */
LCD_Status_t lcdShowCentered(uint8_t line, const char* str) {
    if (str == NULL || line > 3)
        return LCD_ERROR;

    char buffer[LCD_WIDTH + 1];
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

    return lcdShowLine(line, buffer);
}

/**
 * @brief Borra un rango de columnas en una línea del LCD.
 *
 * @param row Fila (0 a 3).
 * @param colStart Columna inicial (inclusive).
 * @param colEnd Columna final (inclusive).
 * @retval LCD_OK si fue exitoso.
 * @retval LCD_ERROR si los parámetros son inválidos.
 */
LCD_Status_t lcdClearRange(uint8_t row, uint8_t colStart, uint8_t colEnd) {
    if (colEnd >= LCD_WIDTH || colStart > colEnd || row > 3)
        return LCD_ERROR;

    char buf[LCD_WIDTH + 1];
    memset(buf, ' ', LCD_WIDTH);
    buf[colEnd - colStart + 1] = '\0';

    if (lcdSetCursor(row, colStart) != LCD_OK)
        return LCD_ERROR;

    return lcdPrint(buf);
}

