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
#include "stdlib.h"

/********** Variables **********/
static BalanzaContexto_t ctx = {
    .estadoActual = EST_GUI_INICIO,
    .evento = EVT_NINGUNO,

};

static conteo_subestado_t subEstadoConteo = SUBMENU_IDLE;
static delay_t delayTaraOk;

/********** Utiles **********/
static uint32_t pesoNeto(void) {
    return ctx.tara ? (ctx.pesoActual - ctx.pesoTara) : ctx.pesoActual;
}

/********** FUNCIONES PRIVADAS ESTADOS **********/
// --- Cambiar de estado ---
static void cambiarEstado(EstadoBalanza_t nuevo);

// Estado: GUI_INICIO
static void fsmInicio(void) {
    guiMostrarInicio();
    HAL_Delay(DELAY_INICIO);
    cambiarEstado(EST_MENU_PRINCIPAL);
}


// Estado: MENU_PRINCIPAL
static void fsmMenuPrincipal(void) {
    if (ctx.evento == EVT_OK) {
        switch (ctx.opcionMenu) {
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
            	ctx.opcionMenu = MENU_OP_PESANDO;
            	break;
        }
    } else if (ctx.evento == EVT_TECLA) {
    	if (ctx.tecla == '8' || ctx.tecla == '6') {
    		ctx.opcionMenu = (ctx.opcionMenu + 1) % MENU_OP_TOTAL;
    		guiMostrarMenu(ctx.opcionMenu);
    	} else if (ctx.tecla == '2' || ctx.tecla == '4') {
    		ctx.opcionMenu = (ctx.opcionMenu + MENU_OP_TOTAL - 1) % MENU_OP_TOTAL;
    		guiMostrarMenu(ctx.opcionMenu);
    	} else if (ctx.tecla == 'A') cambiarEstado(EST_PESANDO); //Acceso directo
    	else if (ctx.tecla == 'B') cambiarEstado(EST_TARA); //Acceso directo
    	else if (ctx.tecla == 'C') cambiarEstado(EST_CONTEO_MENU); //Acceso directo
    }

    ctx.piezasContadas = 0;
    ctx.piezasTotales = 0;
}

// Estado: PESANDO
static void fsmPesando(void) {
    if (ctx.evento == EVT_CANCELAR) {
        ctx.opcionMenu = 0;
        cambiarEstado(EST_MENU_PRINCIPAL);
        return;
    }
    if (ctx.evento == EVT_OK) {
        ctx.tara = !ctx.tara;
    }
    guiPesandoUpdate(pesoNeto(), ctx.tara);
}

// Estado: TARA
static void fsmTara(void) {
    if (ctx.evento == EVT_CANCELAR) {
        ctx.opcionMenu = 0;
        cambiarEstado(EST_MENU_PRINCIPAL);
    } else if (ctx.evento == EVT_OK) {
        ctx.pesoTara = ctx.pesoActual;
        cambiarEstado(EST_TARA_OK);
    } else {
        guiTaraUpdate(ctx.pesoActual);
    }
}

// Estado: TARA_OK
static void fsmTaraOk(void) {
    if (delayRead(&delayTaraOk)) {
        ctx.opcionMenu = 0;
        cambiarEstado(EST_MENU_PRINCIPAL);
    }
}

// Estado: CONTEO_MENU
static void fsmConteoMenu(void) {
    static uint32_t aux;
    static char bufferIngreso[10] = {0}; // hasta 9 dígitos + '\0'
    static uint8_t indexBuffer = 0;

    if (ctx.evento == EVT_CANCELAR) {
    	ctx.opcionMenu = 0;
    	subEstadoConteo = SUBMENU_IDLE;
    	guiCursorOff();
    	cambiarEstado(EST_MENU_PRINCIPAL);
    	return;
    }

    if (ctx.evento == EVT_TECLA && ctx.tecla == 'B' && subEstadoConteo == SUBMENU_IDLE) {
    	ctx.modoConteo = (ctx.modoConteo + 1) % MENU_CONTEO_TOTAL;
    	guiMostrarConteoOpcion((int)ctx.modoConteo);

    } else if(subEstadoConteo == SUBMENU_SET_INGRESO_VALOR && ctx.evento == EVT_TECLA) {
    	if (ctx.tecla >= '0' && ctx.tecla <= '9' && indexBuffer < sizeof(bufferIngreso) - 1) {
    		bufferIngreso[indexBuffer++] = ctx.tecla;
    		bufferIngreso[indexBuffer] = '\0';
    		guiActualizarIngreso(bufferIngreso);  // función para mostrar el buffer parcial
    	}
    	else if (ctx.tecla == 'C' && indexBuffer > 0) {
    		bufferIngreso[--indexBuffer] = '\0';
    		guiActualizarIngreso(bufferIngreso);
    	}
    } else if (ctx.evento == EVT_OK) {
    	switch (ctx.modoConteo) {
    	case MENU_CONTEO_POS:
    		ctx.pesoReferencia = pesoNeto();
    		cambiarEstado(EST_CONTEO);
    		break;
    	case MENU_CONTEO_NEG:
    		switch (subEstadoConteo) {
    		case SUBMENU_IDLE:{
    			guiMostrarConteoNeg1();
    			subEstadoConteo = SUBMENU_NEG_PESAR_TACHO;
    			break;
    		}
    		case SUBMENU_NEG_PESAR_TACHO:
    		{
    			aux = pesoNeto();
    			guiMostrarConteoNeg2();
    			subEstadoConteo = SUBMENU_NEG_TOMAR_MUESTRA;
    			break;
    		}
    		case SUBMENU_NEG_TOMAR_MUESTRA:{
    			aux -= pesoNeto();
    			ctx.pesoReferencia = aux;
    			cambiarEstado(EST_CONTEO);
    			subEstadoConteo = SUBMENU_IDLE;
    			break;
    		}
    		default:
    			subEstadoConteo = SUBMENU_IDLE;
    		}
    		break;
    	case MENU_CONTEO_SET:{
    		if (subEstadoConteo == SUBMENU_IDLE) {
    			memset(bufferIngreso, 0, sizeof(bufferIngreso));
    			indexBuffer = 0;
    			guiMostrarConteoSetIngreso();  // función que deberías definir para mostrar la GUI
    			subEstadoConteo = SUBMENU_SET_INGRESO_VALOR;
    		}
    		else if(subEstadoConteo == SUBMENU_SET_INGRESO_VALOR){
    			ctx.pesoReferencia = atoi(bufferIngreso);
    			guiCursorOff();
    			cambiarEstado(EST_CONTEO);
    			subEstadoConteo = SUBMENU_IDLE;
    		}
    		break;
    	default:
    		}
    	}
    }
}

// Estado: CONTEO
static void fsmConteo(void) {
    if (ctx.evento == EVT_CANCELAR) {
        ctx.opcionMenu = 0;
        cambiarEstado(EST_MENU_PRINCIPAL);
        return;
    }

    if(ctx.pesoReferencia > 0){
    	ctx.piezasContadas = pesoNeto() / ctx.pesoReferencia;
    	if (ctx.evento == EVT_TECLA && ctx.tecla == 'C') {
    		ctx.piezasTotales += ctx.piezasContadas;
    	}
    	guiUpdateConteo(ctx.piezasContadas, ctx.pesoReferencia, ctx.piezasTotales);
    }else{
    	cambiarEstado(EST_ERROR);
    }
}

// --- Cambiar de estado ---
void cambiarEstado(EstadoBalanza_t nuevo) {
    ctx.estadoActual = nuevo;

    switch (nuevo) {
        case EST_GUI_INICIO:
        	fsmInicio();
        	break;
        case EST_MENU_PRINCIPAL:
        	guiMostrarMenuPrincipal(ctx.opcionMenu);
        	break;
        case EST_PESANDO:
        	guiMostrarPesando(pesoNeto(), ctx.tara);
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
        	if(ctx.pesoReferencia != 0)
        		guiMostrarConteo(ctx.piezasContadas, ctx.pesoReferencia, ctx.piezasTotales);
        	else
        		cambiarEstado(EST_ERROR);
        	break;
        case EST_CALIB_OFFSET:
        	guiMostrarCalibrandoOffset();
        	break;
        case EST_CALIB_GANANCIA1:
        	guiMostrarCalibrandoGanancia();
        	break;
        case EST_CALIB_CONFIRMA:
        	guiMostrarConfirmacionCalibracion();
        	break;
        case EST_ERROR:
        	guiMostrarError("Fallo? Mal ahi");
        	break;
        default:
    }
}

// --- Init ---
void balanzaFSM_Init(void) {
    ctx.pesoActual = 150;
    ctx.pesoReferencia = 10;
    ctx.piezasContadas = 75;
    ctx.piezasTotales = 75;
    ctx.pesoTara = 100;
    ctx.modoConteo = MENU_CONTEO_POS;
    ctx.opcionMenu = 0;

    delayInit(&delayTaraOk, 1000);

    cambiarEstado(EST_GUI_INICIO);
}

// --- FSM ---
void balanzaStateMachine(void) {
    switch (ctx.estadoActual) {
        case EST_MENU_PRINCIPAL:
        	fsmMenuPrincipal();
        	break;
        case EST_PESANDO:
        	fsmPesando();
        	break;
        case EST_TARA:
        	fsmTara();
        	break;
        case EST_TARA_OK:
        	fsmTaraOk();
        	break;
        case EST_CONTEO_MENU:
        	fsmConteoMenu();
        	break;
        case EST_CONTEO:
        	fsmConteo();
        	break;
        default:
    }
    ctx.evento = EVT_NINGUNO;
}

// --- Setters externos ---
void balanzaSetEvento(EventoBalanza_t e) {
    ctx.evento = e;
}

void balanzaSetPeso(int32_t peso) {
    ctx.pesoActual = peso;
}

void balanzaSetTecla(char tecla) {
    ctx.tecla = tecla;
}

