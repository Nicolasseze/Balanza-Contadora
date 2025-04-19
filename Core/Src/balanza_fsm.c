/*
 * balanza_fsm.c
 *
 *  Created on: Apr 18, 2025
 *      Author: nicolas-porco
 */

#include "balanza_fsm.h"
#include "gui.h"

#include "API_delay.h"

#include "lcd_driver.h"
#include "hx711_driver.h"
#include "keypad_driver.h"

static BalanzaContexto_t ctx = {
    .estadoActual = EST_GUI_INICIO,
    .evento = EVT_NINGUNO,

};

static conteo_subestado_t subEstadoConteo = SUBMENU_IDLE;

static delay_t delayTaraOk;

static void conteoMenuFSM(void){

	static uint32_t aux;
	//Actualizo modo contador
	if(ctx.evento == EVT_TECLA && ctx.tecla == 'B' && subEstadoConteo == SUBMENU_IDLE){
		ctx.modoConteo ++;
		ctx.modoConteo %= MENU_CONTEO_TOTAL;
		guiMostrarConteoOpcion((int)ctx.modoConteo);
	}else if(ctx.evento == EVT_OK){
		switch( ctx.modoConteo ){
		case MENU_CONTEO_POS:
			ctx.pesoReferencia = ctx.tara? ctx.pesoActual-ctx.pesoTara : ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
			break;
		case MENU_CONTEO_NEG:
			switch(subEstadoConteo) {
			case SUBMENU_IDLE:
				guiMostrarConteoNeg1();
				subEstadoConteo = SUBMENU_NEG_PESAR_TACHO;
				break;
			case SUBMENU_NEG_PESAR_TACHO:
				aux = ctx.tara? ctx.pesoActual - ctx.pesoTara : ctx.pesoActual;
				guiMostrarConteoNeg2();
				subEstadoConteo = SUBMENU_NEG_TOMAR_MUESTRA;
				break;
			case SUBMENU_NEG_TOMAR_MUESTRA:
				aux -= ctx.tara? ctx.pesoActual - ctx.pesoTara : ctx.pesoActual;
				ctx.pesoReferencia = aux;
				cambiarEstado(EST_CONTEO);
				subEstadoConteo = SUBMENU_IDLE;
				break;
			default:
				subEstadoConteo = SUBMENU_IDLE;
			}
			break;
		case MENU_CONTEO_SET:
			break;
		}
	}
}

// Cambiar estado y mostrar GUI asociada
static void cambiarEstado(EstadoBalanza_t nuevo) {
    ctx.estadoActual = nuevo;

    switch (nuevo) {
    case EST_GUI_INICIO:
    	guiMostrarInicio();
    	break;
    case EST_MENU_PRINCIPAL:
    	guiMostrarMenuPrincipal(MENU_OP_PESANDO);
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
    case EST_TARA_OK:
    	guiTaraOk();
    	break;
    case EST_CONTEO_MENU:
    	guiMostrarConteoMenu(MENU_CONTEO_POS);
    	break;
    case EST_CONTEO:
    	guiMostrarConteo(ctx.piezasContadas, ctx.pesoReferencia, ctx.piezasTotales);
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
	/* INICIO PRUEBA */
	ctx.pesoActual = 150;
	ctx.pesoReferencia = 10;
	ctx.piezasContadas = 75;
	ctx.pesoTara = 100;
	ctx.modoConteo = MENU_CONTEO_POS;
	/* FIN PRUEBA */

	guiMostrarInicio();
	HAL_Delay(1500);
	cambiarEstado(EST_MENU_PRINCIPAL);

	delayInit(&delayTaraOk, 1000);

}


void balanzaStateMachine(void) {

//	if (ctx.evento == EVT_NINGUNO)
//		return;

	//Para opciones del manu principal
	static int opcionActual = 0;

	switch (ctx.estadoActual) {

	//LISTA O ESO CREO
	case EST_MENU_PRINCIPAL:
		if(ctx.evento == EVT_OK){
			switch(opcionActual){
			case MENU_OP_PESANDO:
				cambiarEstado(EST_PESANDO);
				break;
			case MENU_OP_TARA:
				cambiarEstado(EST_TARA);
				break;
			case MENU_OP_CONTEO:
				cambiarEstado(EST_CONTEO_MENU);
				break;
			case MENU_OP_CALIBRACION:
				cambiarEstado(EST_CALIB_OFFSET);
				break;
			case MENU_OP_CONFIG_UART:
				cambiarEstado(EST_CONFIGURACION);
				break;
			default:
				opcionActual = MENU_OP_PESANDO;
			}
		}
		else if(ctx.evento == EVT_TECLA){
			if(ctx.tecla == '8' || ctx.tecla == '6'){ // tecla para bajar
				opcionActual = (opcionActual + 1) % MENU_OP_TOTAL;
				guiMostrarMenu(opcionActual);
			}
			else if(ctx.tecla == '2' || ctx.tecla == '4'){ // tecla para subir
				opcionActual = (opcionActual + MENU_OP_TOTAL - 1) % MENU_OP_TOTAL;
				guiMostrarMenu(opcionActual);
			}
			else if( ctx.tecla == 'A')
				cambiarEstado(EST_PESANDO);
			else if(ctx.tecla == 'B')
				cambiarEstado(EST_TARA);
			else if(ctx.tecla == 'C')
				cambiarEstado(EST_CONTEO_MENU);
		}
		//Reseteo contexto
		ctx.piezasContadas = 0;
		ctx.piezasTotales = 0;
		break;

	//	LISTA O ESO CREO
	case EST_PESANDO:
		if(ctx.evento == EVT_CANCELAR) {
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
			break;
		}
		if(ctx.evento == EVT_OK)
			ctx.tara = ctx.tara ? false : true;

		if(ctx.tara)
			guiPesandoUpdate(ctx.pesoActual-ctx.pesoTara, ctx.tara);
		else
			guiPesandoUpdate(ctx.pesoActual, ctx.tara);
		break;

	//	LISTA O ESO CREO
	case EST_TARA:
		if (ctx.evento == EVT_CANCELAR) {
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
			break;
		}
		else if(ctx.evento == EVT_OK){
			ctx.pesoTara = ctx.pesoActual;
			cambiarEstado(EST_TARA_OK);
			break;
		}
		guiTaraUpdate(ctx.pesoActual);
		break;

	case EST_TARA_OK:
		if(delayRead(&delayTaraOk) == true){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		}
		break;

	case EST_CONTEO_MENU:
		if(ctx.evento == EVT_CANCELAR){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		} else
			conteoMenuFSM();
		break;


/*
	case EST_CONTEO_POSITIVO:
		if(ctx.evento == EVT_CANCELAR){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		}
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_NEGATIVO);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.tara? ctx.pesoActual-ctx.pesoTara : ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		break;

	case EST_CONTEO_NEGATIVO:
		if(ctx.evento == EVT_CANCELAR){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		}
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_SET);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		break;

	case EST_CONTEO_SET:
		if(ctx.evento == EVT_CANCELAR){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		}
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'B')
			cambiarEstado(EST_CONTEO_POSITIVO);
		if(ctx.evento == EVT_OK){
			ctx.pesoReferencia = ctx.pesoActual;
			cambiarEstado(EST_CONTEO);
		}
		break;
*/
	case EST_CONTEO:
		if(ctx.evento == EVT_CANCELAR){
			cambiarEstado(EST_MENU_PRINCIPAL);
			opcionActual = 0;
		}
		ctx.piezasContadas = ( ctx.tara? (ctx.pesoActual-ctx.pesoTara) : ctx.pesoActual) / ctx.pesoReferencia;
		if(ctx.evento == EVT_TECLA && ctx.tecla == 'C'){
				ctx.piezasTotales += ctx.piezasContadas;
		}
		guiMostrarConteo( ctx.piezasContadas, ctx.pesoReferencia, ctx.piezasTotales);
		break;

	default:
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

