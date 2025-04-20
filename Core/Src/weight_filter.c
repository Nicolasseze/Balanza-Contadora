/**
 * @file weight_filter.c
 * @brief Módulo de lectura y filtrado de peso desde HX711
 *
 * Este archivo implementa una capa de lectura y filtrado por media móvil
 * para estabilizar el valor de peso leído desde el driver Weight.
 */
#include "weight_filter.h"
#include "string.h"
#include "stdlib.h"

#include "hx711_driver.h"
#include "string.h"

static float buffer[FILTER_WINDOW_SIZE];
static uint8_t indice = 0;
static bool_t filled = false;

void weightInit(void) {
	WeightSensor_t sensor_ctx;

    Weight_Config_t cfg = {
        .port_config = {
            .din_port = HX_DOUT_PORT,
            .din_pin = HX_DOUT_PIN,
            .sck_port = HX_SCK_PORT,
            .sck_pin = HX_SCK_PIN,
            .rate_port = HX_RATE_PORT,
            .rate_pin = HX_RATE_PIN,
            .gain = 0,      // usa WEIGHT_GAIN_DEFAULT
            .rate = 0       // usa WEIGHT_RATE_DEFAULT
        },
        .scale = WEIGHT_SCALE,     // ajustar según calibración
        .offset = WEIGHT_OFFSET      // valor de tara inicial
    };

    WeightDriverInit(&sensor_ctx, &cfg);
    FilterInit();  // Inicializa el buffer de media móvil
}

/**
 * @brief Inicializa el buffer del filtro de peso.
 */
void FilterInit(void) {
    memset(buffer, 0, sizeof(buffer));
    indice = 0;
    filled = false;
}


float MedianFilterRead(void) {
    float raw = WeightRead();
    buffer[indice++] = raw;

    if (indice >= FILTER_WINDOW_SIZE) {
        indice = 0;
        filled = true;
    }

    uint8_t count = filled ? FILTER_WINDOW_SIZE : indice;

    // Copiamos a un buffer temporal para ordenar sin modificar el buffer circular original
    float temp[FILTER_WINDOW_SIZE];
    memcpy(temp, buffer, count * sizeof(float));

    // Función de comparación para qsort
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
