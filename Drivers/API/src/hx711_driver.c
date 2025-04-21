/**
 * @file hx711_driver.c
 * @brief Lógica de procesamiento para el sensor HX711 (filtro, escala, offset).
 *
 * Este módulo implementa la funcionalidad lógica del sensor HX711, incluyendo:
 * - Inicialización de ganancia y frecuencia.
 * - Lectura de datos crudos.
 * - Aplicación de offset (tara) y factor de escala.
 *
 * La lectura del sensor se delega a la capa de puerto (`hx711_port`), mientras que
 * este archivo se enfoca en el procesamiento de datos y calibración.
 */

#include "hx711_driver.h"
#include "hx711_port.h"
#include <string.h> // Para memset

/// Valor de frecuencia de muestreo por defecto
#define RATE_DEFAULT  	fq_10hz

/// Valor de ganancia por defecto (128x)
#define GAIN_DEFAULT	HX711_GAIN_DEFAULT

/// Offset por defecto (sin tara)
#define OFFSET_DEFAULT	0

/// Escala por defecto (1 unidad por bit)
#define SCALE_DEFAULT	2.097152f //1cuenta / mg

/**
 * @brief Contexto interno del driver para mantener estado y calibración.
 */
static struct {
    int32_t offset;         /**< Offset medido durante la tara */
    float scale;            /**< Factor de escala: unidades por bit */
    int32_t last_raw;       /**< Última lectura cruda (raw) */
    uint8_t gain;           /**< Ganancia actual aplicada */
    FreqRate_t rate;        /**< Frecuencia de muestreo actual aplicada */
} ctx;


/**
 * @brief Inicializa el driver lógico del HX711.
 *
 * Configura los valores por defecto de ganancia, frecuencia, offset y escala.
 * Inicializa la capa de puerto y aplica los parámetros iniciales al hardware.
 *
 * @return true si la inicialización fue exitosa, false en caso contrario.
 */
bool_t hx711Init(void) {

	ctx.gain = GAIN_DEFAULT;
	ctx.rate = RATE_DEFAULT;

    ctx.offset = OFFSET_DEFAULT;
    ctx.scale = SCALE_DEFAULT;

    HX711PortInit();
    HX711PortSetGain(ctx.gain);
    HX711PortSetRate(ctx.rate);

    return true;
}

/**
 * @brief Lee el valor crudo directamente del HX711.
 *
 * Guarda y retorna el valor leído del sensor, sin aplicar offset ni escala.
 *
 * @return Lectura cruda del sensor (formato de 24 bits extendido a 32 bits).
 */
int32_t hx711ReadRaw(void) {
    ctx.last_raw = HX711PortReadRaw();
    return ctx.last_raw;
}

/**
 * @brief Establece el valor de offset para la compensación de tara.
 *
 * Este valor se resta a todas las lecturas antes de aplicar la escala.
 *
 * @param offset Valor crudo a utilizar como compensación.
 */
void hx711SetOffset(int32_t offset) {
    ctx.offset = offset;
}

/**
 * @brief Devuelve el valor de offset actualmente aplicado.
 *
 * @return Offset actual configurado (valor crudo).
 */
int32_t hx711GetOffset(void) {
    return ctx.offset;
}


/**
 * @brief Obtiene el peso actual en unidades físicas.
 *
 * Realiza una lectura cruda del sensor HX711, le aplica la compensación de offset (tara),
 * y divide el resultado por el factor de escala para convertirlo a unidades físicas
 * (por ejemplo, gramos).
 *
 * @note Si la escala no fue configurada (igual a cero), devuelve 0.0f.
 *
 * @return Peso calculado como número de punto flotante, o 0.0f si la escala no es válida.
 */
float hx711ReadWeight(void) {
    int32_t raw = hx711ReadRaw();

    if (ctx.scale == 0.0f) {
        // Escala no configurada, evitamos división por cero
        return 0.0f;
    }

    return (raw - ctx.offset) / ctx.scale;
}



