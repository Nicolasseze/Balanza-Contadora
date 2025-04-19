/**
 * @file gui.c
 * @brief Módulo encargado de mostrar pantallas completas en el LCD
 *
 * Este archivo encapsula el diseño visual de cada estado del sistema.
 * Todas las funciones están pensadas para usarse desde la FSM sin lógica de control.
 */

#include "gui.h"
#include "lcd_driver.h"
#include <stdio.h>
#include <string.h>

static const char* menuOpciones[] = {
	    "1-Pesar",
	    "2-Tarear",
	    "3-Contar",
	    "4-Calibracion",
	    "5-Config UART"
	};

#define MENU_TOTAL_OPCIONES (sizeof(menuOpciones)/sizeof(menuOpciones[0]))
#define MENU_VISIBLE_LINEAS	3

static void printCentered(uint8_t row, const char* str) {
    char buffer[LCD_WIDTH + 1];
    size_t len = strlen(str);

    if (len > LCD_WIDTH) {
        strncpy(buffer, str, LCD_WIDTH);
        buffer[LCD_WIDTH] = '\0';
    } else {
        size_t padding = (LCD_WIDTH - len) / 2;
        memset(buffer, ' ', LCD_WIDTH);
        memcpy(buffer + padding, str, len);
        buffer[LCD_WIDTH] = '\0';
    }

    lcdShowLine(row, buffer);
}

static void printSubmenu(uint8_t row, const char* str1, const char* str2) {
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

void guiMostrarInicio(void) {
    lcdClear();
    printCentered(0, "Balanza Contadora");
    printCentered(1, "Inicializando...");
    printCentered(3, "Nicolas Porco 2025");
}

void guiMostrarMenu(int opcion){

    // Determinar el primer índice visible (scroll)
    static int inicio = 0;
    // ¿Está fuera de la ventana actual?
    if (opcion < inicio) {
        inicio = opcion;
    } else if (opcion >= inicio + MENU_VISIBLE_LINEAS) {
        inicio = opcion - MENU_VISIBLE_LINEAS + 1;
    }

    for (int i = 0; i < MENU_VISIBLE_LINEAS; i++) {
        int idx = inicio + i;
        if (idx >= MENU_TOTAL_OPCIONES) break;

        char buffer[21];
        snprintf(buffer, sizeof(buffer), "%c %-*s", (idx == opcion) ? '>' : ' ',
        				LCD_WIDTH - 2, menuOpciones[idx]);  // -2 por flecha y espacio
        lcdShowLine(i + 1, buffer);
    }
}

void guiMostrarMenuPrincipal(int opcion) {
    lcdClear();
    printCentered(0, " Balanza Contadora ");
    guiMostrarMenu(opcion);
}

void guiMostrarPesando(uint32_t pesoGr, bool_t tara) {
    char buf[21] = "                    " ;
    lcdClear();
    printCentered(0, "Modo: Pesando");
    snprintf(buf, sizeof(buf), "Peso: %lu g", pesoGr);
    lcdShowLine(1, buf);
    //Aca falta acomodar
    lcdShowLine(2, "Tara: ");
    if(tara)
    	lcdPrint("SI");
    else
    	lcdPrint("NO");
    printSubmenu(3, "[*]Menu","[#]Tara");
}

void guiPesandoUpdate(uint32_t pesoGr, bool_t tara){
	char buf[21] = "              " ;
	snprintf(buf, sizeof(buf), "%lu g", pesoGr);
	lcdSetCursor(1, 6);
	lcdPrint(buf);
	lcdSetCursor(2, 6);
    if(tara)
    	lcdPrint("SI");
    else
    	lcdPrint("NO");
}

void guiMostrarTara(uint32_t pesoGr) {
	char buf[21] = "              ";
    lcdClear();
    printCentered(0, "Modo: Tara");
    lcdShowLine(1, "Coloque recipiente");
    snprintf(buf, sizeof(buf), "Tara: %lu g", pesoGr);
    lcdShowLine(2, buf);
    printSubmenu(3, "[*]Menu","[#]Tara");
}

void guiTaraUpdate(uint32_t pesoGr){
	char buf[21] = "                    " ;
	snprintf(buf, sizeof(buf), "%lu g", pesoGr);
	lcdSetCursor(1, 6);
	lcdPrint(buf);
}

void guiMostrarReferenciaConteo(const char *s) {
	char buf[21]="[B]";
    lcdClear();
    printCentered(0, "Modo: Contador");
    lcdShowLine(1, "Coloque 1 pieza");
    strcat(buf, s);
    lcdShowLine(2, buf);
    printSubmenu(3, "[*]Menu","[#]OK");
}

void guiMostrarConteo(uint32_t piezas, uint32_t pesoRef, uint32_t totales) {
    char linea1[21], linea2[21], linea3[21];

	lcdClear();
    snprintf(linea1, sizeof(linea1), "Peso Muestra: %lu g", pesoRef);
    snprintf(linea2, sizeof(linea2), "Cantidad: %lu", piezas);
    snprintf(linea3, sizeof(linea3), "Total: %lu", totales);
    lcdShowLine(0, linea1);
    lcdShowLine(1, linea2);
    lcdShowLine(2, linea3);
    printSubmenu(3, "[*]Menu","[C]Suma");
}


void guiMostrarCalibrandoOffset(void) {
    lcdClear();
    lcdShowLine(0, "Calibrando Offset");
    lcdShowLine(1, "Quite peso del plato");
    lcdShowLine(2, "Presione [#]OK");
    lcdShowLine(3, "[*] Cancelar");
}

void guiMostrarCalibrandoGanancia(void) {
    lcdClear();
    lcdShowLine(0, "Calib: Ganancia");
    lcdShowLine(1, "Coloque peso conocido");
    lcdShowLine(2, "[#] Confirmar");
    lcdShowLine(3, "[*] Cancelar");
}

void guiMostrarConfirmacionCalibracion(void) {
    lcdClear();
    lcdShowLine(0, "Calibracion OK");
    lcdShowLine(1, " [#] Volver al menu");
}

void guiMostrarError(const char* msg) {
    lcdClear();
    lcdShowLine(0, "!! ERROR DE SENSOR !!");
    lcdShowLine(1, msg);
    lcdShowLine(2, "[#] Volver al menu");
}
