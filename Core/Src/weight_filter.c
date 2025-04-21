/**
 * @file weight_filter.c
 * @brief Módulo de lectura y filtrado de peso desde HX711.
 *
 * Este archivo implementa una capa de lectura y filtrado de peso utilizando
 * un filtro de mediana (media móvil ordenada) para estabilizar las lecturas
 * provenientes del sensor HX711. También incorpora una función híbrida que
 * combina mediana + EMA para lograr estabilidad y respuesta rápida.
 */

#include "weight_filter.h"
#include "string.h"
#include "stdlib.h"

/// Buffer circular que almacena las últimas N muestras para el filtro de mediana
static float buffer[FILTER_WINDOW_SIZE];

/// Índice actual de escritura en el buffer circular
static uint8_t indice = 0;

/// Flag que indica si el buffer ya fue llenado al menos una vez
static bool_t filled = false;

/**
 * @brief Inicializa el filtro de mediana.
 *
 * Limpia el buffer de muestras y reinicia el índice de escritura.
 * Esta función es utilizada internamente por `weightInit()`.
 */
static void filterInit(void) {
    memset(buffer, 0, sizeof(buffer));
    indice = 0;
    filled = false;
}

/**
 * @brief Inicializa el sensor de peso y el sistema de filtrado.
 *
 * Llama a la función de inicialización del HX711 (`hx711Init`) y
 * luego configura el buffer del filtro de mediana.
 *
 * Debe ser llamada una vez al inicio del programa.
 */
void weightInit(void) {
    hx711Init();
    filterInit();  // Inicializa el buffer de mediana
}

/**
 * @brief Lee el peso actual aplicando un filtro de mediana.
 *
 * Toma una nueva muestra del sensor HX711 mediante `hx711ReadWeight()`,
 * la almacena en un buffer circular, y calcula la mediana del conjunto
 * de muestras disponibles. Esto elimina outliers y ruido impulsivo.
 *
 * @return Peso filtrado en unidades físicas (por ejemplo, gramos).
 */
float MedianFilterRead(void) {
    float raw = hx711ReadWeight();
    buffer[indice++] = raw;

    if (indice >= FILTER_WINDOW_SIZE) {
        indice = 0;
        filled = true;
    }

    uint8_t count = filled ? FILTER_WINDOW_SIZE : indice;

    // Copiamos a un buffer temporal para ordenar sin modificar el buffer circular original
    float temp[FILTER_WINDOW_SIZE];
    memcpy(temp, buffer, count * sizeof(float));

    /**
     * @brief Función de comparación para ordenar flotantes (usada por qsort).
     */
    int float_cmp(const void* a, const void* b) {
        float fa = *(const float*)a;
        float fb = *(const float*)b;
        return (fa > fb) - (fa < fb);
    }

    qsort(temp, count, sizeof(float), float_cmp);

    // Devolvemos la mediana
    if (count % 2 == 1) {
        return temp[count / 2];
    } else {
        return (temp[(count - 1) / 2] + temp[count / 2]) / 2.0f;
    }
}

/**
 * @brief Lee el peso aplicando un filtro híbrido: mediana + EMA.
 *
 * Combina la robustez de un filtro de mediana con la suavidad de un
 * filtro exponencial (EMA). Ideal para sistemas de medición que requieren
 * estabilidad y respuesta rápida. El valor de `alpha` controla la respuesta
 * del filtro exponencial:
 *
 * - `alpha = 0.2f`: más estable, más lento
 * - `alpha = 0.8f`: más rápido, menos estable
 *
 * @param alpha Coeficiente de suavizado del EMA (entre 0.0 y 1.0).
 * @return Peso filtrado en unidades físicas.
 */
float HybridFilterRead(float alpha) {
    static float ema = 0.0f;
    static bool_t first = true;

    float mediana = MedianFilterRead();

    if (first) {
        ema = mediana;
        first = false;
    } else {
        ema = alpha * mediana + (1.0f - alpha) * ema;
    }

    return ema;
}

