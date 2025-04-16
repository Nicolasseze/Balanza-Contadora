/*
 * keypad_port.c
 *
 *  Created on: Apr 15, 2025
 *      Author: nicolas-porco
 */



static const uint16_t row_pins[KEYPAD_ROWS] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };
static const uint16_t col_pins[KEYPAD_COLS] = { GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };

void keypadPortInit(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Filas como salida Open Drain (apagadas por defecto)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (int i = 0; i < KEYPAD_ROWS; i++) {
        GPIO_InitStruct.Pin = row_pins[i];
        HAL_GPIO_Init(ROW_GPIO, &GPIO_InitStruct);
        HAL_GPIO_WritePin(ROW_GPIO, row_pins[i], GPIO_PIN_SET); // HIGH-Z (no activo)
    }

    // Columnas como entrada con pull-up
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    for (int i = 0; i < KEYPAD_COLS; i++) {
        GPIO_InitStruct.Pin = col_pins[i];
        HAL_GPIO_Init(COL_GPIO, &GPIO_InitStruct);
    }
}

void keypadPortSetRow(uint8_t row, bool state) {
    HAL_GPIO_WritePin(ROW_GPIO, row_pins[row], state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool keypadPortReadCol(uint8_t col) {
    return (HAL_GPIO_ReadPin(COL_GPIO, col_pins[col]) == GPIO_PIN_RESET);
}
