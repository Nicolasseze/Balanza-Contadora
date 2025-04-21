/**
 * @file gui.c
 * @brief Módulo encargado de mostrar pantallas completas en el LCD
 *
 * Este archivo encapsula el diseño visual de cada estado del sistema.
 * Todas las funciones están pensadas para usarse desde la FSM sin lógica de control.
 */
#include <stdio.h>
#include <string.h>

#include "gui.h"
#include "lcd_driver.h"

#define DISPLAY_WIDTH		LCD_WIDTH
#define BUFFER_LEN          (DISPLAY_WIDTH + 1)
#define MENU_VISIBLE_LINEAS	3

/** @brief Opciones del menú principal */
static const char* menuOpciones[] = {
	    "1-Pesar",
	    "2-Tarear",
	    "3-Contar",
	    "4-Calibracion",
	    "5-Config UART"
};

/** @brief Comentarios del menu funcion conteo */
static const char *menuConteo[] = {
		"Positivo",
		"Negativo",
		"Seteo de muestra"
};

/** @brief Resumen del menu funcion conteo */
static const char *menuConteoBrief[] = {
		"Coloque 1 pieza",
		"Coloque el total pzs",
		"Ingrese valor en gr",
};

#define MENU_TOTAL_OPCIONES (sizeof(menuOpciones)/sizeof(menuOpciones[0]))

/********** Funciones privadas de impresión **********/

static void printCentered(uint8_t row, const char* str) {
    char buffer[BUFFER_LEN];
    size_t len = strlen(str);

    if (len >= DISPLAY_WIDTH) {
        strncpy(buffer, str, DISPLAY_WIDTH);
    } else {
        size_t padding = (DISPLAY_WIDTH - len) / 2;
        memset(buffer, ' ', DISPLAY_WIDTH);
        memcpy(buffer + padding, str, len);
    }

    buffer[DISPLAY_WIDTH] = '\0';
    lcdShowLine(row, buffer);
}

static void printSubmenu(uint8_t row, const char* left, const char* right) {
    char buffer[BUFFER_LEN];  // +1 para el '\0'
    size_t len1 = strlen(left);
    size_t len2 = strlen(right);

    if ((len1 + len2) > DISPLAY_WIDTH)
        return;

    memset(buffer, ' ', DISPLAY_WIDTH);  // Inicializa todo en espacios
    memcpy(buffer, left, len1);      // Copia left al principio
    memcpy(buffer + (DISPLAY_WIDTH - len2), right, len2);  // Copia right al final

    buffer[DISPLAY_WIDTH] = '\0';  // Null terminator

    lcdShowLine(row, buffer);  // Imprime toda la línea
}

static void printYesNo(bool_t condition) {
    lcdPrint(condition ? "SI" : "NO");
}

/********** Pantallas **********/

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
        		DISPLAY_WIDTH - 2, menuOpciones[idx]);  // -2 por flecha y espacio
        lcdShowLine(i + 1, buffer);
    }
}

void guiMostrarMenuPrincipal(int opcion) {
    lcdClear();
    printCentered(0, " Balanza Contadora ");
    guiMostrarMenu(opcion);
}

void guiMostrarPesando(uint32_t pesoGr, bool_t tara) {
    char buf[BUFFER_LEN];
    lcdClear();
    printCentered(0, "Modo: Pesando");

    snprintf(buf, sizeof(buf), "Peso: %lu g", pesoGr);
    lcdShowLine(1, buf);

    snprintf(buf, sizeof(buf), "Tara: ");
    lcdShowLine(2, buf);
    lcdSetCursor(2, 6);
    printYesNo(tara);

    printSubmenu(3, "[*]Menu","[#]Tara");
}

void guiPesandoUpdate(uint32_t pesoGr, bool_t tara){
    char buf[BUFFER_LEN];
    snprintf(buf, sizeof(buf), "%lu g", pesoGr);

    lcdClearRange(1, 6, DISPLAY_WIDTH - 1);
    lcdSetCursor(1, 6);
    lcdPrint(buf);

    lcdSetCursor(2, 6);
    printYesNo(tara);
}

void guiMostrarTara(uint32_t pesoGr) {
    char buf[BUFFER_LEN];
    lcdClear();
    printCentered(0, "Modo: Tara");
    lcdShowLine(1, "Coloque recipiente");

    snprintf(buf, sizeof(buf), "Tara: %lu g", pesoGr);
    lcdShowLine(2, buf);
    printSubmenu(3, "[*]Menu", "[#]Tara");
}

void guiTaraUpdate(uint32_t pesoGr) {
    char buf[BUFFER_LEN];
    snprintf(buf, sizeof(buf), "%lu g", pesoGr);
    lcdClearRange(2, 6, DISPLAY_WIDTH - 1);  // Limpia desde columna 6 hasta 19
    lcdSetCursor(2, 6);
    lcdPrint(buf);
}

void guiTaraOk(void){
	lcdClear();
	printCentered(1,"TARA exitosa");
}

void guiMostrarConteoOpcion(int opcion){
	char buf[BUFFER_LEN];
	snprintf(buf, sizeof(buf),"%-*s", DISPLAY_WIDTH, menuConteoBrief[opcion]);
    lcdShowLine(1, buf);
 	snprintf(buf, sizeof(buf),"[B] %-*s", DISPLAY_WIDTH-4, menuConteo[opcion]);
    lcdShowLine(2, buf);
}

void guiMostrarConteoMenu(int opcion){
    lcdClear();
    printCentered(0, "Modo: Contador");
    guiMostrarConteoOpcion(opcion);
    printSubmenu(3, "[*]Menu","[#]OK");
}

void guiMostrarConteoNeg1(void){
	lcdClear();
	printCentered(0, "Contador Negativo");
//				   "                    "
	lcdShowLine(1, "Por favor ponga to-");
	lcdShowLine(2, "das las piezas.");
    printSubmenu(3, "[*]Menu","[#]OK");
}

void guiMostrarConteoNeg2(void){
	lcdClear();
	printCentered(0, "Contador Negativo");
//				   "                    "
	lcdShowLine(1, "Retire una pieza y");
	lcdShowLine(2, "presione OK.");
    printSubmenu(3, "[*]Menu","[#]OK");
}

void guiMostrarConteoSetIngreso(void){
	lcdClear();
	printCentered(0, "Contador Set Valor");
//				   "                    "
	lcdShowLine(1, "Ingrese un valor:");
    printSubmenu(3, "[C]Borrar","[#]OK");
}

void guiActualizarIngreso(const char *str){
	lcdClearRange(2, 0, DISPLAY_WIDTH-1);
	lcdPrintConCursor(2, 0, str);
}

void guiCursorOff(void){
	lcdOffCursor();
}

void guiMostrarConteo(uint32_t piezas, uint32_t pesoRef, uint32_t totales) {
    char buf[BUFFER_LEN];
	lcdClear();
    snprintf(buf, sizeof(buf), "Peso Muestra: %lu g", pesoRef);
    lcdShowLine(0, buf);
    snprintf(buf, sizeof(buf), "Cantidad: %lu", piezas);
    lcdShowLine(1, buf);
    snprintf(buf, sizeof(buf), "Total: %lu", totales);
    lcdShowLine(2, buf);
    printSubmenu(3, "[*]Menu","[C]Suma");
}

void guiUpdateConteo(uint32_t piezas, uint32_t pesoRef, uint32_t totales) {
    char buf[BUFFER_LEN];
    lcdClearRange(0, 14, DISPLAY_WIDTH-1);
    snprintf(buf, sizeof(buf), "%lu g", pesoRef);
    lcdSetCursor(0, 14);
    lcdPrint(buf);
    lcdClearRange(1, 10, DISPLAY_WIDTH-1);
    snprintf(buf, sizeof(buf), "%lu", piezas);
    lcdSetCursor(1, 10);
    lcdPrint(buf);
    lcdClearRange(2, 6, DISPLAY_WIDTH);
    snprintf(buf, sizeof(buf), "Total: %lu", totales);
    lcdSetCursor(2, 6);
    lcdPrint(buf);
}




//****************************** Hasta aca supongo que estoy corrigiendo
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
