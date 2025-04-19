/**
 * @file balanza_fsm.c
 * @brief Máquina de estados finita para la balanza contadora
 *
 * Autor: Nicolás Porco
 */


#ifndef INC_BALANZA_FSM_H_
#define INC_BALANZA_FSM_H_

#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"

typedef bool bool_t;

// Estados de la MEF
typedef enum {
    EST_GUI_INICIO = 0,
    EST_MENU_PRINCIPAL,
    EST_PESANDO,
    EST_TARA,
	EST_TARA_OK,
    EST_CONTEO_POSITIVO,
    EST_CONTEO_NEGATIVO,
    EST_CONTEO_SET,
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
    EVT_TECLA,
	EVT_NUMERO,
	EVT_PESO_INESTABLE,
    EVT_ERROR_SENSOR,
} EventoBalanza_t;

//Opciones menu principal
typedef enum {
    MENU_OP_PESANDO = 0,
    MENU_OP_TARA,
    MENU_OP_CONTEO,
	MENU_OP_CALIBRACION,
	MENU_OP_CONFIG_UART,
    MENU_OP_TOTAL // <- No es una opción, solo indica la cantidad
} menu_opciones_t;

// Contexto de la balanza
typedef struct {
    EstadoBalanza_t estadoActual;
    EventoBalanza_t evento;
    char tecla;
    uint32_t pesoActual;
    uint32_t pesoTara;
    bool_t tara;
    uint32_t pesoReferencia;
    uint32_t piezasContadas;
    uint32_t piezasTotales;
} BalanzaContexto_t;

void balanzaFSM_Init(void);

void balanzaStateMachine(void);

// Setter externo para peso
void balanzaSetPeso(uint32_t peso);

void balanzaSetEvento(EventoBalanza_t e);

void balanzaSetTecla(char tecla);

#endif /* INC_BALANZA_FSM_H_ */
