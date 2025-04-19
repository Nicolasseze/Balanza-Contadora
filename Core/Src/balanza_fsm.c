/*
 * balanza_fsm.c
 *
 *  Created on: Apr 18, 2025
 *      Author: nicolas-porco
 */

#include "balanza_fsm.h"
#include "gui.h"

static BalanzaContexto_t ctx = {
    .estadoActual = EST_GUI_INICIO,
    .evento = EVT_NINGUNO,

};

// Cambiar estado y mostrar GUI asociada
static void cambiarEstado(EstadoBalanza_t nuevo) {
    ctx.estadoActual = nuevo;
    ctx.evento = EVT_NINGUNO;

    switch (nuevo) {
    case EST_GUI_INICIO:
    	guiMostrarInicio();
    	break;
    case EST_MENU_PRINCIPAL:
    	guiMostrarMenuPrincipal();
    	break;
    case EST_PESANDO:
    	if(ctx.tara)
    		guiMostrarPesando(ctx.pesoActual-ctx.pesoTara, ctx.tara);
    	else
    		guiMostrarPesando(ctx.pesoActual, ctx.tara);
    	break;
    case EST_TARA:
    	guiMostrarTara(ctx.pesoActual);
    	break;
    case EST_CONTEO_POSITIVO:
    	guiMostrarReferenciaConteo("Positivo");
    	break;
    case EST_CONTEO_NEGATIVO:
    	guiMostrarReferenciaConteo("Negativo");
    	break;
    case EST_CONTEO_SET:
    	guiMostrarReferenciaConteo("Ingrese valor en gr");
    	break;
    case EST_CONTEO:
    	// En 000 deberia de estar el total almacenado de las piezas contadas
    	guiMostrarConteo(ctx.piezasContadas, ctx.pesoReferencia, 0);
    	break;
    case EST_CALIB_OFFSET:
    	guiMostrarCalibrandoOffset();
    	break;
    case EST_CALIB_GANANCIA1:
    	guiMostrarCalibrandoGanancia();
    	break;
    case EST_CALIB_GANANCIA2:

    	break;
    case EST_CALIB_CONFIRMA:
    	guiMostrarConfirmacionCalibracion();
    	break;
    case EST_ERROR:
    	guiMostrarError("Fallo? Mal ahi");
    	break;
    default:
    	break;
    }
}

void balanzaFSM_Init(void) {

	//Inicializo
	ctx.pesoActual = 150;
	ctx.pesoReferencia = 10;
	ctx.piezasContadas = 75;
	ctx.pesoTara = 100;

	guiMostrarInicio();
	HAL_Delay(1500);
	cambiarEstado(EST_MENU_PRINCIPAL);

}


void balanzaStateMachine(void) {
//	if (ctx.evento == EVT_NINGUNO)
//		return;

	switch (ctx.estadoActual) {

	case EST_MENU_PRINCIPAL:
//		if (ctx.evento == EVT_OK) {
//			int opcion = opcionSeleccionada();
//			if (opcion == 0) cambiarEstado(EST_CALIB_OFFSET);
//			else if (opcion == 1) cambiarEstado(EST_TARA);
//			else if (opcion == 2) cambiarEstado(EST_CONTEO_REFERENCIA);
//		}
		if(ctx.evento == EVT_TECLA){
			if( ctx.tecla == 'A')
				cambiarEstado(EST_PESANDO);
			else if(ctx.tecla == 'B')
				cambiarEstado(EST_TARA);
			else if(ctx.tecla == 'C')
				cambiarEstado(EST_CONTEO_POSITIVO);
		}
		break;

	case EST_PESANDO:
		if(ctx.evento == EVT_CANCELAR) {
			cambiarEstado(EST_MENU_PRINCIPAL);
			break;
		}
		if(ctx.evento == EVT_OK)
				ctx.tara = ctx.tara ? false : true;

		if(ctx.tara)
			guiPesandoUpdate(ctx.pesoActual-ctx.pesoTara, ctx.tara);
		else
			guiPesandoUpdate(ctx.pesoActual, ctx.tara);
		break;

	case EST_TARA:
		if (ctx.evento == EVT_CANCELAR) {
			cambiarEstado(EST_MENU_PRINCIPAL);
		}
		else if(ctx.evento == EVT_OK){
			ctx.pesoTara = ctx.pesoActual;
			cambiarEstado(EST_MENU_PRINCIPAL);
			break;
		}
		guiTaraUpdate(ctx.pesoActual);
		break;

	case EST_CONTEO_POSITIVO:
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_NEGATIVO);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		if(ctx.evento == EVT_CANCELAR)
			cambiarEstado(EST_MENU_PRINCIPAL);
		break;

	case EST_CONTEO_NEGATIVO:
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_SET);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		if(ctx.evento == EVT_CANCELAR)
			cambiarEstado(EST_MENU_PRINCIPAL);
		break;

	case EST_CONTEO_SET:
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_POSITIVO);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		if(ctx.evento == EVT_CANCELAR)
			cambiarEstado(EST_MENU_PRINCIPAL);
		break;

	case EST_CONTEO:
		break;

        default:
            break;
    }

    ctx.evento = EVT_NINGUNO;
}

// Setter externo para eventos
void balanzaSetEvento(EventoBalanza_t e) {
    ctx.evento = e;
}

// Setter externo para peso
void balanzaSetPeso(uint32_t peso) {
    ctx.pesoActual = peso;
}

void balanzaSetTecla(char tecla){
	ctx.tecla = tecla;
}

