/*
 * debounce.c
 *
 *  Created on: Mar 29, 2025
 *      Author: nicolas-porco
 */

#include "API_debounce.h"

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

typedef struct{
	char name;
	uint8_t row;
	uint8_t col;
} btn_t;


typedef struct{
	debounceState_t state;
	delay_t delay;
	bool_t btnFalling;
	btn_t btn;
} debounceFSM_t;

#define TOTAL_KEYS	(KEYPAD_ROWS * KEYPAD_COLS)
#define ASCII_SIZE 	128

const char keypadKeymap[KEYPAD_ROWS][KEYPAD_COLS] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
};

static debounceFSM_t botones[TOTAL_KEYS];
static const uint32_t cantBtn = TOTAL_KEYS;

static int8_t btnLookup[ASCII_SIZE];

static bool_t readBtn(char letra){
	int8_t index = btnLookup[(uint8_t)letra];
	if (index >= 0) {
		return keypadReadKey(botones[index].btn.row, botones[index].btn.col);
	}
	return false;
}

void debounceFSM_init( void ){

	uint8_t i;
	for (i = 0; i < ASCII_SIZE; i++) {
        btnLookup[i] = -1;
    }

    i = 0;
	for(uint8_t row = 0; row < KEYPAD_ROWS; row++){
		for (uint8_t col = 0; col < KEYPAD_COLS; col++){
			char letra = keypadKeymap[row][col];
			botones[i].btn.name = letra;
			botones[i].btn.row = row;
			botones[i].btn.col = col;
			botones[i].state = BUTTON_UP;
			botones[i].btnFalling = false;
			delayInit(&botones[i].delay, 40);  // 40 ms antirrebote

            // Guardamos el índice
            btnLookup[(uint8_t)letra] = i;
			i++;
		}
	}
}

char debounceFSM_update( void ){

	char tecla = 0;

	debounceFSM_t *pBtn;

	pBtn = botones;

	for( uint8_t i = 0; i < cantBtn; i++ ){

		assert_param( pBtn );

		switch( pBtn->state ){

		case BUTTON_UP:
			if( readBtn(pBtn->btn.name) == true )
				pBtn->state = BUTTON_FALLING;
			break;

		case BUTTON_FALLING:
			if( delayRead( &pBtn->delay ) ){
				if( readBtn(pBtn->btn.name) == true ){
					pBtn->state= BUTTON_DOWN;
					pBtn->btnFalling = true;
					tecla = pBtn->btn.name;
				}
				else {
					pBtn->state = BUTTON_UP;
				}
			}
			break;

		case BUTTON_DOWN:
			if( readBtn(pBtn->btn.name) == false )
				pBtn->state = BUTTON_RAISING;
			break;

		case BUTTON_RAISING:
			if( delayRead( &pBtn->delay ) ){
				if( readBtn(pBtn->btn.name) == false ){
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

	return tecla;
}

bool_t debounceWasPressed( char boton){

	debounceFSM_t *pBtn;
	int8_t idx = btnLookup[(uint8_t)boton];
	if(idx >= 0)
		pBtn = &botones[idx];
	else
		return false;

	if( pBtn->btnFalling == true){
		pBtn->btnFalling = false;
		return true;
	}
	else
		pBtn->btnFalling = false;

	return false;
}
