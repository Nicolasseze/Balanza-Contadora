/*
 * gui.h
 *
 *  Created on: Apr 18, 2025
 *      Author: nicolas-porco
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_

#include <stdint.h>
#include "stdbool.h"

typedef bool bool_t;

/**
 * @brief Muestra la pantalla de inicio
 */
void guiMostrarInicio(void);


void guiMostrarMenu(int opcion);
/**
 * @brief Muestra el menú principal con opciones
 */
void guiMostrarMenuPrincipal(int opcion);

/**
 * @brief Muestra el modo de pesaje libre con peso actual
 * @param pesoKg Peso en gramos
 * @param tara Si esta activa la tara
 */
void guiMostrarPesando(uint32_t pesoGr, bool_t tara);

/**
 * @brief Actualiza el peso en modo pesando libre
 * @param pesoKg Peso en gramos
 * @param tara Si esta activada la tara.
 */
void guiPesandoUpdate(uint32_t pesoGr, bool_t tara);

/**
 * @brief Muestra la pantalla para realizar la tara
 */
void guiMostrarTara(uint32_t pesoGr);


void guiTaraOk(void);


void guiTaraUpdate(uint32_t pesoGr);

/**
 * @brief Muestra la pantalla para colocar la referencia en el conteo
 */
void guiMostrarReferenciaConteo(const char *s);

/**
 * @brief Muestra la pantalla de conteo de piezas
 * @param piezas Cantidad de piezas contadas
 * @param pesoRef Peso de referencia de la muestra
 * @param totales Cantidad de piezas totales acumuladas
 */
void guiMostrarConteo(uint32_t piezas, uint32_t pesoRef, uint32_t totales);

/**
 * @brief Muestra la pantalla de calibración de offset
 */
void guiMostrarCalibrandoOffset(void);

/**
 * @brief Muestra la pantalla para calibrar la ganancia con un peso conocido
 */
void guiMostrarCalibrandoGanancia(void);

/**
 * @brief Muestra la confirmación de calibración exitosa
 */
void guiMostrarConfirmacionCalibracion(void);

/**
 * @brief Muestra un mensaje de error con detalle
 * @param msg Texto adicional explicativo del error
 */
void guiMostrarError(const char* msg);

#endif /* INC_GUI_H_ */
