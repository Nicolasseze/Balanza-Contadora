/*
 * weight_filter.h
 *
 *  Created on: Apr 20, 2025
 *      Author: nicolas-porco
 */

#ifndef INC_WEIGHT_FILTER_H_
#define INC_WEIGHT_FILTER_H_

#include <stdint.h>
#include "stdbool.h"

#include "stm32f4xx_hal.h"

#define FILTER_WINDOW_SIZE 10

// Reemplazá estos defines por los pines reales que uses
#define HX_DOUT_PORT   GPIOA
#define HX_DOUT_PIN    GPIO_PIN_6
#define HX_SCK_PORT    GPIOA
#define HX_SCK_PIN     GPIO_PIN_7
#define HX_RATE_PORT   0
#define HX_RATE_PIN    0

//
#define WEIGHT_SCALE 	1.0f
#define WEIGHT_OFFSET	0

typedef bool bool_t;

/**
 * @brief Inicializa la balanza y el filtro de peso.
 */
void weightInit(void);

/**
 * @brief Inicializa el buffer del filtro de media móvil.
 */
void FilterInit(void);

float MedianFilterRead(void);

#endif /* INC_WEIGHT_FILTER_H_ */
