/**
 * @file balanza_fsm.c
 * @brief Máquina de estados finita para la balanza contadora
 *
 * Autor: Nicolás Porco
 */


#ifndef INC_BALANZA_FSM_H_
#define INC_BALANZA_FSM_H_

#include "lcd_driver.h"
#include "hx711_driver.h"
#include "keypad_driver.h"
#include <stdio.h>

// Estados de la MEF
typedef enum {
    EST_GUI_INICIO = 0,
    EST_MENU_PRINCIPAL,
    EST_PESANDO,
    EST_TARA,
    EST_CONTEO_MENU1,
    EST_CONTEO_MENU2,
    EST_CONTEO_MENU3,
	EST_CONTEO,
    EST_CALIB_OFFSET,
    EST_CALIB_GANANCIA1,
	EST_CALIB_GANANCIA2,
	EST_CALIB_CONFIRMA,
    EST_ERROR,
    EST_CONFIGURACION
} EstadoBalanza_t;

// Eventos del sistema
typedef enum {
    EVT_NINGUNO,
    EVT_OK,
    EVT_CANCELAR,
    EVT_SUBIR,
    EVT_BAJAR,
    EVT_PESO_ESTABLE,
    EVT_ERROR_SENSOR,
    EVT_TIMEOUT,
    EVT_FIN_CONTEO
} EventoBalanza_t;

// Contexto de la balanza
typedef struct {
    EstadoBalanza_t estadoActual;
    EventoBalanza_t evento;
    float pesoActual;
    float pesoReferencia;
    uint16_t piezasContadas;
} BalanzaContexto_t;

void balanzaStateMachine(void);

#endif /* INC_BALANZA_FSM_H_ */
