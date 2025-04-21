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
#include "weight_filter.h"

//Drivers
#include "keypad_driver.h"
#include "keypad_port.h"
#include "lcd_driver.h"

//API
#include "API_delay.h"
#include "API_debounce.h"

static delay_t delayFSM, delayMeasureWeight;

void balanzaInit(void) {
	//Inicializacion del delay us
	DWT_Delay_Init();

	//Inicializacion del delay ms no bloqueante
	delayInit(&delayFSM, DELAY_MEF);
	delayInit(&delayMeasureWeight, DELAY_MEDICION_PESO);

	//Inicializacion del LCD en modo 4bits
    lcdInit();

    //Inicialiacion de la celda de Carga
    weightInit();

    //Inicializacion del keypad
    keypadInit();

    //Inicializo el debounce
	debounceFSM_init();

	// FSM entra en estado inicial y muestra GUI
    balanzaFSM_Init();
}

void balanzaLoop(void) {
//	uint32_t start, end, cycles;
//	static float tiempo_us1,tiempo_us2,tiempo_us3;
	char tecla;

	if( delayRead(&delayMeasureWeight) == true) {
//		start = DWT->CYCCNT;
		float pesoFiltrado = HybridFilterRead(0.8);
		balanzaSetPeso((int32_t)pesoFiltrado / 1000);
//		end = DWT->CYCCNT;
//		cycles = end - start;
//
//		tiempo_us1 = (float)cycles / (HAL_RCC_GetHCLKFreq() / 1e6); // Tiempo en microsegundos
	}
//	start = DWT->CYCCNT;
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
//	end = DWT->CYCCNT;
//	cycles = end - start;
//	tiempo_us2 = (float)cycles / (HAL_RCC_GetHCLKFreq() / 1e6); // Tiempo en microsegundos
	// Ejecutar MEF
	if(delayRead(&delayFSM)==true) {
//		start = DWT->CYCCNT;
		balanzaStateMachine();
//		end = DWT->CYCCNT;
//		cycles = end - start;
//		tiempo_us3 = (float)cycles / (HAL_RCC_GetHCLKFreq() / 1e6); // Tiempo en microsegundos
	}

}


