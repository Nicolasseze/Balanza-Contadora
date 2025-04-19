/**
 * @file balanza.c
 * @brief Módulo de alto nivel para el manejo de la lógica principal de la balanza
 *
 * Contiene funciones de inicialización y el loop principal que coordina drivers,
 * entradas de usuario y la máquina de estados finita (FSM).
 */
#include <stdbool.h>

//Core
#include "balanza.h"
#include "balanza_fsm.h"

//Drivers
#include "hx711_driver.h"
#include "keypad_driver.h"
#include "keypad_port.h"
#include "lcd_driver.h"

//API
#include "API_delay.h"
#include "API_debounce.h"

static delay_t delay50;

void balanzaInit(void) {
	//Inicializacion del delay us
	DWT_Delay_Init();

	//Inicializacion del delay ms no bloqueante
	delayInit(&delay50, 50);

	//Inicializacion del LCD en modo 4bits
    lcdInit();

    //Inicialiacion de la celda de Carga
    //hx711Init();

    //Inicializacion del keypad
    keypadPortAttachRow(0, GPIOC, GPIO_PIN_7);
    keypadPortAttachRow(1, GPIOA, GPIO_PIN_9);
    keypadPortAttachRow(2, GPIOA, GPIO_PIN_8);
    keypadPortAttachRow(3, GPIOB, GPIO_PIN_10);
    keypadPortAttachCol(0, GPIOB, GPIO_PIN_4);
    keypadPortAttachCol(1, GPIOB, GPIO_PIN_5);
    keypadPortAttachCol(2, GPIOB, GPIO_PIN_3);
    keypadPortAttachCol(3, GPIOA, GPIO_PIN_10);
    keypadInit();

    //Inicializo el debounce
	debounceFSM_init();

	// FSM entra en estado inicial y muestra GUI
    balanzaFSM_Init();
}

void balanzaLoop(void) {

	char tecla;

	/*
	 * DEBERIA DE LEER EL PESO DE LA BALANZA
	 *
    static uint32_t lastUpdate = 0;
    float peso = hx711LeerPromedioKg();
    balanzaSetPeso(peso);
	*/
	//	if(delayRead(&delay50) == true){
	if(true){
		tecla = debounceFSM_update();
		if(tecla != 0){
			if( tecla == '*' )
				balanzaSetEvento(EVT_CANCELAR);
			else if ( tecla == '#' )
				balanzaSetEvento(EVT_OK);
			else {
				balanzaSetEvento(EVT_TECLA);
				balanzaSetTecla(tecla);
			}
		}
	}

    // Ejecutar MEF
    balanzaStateMachine();

}


