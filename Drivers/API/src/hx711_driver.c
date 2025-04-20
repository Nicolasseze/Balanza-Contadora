/**
 * @file hx711_driver.c
 * @brief Lógica de procesamiento para el sensor HX711 (filtro, escala, offset)
 */

#include "hx711_driver.h"
#include "hx711_port.h"
#include <stdlib.h>

static WeightSensor_t sensor_ctx;

void WeightDriverInit(WeightSensor_t* ctx, Weight_Config_t* cfg) {
    if (!ctx || !cfg) return;

    // Convertir HWConfig genérico a estructura del port
    HX711_t port_cfg;
    port_cfg.din_port = (GPIO_TypeDef*)cfg->port_config.din_port;
    port_cfg.din_pin  = cfg->port_config.din_pin;
    port_cfg.sck_port = (GPIO_TypeDef*)cfg->port_config.sck_port;
    port_cfg.sck_pin  = cfg->port_config.sck_pin;
    port_cfg.rate_port = (GPIO_TypeDef*)cfg->port_config.rate_port;
    port_cfg.rate_pin  = cfg->port_config.rate_pin;
    port_cfg.gain = (cfg->port_config.gain == 0) ? WEIGHT_GAIN_DEFAULT : cfg->port_config.gain;
    port_cfg.rate = (cfg->port_config.rate == 0) ? WEIGHT_RATE_DEFAULT : (FreqRate_t)cfg->port_config.rate;

    // Inicializar puerto (GPIO + ganancia + rate)
    HX711_Init(&port_cfg);

    // Guardar configuración interna
    ctx->offset = cfg->offset;
    ctx->scale = (cfg->scale == 0.0f) ? 1.0f : cfg->scale; // evitar división por cero
    ctx->weight = 0.0f;
    sensor_ctx = *ctx;
}

float WeightRead(void) {
    int32_t raw = HX711_ReadRaw();
    sensor_ctx.weight = ((float)(raw - sensor_ctx.offset)) / sensor_ctx.scale;
    return sensor_ctx.weight;
}

void Weight_SetScale(float scale) {
    if (scale != 0.0f)
        sensor_ctx.scale = scale;
}

void Weight_SetOffset(int32_t offset) {
    sensor_ctx.offset = offset;
}

float Weight_GetScale(void) {
    return sensor_ctx.scale;
}

float Weight_CalibrateScale(float peso_known, int32_t offset, int32_t raw) {
    if (peso_known == 0.0f) return 1.0f;  // evitar división por cero
    return (float)(raw - offset) / peso_known;
}

int32_t Weight_GetOffset(void) {
    return sensor_ctx.offset;
}



