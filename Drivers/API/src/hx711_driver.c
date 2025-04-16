/*
 * hx711_driver.c
 *
 *  Created on: Apr 14, 2025
 *      Author: nicolas-porco
 */


#include "hx711_driver.h"
#include "API_delay.h"

static HX711_t hx711;

void initReadWeight ( WeightSensor_t *wx ){

	hx711.din_pin = DinWeight_Pin;
	hx711.din_port = DinWeight_Port;

	hx711.rate_pin = RateWeight_Pin;
	hx711.rate_port = RateWeight_Port;

	hx711.sck_pin = SckWeight_Pin;
	hx711.sck_port = SckWeight_Port;

	wx->offset = OFFSET_WEIGHT;
	wx->scale = SCALE_WEIGHT;
	wx->gain = GAIN_DEFAULT;
	wx->rate = RATE_DEFAULT;

	hx711.gain = wx->gain;
	hx711.rate = wx->rate;

	HX711_Init ( &hx711 );

}

float readWeight (WeightSensor_t *wx){

	float measure;

	measure = (float)(HX711_ReadRaw ( &hx711 ) - wx->offset) / wx->scale;

	if(wx->gain == 64)
		return measure * 2;
	else if(wx->gain == 32)
		return measure * 4;

	return measure;

}

