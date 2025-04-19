/*
 * hx711_port.h
 *
 *  Created on: Apr 14, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_HX711_PORT_H_
#define API_INC_HX711_PORT_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"

typedef bool bool_t;

typedef enum rate{
	fq_10hz,
	fq_80hz,

} FreqRate_t;

typedef struct {
    GPIO_TypeDef* din_port;
    uint16_t din_pin;

    GPIO_TypeDef* sck_port;
    uint16_t sck_pin;

    GPIO_TypeDef* rate_port;
    uint16_t rate_pin;

    uint8_t gain;  // 128, 64 (32 no va a ser usada porque esta en otro canal)
    FreqRate_t rate;

    int32_t data;

} HX711_t;

void HX711_Init(HX711_t *hx);

int32_t HX711_ReadRaw(HX711_t *hx);


#endif /* API_INC_HX711_PORT_H_ */
