/**
 * @file weight_filter.h
 * @brief Interfaz del módulo de filtrado de peso utilizando el sensor HX711.
 *
 * Este módulo proporciona funciones para inicializar el sistema de medición de peso
 * y obtener una lectura filtrada aplicando un filtro de mediana sobre una ventana
 * de muestras recientes. Se apoya en el driver del HX711 para obtener los datos crudos.
 *
 * @author Nicolás Porco
 * @date April 20, 2025
 */

#ifndef INC_WEIGHT_FILTER_H_
#define INC_WEIGHT_FILTER_H_

#include <stdint.h>
#include "stdbool.h"
#include "hx711_driver.h"

/**
 * @brief Tamaño de la ventana del filtro de mediana.
 *
 * Define cuántas muestras se almacenan para calcular la mediana.
 */
#define FILTER_WINDOW_SIZE 25

/**
 * @brief Definición de tipo booleano para compatibilidad.
 */
typedef bool bool_t;

/**
 * @brief Inicializa el sensor de peso y el sistema de filtrado.
 *
 * Llama a la inicialización del HX711 y limpia el buffer utilizado
 * por el filtro de mediana. Debe llamarse una única vez al inicio.
 */
void weightInit(void);

/**
 * @brief Lee el peso actual aplicando un filtro de mediana.
 *
 * Toma una nueva muestra del sensor HX711, la almacena en un buffer
 * circular y calcula la mediana de las últimas muestras para reducir
 * el ruido y estabilizar la lectura.
 *
 * @return Valor de peso filtrado en unidades físicas (ej. gramos).
 */
float MedianFilterRead(void);

/**
 * @brief Lee el peso aplicando un filtro híbrido: mediana + EMA.
 *
 * Esta función aplica un filtro de mediana para eliminar valores atípicos
 * y luego suaviza el resultado usando un filtro exponencial (EMA). Es útil
 * en sistemas donde se desea buena estabilidad sin perder demasiada velocidad
 * de respuesta ante cambios reales de peso.
 *
 * @param alpha Coeficiente del filtro EMA (entre 0.0 y 1.0).
 *              Valores bajos suavizan más (respuesta más lenta), valores altos responden más rápido.
 *
 * @return Peso filtrado en unidades físicas (por ejemplo, gramos).
 */
float HybridFilterRead(float alpha);

#endif /* INC_WEIGHT_FILTER_H_ */
