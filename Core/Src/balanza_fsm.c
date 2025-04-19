/*
 * balanza_fsm.c
 *
 *  Created on: Apr 18, 2025
 *      Author: nicolas-porco
 */

#include "balanza_fsm.h"

static BalanzaContexto_t ctx = {
    .estadoActual = EST_GUI_INICIO,
    .evento = EVT_NINGUNO
};

static void lcdSubmenu(uint8_t row, const char* str1, const char* str2);

// Cambiar estado y mostrar GUI asociada
void cambiarEstado(EstadoBalanza_t nuevo) {
    ctx.estadoActual = nuevo;
    ctx.evento = EVT_NINGUNO;

    switch (nuevo) {
    case EST_GUI_INICIO:
    	lcdClear();
    	lcdShowCentered(0, "Balanza Contadora");
    	lcdShowLine(2, "Nicolas Porco 2025");
    	HAL_Delay(1500);
    	lcdClear();
        case EST_MENU_PRINCIPAL:
            lcdClear();
            lcdShowCentered(0, "Balanza Contadora");
            lcdShowLine(1, "> 1-Calibrar");
            lcdShowLine(2, "  2-Tarear");
            lcdShowLine(3, "  3-Contar");
            break;
        case EST_PESANDO:
            lcdClear();
            lcdShowLine(0, "Modo: Pesando");
            lcdShowLine(1, "Peso: ");
            lcdShowLine(2, "Tara: ");
            lcdSubmenu(3, "[*]Menu","[#]Tara");
            break;
        case EST_TARA:
            lcdClear();
            lcdShowLine(0, "Modo: Tareando");
            lcdShowLine(1, "Coloque recipiente");
            lcdShowLine(2, "Peso Tara: ");
            lcdSubmenu(3, "[*]Menu","[#]Tara");
            break;
        case EST_CONTEO_MENU1:
            lcdClear();
            lcdShowLine(0, "Modo: Contador");
            lcdShowLine(1, "Coloque 1 pieza");
            lcdShowLine(2, "[B]Positivo");
            lcdSubmenu(3, "[*]Menu","[#]OK");
            break;
        case EST_CONTEO_MENU2:
            lcdClear();
            lcdShowLine(0, "Modo: Contador");
            lcdShowLine(1, "Coloque 1 pieza");
            lcdShowLine(2, "[B]Negativo");
            lcdSubmenu(3, "[*]Menu","[#]OK");
            break;
        case EST_CONTEO_MENU3:
            lcdClear();
            lcdShowLine(0, "Modo: Contador");
            lcdShowLine(1, "Coloque 1 pieza");
            lcdShowLine(2, "[B]Ingrese valor");
            lcdSubmenu(3, "[*]Menu","[#]OK");
            break;
        case EST_CONTEO:
            lcdClear();
            lcdShowLine(0, "Peso Muestra: ");
            lcdShowLine(1, "Cantidad: ");
            lcdShowLine(2, "Total: ");
            lcdSubmenu(3, "[*]Menu","[C]Suma");
            break;
        case EST_CALIB_OFFSET:
            lcdClear();
            lcdShowLine(0, "Calibrando Offset");
            lcdShowLine(1, "Quite peso del plato");
            lcdShowLine(2, "Presione [#]OK");
            lcdShowLine(3, "[*] Cancelar");
            break;
        case EST_CALIB_GANANCIA1:
            lcdClear();
            lcdShowLine(0, "Calib: Ganancia");
            lcdShowLine(1, "Coloque peso conocido");
            lcdShowLine(2, "[#] Confirmar");
            lcdShowLine(3, "[*] Cancelar");
            break;
        case EST_CALIB_GANANCIA2:
            lcdClear();
            lcdShowLine(0, "Calib: Ganancia");
            lcdShowLine(1, "Referencia: ");
            lcdShowLine(2, "[#] Confirmar");
            lcdShowLine(3, "[*] Cancelar");
            break;
        case EST_CALIB_CONFIRMA:
            lcdClear();
            lcdShowLine(0, "Calibracion OK");
            lcdShowLine(1, "[OK] Volver");
            break;
        case EST_ERROR:
            lcdClear();
            lcdShowCentered(0, " ERROR DE SENSOR");
            lcdShowCentered(1, " Verifique conexiones");
            lcdShowCentered(2, " [*] Menu Principal");
            break;
        default:
            break;
    }
}

static void lcdSubmenu(uint8_t row, const char* str1, const char* str2) {
    char buffer[LCD_WIDTH + 1];  // +1 para el '\0'
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if ((len1 + len2) > LCD_WIDTH)
        return;

    memset(buffer, ' ', LCD_WIDTH);  // Inicializa todo en espacios
    memcpy(buffer, str1, len1);      // Copia str1 al principio
    memcpy(buffer + (LCD_WIDTH - len2), str2, len2);  // Copia str2 al final

    buffer[LCD_WIDTH] = '\0';  // Null terminator

    lcdShowLine(row, buffer);  // Imprime toda la línea
}

// Máquina de estados principal
/*
void balanzaStateMachine(void) {
    if (ctx.evento == EVT_NINGUNO)
        return;

    switch (ctx.estadoActual) {
        case EST_MENU_PRINCIPAL:
            if (ctx.evento == EVT_OK) {
                int opcion = opcionSeleccionada();
                if (opcion == 0) cambiarEstado(EST_CALIB_OFFSET);
                else if (opcion == 1) cambiarEstado(EST_TARA);
                else if (opcion == 2) cambiarEstado(EST_CONTEO_REFERENCIA);
            }
            break;

        case EST_CALIB_OFFSET:
            if (ctx.evento == EVT_OK) {
                hx711Tarear();
                cambiarEstado(EST_CALIB_GANANCIA);
            }
            break;

        case EST_CALIB_GANANCIA:
            if (ctx.evento == EVT_OK) {
                // Suponé que leímos el peso conocido
                ctx.pesoReferencia = hx711LeerPromedioKg();
                cambiarEstado(EST_CALIB_CONFIRMA);
            }
            break;

        case EST_CALIB_CONFIRMA:
            if (ctx.evento == EVT_OK) {
                guardarCalibracion(ctx.pesoReferencia);
                cambiarEstado(EST_MENU_PRINCIPAL);
            }
            break;

        case EST_TARA:
            if (ctx.evento == EVT_OK) {
                hx711Tarear();
                cambiarEstado(EST_MENU_PRINCIPAL);
            }
            break;

        case EST_PESANDO:
            if (ctx.evento == EVT_PESO_ESTABLE) {
                char buf[21];
                snprintf(buf, 21, "Peso: %.2f kg", ctx.pesoActual);
                lcdShowLine(1, buf);
            } else if (ctx.evento == EVT_CANCELAR) {
                cambiarEstado(EST_MENU_PRINCIPAL);
            }
            break;

        case EST_CONTEO_REFERENCIA:
            if (ctx.evento == EVT_OK) {
                ctx.pesoReferencia = hx711LeerPromedioKg();
                cambiarEstado(EST_CONTEO_ACUMULANDO);
            }
            break;

        case EST_CONTEO_ACUMULANDO:
            if (ctx.evento == EVT_PESO_ESTABLE) {
                ctx.pesoActual = hx711LeerPromedioKg();
                ctx.piezasContadas = (uint16_t)(ctx.pesoActual / ctx.pesoReferencia);
                char buf[21];
                snprintf(buf, 21, "Total: %u piezas", ctx.piezasContadas);
                lcdShowLine(1, buf);
            } else if (ctx.evento == EVT_FIN_CONTEO) {
                cambiarEstado(EST_CONTEO_RESULTADO);
            }
            break;

        case EST_CONTEO_RESULTADO:
            if (ctx.evento == EVT_OK || ctx.evento == EVT_CANCELAR) {
                cambiarEstado(EST_MENU_PRINCIPAL);
            }
            break;

        case EST_ERROR:
            if (ctx.evento == EVT_OK || ctx.evento == EVT_CANCELAR) {
                cambiarEstado(EST_MENU_PRINCIPAL);
            }
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
void balanzaSetPeso(float peso) {
    ctx.pesoActual = peso;
}
*/

