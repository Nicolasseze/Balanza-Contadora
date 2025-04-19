/*
 * API_debounce.h
 *
 *  Created on: Mar 29, 2025
 *      Author: nicolas-porco
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"

#include "keypad_driver.h"

#define TIME_DELAY	5

/* Typedef -------------------------------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
void debounceFSM_init( void );

char debounceFSM_update( void );

bool_t debounceWasPressed( char boton);



#endif /* API_INC_API_DEBOUNCE_H_ */
