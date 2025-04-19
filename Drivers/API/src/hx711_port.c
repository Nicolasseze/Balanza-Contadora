/*
 * hx711_port.c
 *
 *  Created on: Apr 14, 2025
 *      Author: nicolas-porco
 */

#ifndef API_SRC_HX711_PORT_C_
#define API_SRC_HX711_PORT_C_

#include "hx711_port.h"
#include "stm32f4xx_hal.h"

#define DELAY_US 1  // delay entre flancos, ajustar si hace falta

static bool_t HX711_IsReady(HX711_t *hx);


void HX711_Init(HX711_t *hx) {
    // Configura el puerto del clock como salida
    GPIO_InitTypeDef gpio = {0};

    gpio.Pin = hx->sck_pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(hx->sck_port, &gpio);
    HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);

    // Configura DIN como entrada
    gpio.Pin = hx->din_pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(hx->din_port, &gpio);

    //Configura el pin como salida p/ manejar la frecuencia de medicion
    gpio.Pin = hx->rate_pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(hx->rate_port, &gpio);

    HAL_GPIO_WritePin(hx->rate_port, hx->rate_pin, hx->rate == fq_10hz ? GPIO_PIN_RESET : GPIO_PIN_SET);

    DWT_Delay_Init();

}


int32_t HX711_ReadRaw(HX711_t *hx) {

	while (!HX711_IsReady(hx));  // espera conversión

    int32_t data = 0;

    for (uint8_t i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_SET);
        delay_us(DELAY_US);
        data = (data << 1) | HAL_GPIO_ReadPin(hx->din_port, hx->din_pin);
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);
        delay_us(DELAY_US);
    }

    // Pulsos extras para setear ganancia
    for (uint8_t i = 0; i < (hx->gain == 128 ? 1 : hx->gain == 64 ? 3 : 2); i++) {
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_SET);
        delay_us(DELAY_US);
        HAL_GPIO_WritePin(hx->sck_port, hx->sck_pin, GPIO_PIN_RESET);
        delay_us(DELAY_US);
    }

    // Sign extend (24 bits → 32 bits)
    if (data & 0x800000) data |= 0xFF000000;

    return data;
}


static bool_t HX711_IsReady(HX711_t *hx) {
    return HAL_GPIO_ReadPin(hx->din_port, hx->din_pin) == GPIO_PIN_RESET;
}




#endif /* API_SRC_HX711_PORT_C_ */
