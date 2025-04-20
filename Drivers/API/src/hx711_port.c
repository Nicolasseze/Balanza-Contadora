/**
 * @file hx711_port.c
 * @brief Implementación de acceso de bajo nivel al sensor HX711.
 *
 * Este módulo maneja la configuración de pines, comunicación serial
 * y lectura raw del convertidor de carga HX711.
 */

#include "hx711_port.h"
#include "stm32f4xx_hal.h"

static HX711_t hx711;

static bool_t HX711_IsReady(void) {
    return HAL_GPIO_ReadPin(hx711.din_port, hx711.din_pin) == GPIO_PIN_RESET;
}

void HX711_Init(HX711_t *hx) {
	assert_param(hx != NULL);

	// Guardar puntero de configuración en el contexto interno
	hx711.din_port   = hx->din_port;
	hx711.din_pin    = hx->din_pin;
	hx711.sck_port   = hx->sck_port;
	hx711.sck_pin    = hx->sck_pin;
	hx711.rate_port  = hx->rate_port;
	hx711.rate_pin   = hx->rate_pin;

	// Inicializa pines
	GPIO_InitTypeDef gpio = {0};

	// SCK → salida push-pull
	gpio.Pin = hx711.sck_pin;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(hx711.sck_port, &gpio);
	HAL_GPIO_WritePin(hx711.sck_port, hx711.sck_pin, GPIO_PIN_RESET);

	// DOUT → entrada
	gpio.Pin = hx711.din_pin;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(hx711.din_port, &gpio);

	// RATE → solo si está definido
	if (hx711.rate_port != NULL && hx711.rate_pin != 0xFFFF) {
		gpio.Pin = hx711.rate_pin;
		gpio.Mode = GPIO_MODE_OUTPUT_PP;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(hx711.rate_port, &gpio);
	}

	// Setear configuración de frecuencia y ganancia (una vez que los pines están inicializados)
	HX711_SetRate(hx->rate);
	HX711_SetGain(hx->gain);

	// Inicializar delay (DWT u otra fuente de us)
	DWT_Delay_Init();
}

int32_t HX711_ReadRaw(void) {

	while (!HX711_IsReady());  // espera conversión

    int32_t data = 0;

    for (uint8_t i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(hx711.sck_port, hx711.sck_pin, GPIO_PIN_SET);
        delay_us(HX711_DELAY_US);
        data = (data << 1) | HAL_GPIO_ReadPin(hx711.din_port, hx711.din_pin);
        HAL_GPIO_WritePin(hx711.sck_port, hx711.sck_pin, GPIO_PIN_RESET);
        delay_us(HX711_DELAY_US);
    }

    // Pulsos extras para setear ganancia
    for (uint8_t i = 0; i < (hx711.gain == 128 ? 1 : hx711.gain == 64 ? 3 : 2); i++) {
        HAL_GPIO_WritePin(hx711.sck_port, hx711.sck_pin, GPIO_PIN_SET);
        delay_us(HX711_DELAY_US);
        HAL_GPIO_WritePin(hx711.sck_port, hx711.sck_pin, GPIO_PIN_RESET);
        delay_us(HX711_DELAY_US);
    }

    // Sign extend (24 bits → 32 bits)
    if (data & 0x800000) data |= 0xFF000000;

    return data;
}

void HX711_SetGain(uint8_t gain) {
	if (gain == HX711_GAIN_128 || gain == HX711_GAIN_64){
        hx711.gain = gain;
    } else {
        hx711.gain = 128;  // Valor por defecto si se pasa algo inválido
    }
}

void HX711_SetRate(FreqRate_t rate) {
    hx711.rate = rate;

    if (hx711.rate_port == NULL || hx711.rate_pin == 0xFFFF)
        return; // No hay pin configurado, ignorar

    HAL_GPIO_WritePin(hx711.rate_port, hx711.rate_pin,
                      (rate == fq_10hz) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

