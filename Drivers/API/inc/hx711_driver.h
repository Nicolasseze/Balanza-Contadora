/*
 * hx711_driver.h
 *
 *  Created on: Apr 14, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_HX711_DRIVER_H_
#define API_INC_HX711_DRIVER_H_

#include "hx711_port.h"

#define RateWeight_Pin 	GPIO_PIN_13
#define RateWeight_Port GPIOC
#define DinWeight_Pin 	GPIO_PIN_9
#define DinWeight_Port 	GPIOC
#define SckWeight_Pin 	GPIO_PIN_8
#define SckWeight_Port	GPIOC

#define OFFSET_WEIGHT	0
#define SCALE_WEIGHT	1
#define GAIN_DEFAULT	128
#define RATE_DEFAULT	fq_10hz

typedef struct {
    float weight_kg;       // Último valor medido, en kg
    int32_t offset;        // Valor de tara
    float scale;           // Escala en cuentas por kg
    uint8_t gain;          // Ganancia actual
    FreqRate_t rate;       // Frecuencia actual

} WeightSensor_t;

void initReadWeight ( WeightSensor_t *wx );

float readWeight ( WeightSensor_t *wx );

#endif /* API_INC_HX711_DRIVER_H_ */
