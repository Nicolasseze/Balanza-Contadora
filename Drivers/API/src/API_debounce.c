/*
 * debounce.c
 *
 *  Created on: Mar 29, 2025
 *      Author: nicolas-porco
 */

#include "API_debounce.h"
#include "main.h"

#define TIME_DELAY	40

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

typedef struct{

	debounceState_t state;
	delay_t delay;
	bool_t btnFalling;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} debounceFSM_t;


static debounceFSM_t btn[] = {
		{.GPIOx = B1_GPIO_Port, .GPIO_Pin = B1_Pin}
};
static const uint32_t cantBtn = sizeof( btn ) / sizeof( btn[0] );

void debounceFSM_init( void ){

	debounceFSM_t *pBtn;

	pBtn = btn;

	for( uint8_t i = 0; i < cantBtn; i++ ){
		assert_param( pBtn );

		pBtn->state = BUTTON_UP;
		pBtn->btnFalling = false;
		delayInit( &pBtn->delay, TIME_DELAY);
		pBtn++;
	}

}


void debounceFSM_update( void ){

	debounceFSM_t *pBtn;

	pBtn = btn;

	for( uint8_t i = 0; i < cantBtn; i++ ){

		assert_param( pBtn );

		switch( pBtn->state ){

		case BUTTON_UP:
			if( HAL_GPIO_ReadPin(pBtn->GPIOx, pBtn->GPIO_Pin) == GPIO_PIN_RESET )
				pBtn->state = BUTTON_FALLING;
			break;

		case BUTTON_FALLING:
			if( delayRead( &pBtn->delay ) ){
				if( HAL_GPIO_ReadPin(pBtn->GPIOx, pBtn->GPIO_Pin) == GPIO_PIN_RESET ){
					pBtn->state= BUTTON_DOWN;
					pBtn->btnFalling = true;
				}
				else {
					pBtn->state = BUTTON_UP;
				}
			}
			break;

		case BUTTON_DOWN:
			if( HAL_GPIO_ReadPin(pBtn->GPIOx, pBtn->GPIO_Pin) == GPIO_PIN_SET )
				pBtn->state = BUTTON_RAISING;
			break;

		case BUTTON_RAISING:
			if( delayRead( &pBtn->delay ) ){
				if( HAL_GPIO_ReadPin(pBtn->GPIOx, pBtn->GPIO_Pin) == GPIO_PIN_SET ){
					pBtn->state = BUTTON_UP;
				}
				else {
					pBtn->state = BUTTON_DOWN;
				}
			}
			break;

		default:
			debounceFSM_init();

		}

		pBtn++;
	}
}

bool_t readKey( GPIO_InitTypeDef* GPIOx, uint16_t GPIO_Pin){

	debounceFSM_t *pBtn;

	pBtn = btn;

	if( pBtn->btnFalling == true){
		pBtn->btnFalling = false;
		return true;
	}
	else
		pBtn->btnFalling = false;

	return false;

}
