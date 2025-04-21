/**
 * @file hx711_driver.h
 * @brief Interfaz de alto nivel para procesamiento de datos del HX711.
 *
 * Este módulo proporciona funciones para inicializar el sensor HX711,
 * obtener lecturas crudas o calibradas, y configurar parámetros como
 * offset (tara) y factor de escala. Se basa en una capa inferior que
 * accede al hardware, permitiendo un diseño desacoplado y mantenible.
 */

#ifndef API_INC_HX711_DRIVER_H_
#define API_INC_HX711_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Definición de tipo booleano para compatibilidad.
 */
typedef bool bool_t;

/**
 * @brief Inicializa el driver lógico del HX711.
 *
 * Esta función configura el sensor con parámetros por defecto y deja el
 * sistema listo para comenzar a leer datos. Asume que la capa de puerto
 * ya contiene los pines correctamente definidos.
 *
 * @return true si la inicialización fue exitosa, false en caso de error.
 */
bool_t hx711Init(void);

/**
 * @brief Obtiene la lectura cruda actual del HX711.
 *
 * Devuelve el valor de 24 bits leído directamente del sensor, extendido
 * a 32 bits con signo.
 *
 * @return Valor crudo actual del sensor (en bits).
 */
int32_t hx711ReadRaw(void);

/**
 * @brief Obtiene el peso medido, compensado y escalado.
 *
 * Aplica el offset y la escala configurados para devolver el peso en
 * unidades físicas (por ejemplo, gramos o kilogramos).
 *
 * @return Peso actual medido, como número de punto flotante.
 */
float hx711ReadWeight(void);

/**
 * @brief Establece el offset a aplicar a la lectura cruda.
 *
 * Este valor se resta a cada lectura cruda para compensar la tara.
 *
 * @param offset Valor crudo a utilizar como tara.
 */
void hx711SetOffset(int32_t offset);

/**
 * @brief Obtiene el offset actualmente aplicado.
 *
 * @return Valor de offset en bits crudos.
 */
int32_t hx711GetOffset(void);

/**
 * @brief Establece el factor de escala para convertir a unidades físicas.
 *
 * El valor leído (menos el offset) se multiplica por este factor para
 * obtener el peso final.
 *
 * @param scale Factor de conversión (por ejemplo, gramos/bit).
 */
void hx711SetScale(float scale);

/**
 * @brief Obtiene el factor de escala actual.
 *
 * @return Valor actual del factor de conversión aplicado.
 */
float hx711GetScale(void);

#endif /* API_INC_HX711_DRIVER_H_ */

