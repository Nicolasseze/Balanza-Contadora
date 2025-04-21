/**
 * @file hx711_port.c
 * @brief Implementación de la capa de abstracción de hardware para el ADC HX711.
 *
 * Este módulo configura y accede directamente a los pines GPIO del microcontrolador
 * para controlar el conversor HX711, utilizado en aplicaciones de medición de peso
 * con celdas de carga. Incluye funciones para inicialización, lectura de datos crudos,
 * y configuración de ganancia y frecuencia de muestreo.
 */

#include "hx711_port.h"

/// @brief Factor de ganancia actual (128 o 64)
static uint8_t gain_factor = HX711_GAIN_DEFAULT;

/**
 * @brief Lee el estado lógico del pin DOUT.
 *
 * @return GPIO_PIN_SET si está en alto, GPIO_PIN_RESET si está en bajo.
 */
static inline GPIO_PinState HX711PortReadDOUT(void) {
    return HAL_GPIO_ReadPin(HX_DOUT_PORT, HX_DOUT_PIN);
}

/**
 * @brief Verifica si el HX711 tiene una conversión lista.
 *
 * @return true si el pin DOUT está en bajo (dato disponible).
 */
static inline bool_t HX711PortIsReady(void) {
    return HX711PortReadDOUT() == GPIO_PIN_RESET;
}

/**
 * @brief Coloca el pin SCK en nivel alto.
 */
static inline void HX711PortSCK_High(void) {
    HAL_GPIO_WritePin(HX_SCK_PORT, HX_SCK_PIN, GPIO_PIN_SET);
}

/**
 * @brief Coloca el pin SCK en nivel bajo.
 */
static inline void HX711PortSCK_Low(void) {
    HAL_GPIO_WritePin(HX_SCK_PORT, HX_SCK_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Inicializa los pines utilizados por el HX711.
 *
 * Configura los GPIO de SCK y DOUT, y opcionalmente el pin RATE si está definido.
 * También configura la ganancia y frecuencia por defecto.
 */
void HX711PortInit(void) {
    // Habilitar retardo en microsegundos
    DWT_Delay_Init();

    GPIO_InitTypeDef gpio = {0};

    // Inicializar pin SCK como salida push-pull
    gpio.Pin = HX_SCK_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX_SCK_PORT, &gpio);
    HAL_GPIO_WritePin(HX_SCK_PORT, HX_SCK_PIN, GPIO_PIN_RESET);

    // Inicializar pin DOUT como entrada
    gpio.Pin = HX_DOUT_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HX_DOUT_PORT, &gpio);

    // Inicializar pin RATE si está presente
    if (HX_RATE_PORT != NULL && HX_RATE_PIN != 0xFFFF) {
        gpio.Pin = HX_RATE_PIN;
        gpio.Mode = GPIO_MODE_OUTPUT_PP;
        gpio.Pull = GPIO_NOPULL;
        gpio.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(HX_RATE_PORT, &gpio);
    }

    // Aplicar configuración de frecuencia y ganancia por defecto
    HX711PortSetRate(fq_10hz);
    HX711PortSetGain(HX711_GAIN_DEFAULT);
}

/**
 * @brief Configura la ganancia del canal A.
 *
 * Solo se admiten 128 o 64 como valores válidos. Otros valores serán
 * ignorados y se aplicará la ganancia por defecto (128).
 *
 * @param gain Valor de ganancia (128 o 64).
 * @return Ganancia efectiva aplicada.
 */
uint8_t HX711PortSetGain(uint8_t gain) {
    if (gain == HX711_GAIN_128 || gain == HX711_GAIN_64) {
        gain_factor = gain;
    } else {
        gain_factor = HX711_GAIN_DEFAULT;
    }

    return gain_factor;
}

/**
 * @brief Configura la frecuencia de muestreo del HX711.
 *
 * Esta función controla el pin RATE para seleccionar entre 10 Hz y 80 Hz.
 * Si el pin RATE no está definido, la función no tiene efecto.
 *
 * @param rate Frecuencia deseada (fq_10hz o fq_80hz).
 * @return Frecuencia efectiva aplicada.
 */
FreqRate_t HX711PortSetRate(FreqRate_t rate) {
    if (HX_RATE_PORT == NULL || HX_RATE_PIN == 0xFFFF)
        return fq_10hz;

    if (rate == fq_10hz)
        HAL_GPIO_WritePin(HX_RATE_PORT, HX_RATE_PIN, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(HX_RATE_PORT, HX_RATE_PIN, GPIO_PIN_SET);

    return rate;
}

/**
 * @brief Realiza una lectura de 24 bits desde el HX711.
 *
 * Espera a que el conversor esté listo, luego realiza la secuencia de reloj
 * para leer los 24 bits del dato. Finalmente, agrega los pulsos necesarios
 * para fijar la ganancia y extiende el signo a 32 bits.
 *
 * @return Valor leído (formato signed 32 bits).
 */
int32_t HX711PortReadRaw(void) {

    // Esperar a que el HX711 indique que el dato está listo
    while (!HX711PortIsReady());

    int32_t data = 0;

    // Leer 24 bits MSB-first
    for (uint8_t i = 0; i < 24; i++) {
        HX711PortSCK_High();
        delay_us(HX711_DELAY_US);
        data = (data << 1) | (int32_t)(HX711PortReadDOUT() == GPIO_PIN_SET);
        HX711PortSCK_Low();
        delay_us(HX711_DELAY_US);
    }

    // Enviar pulsos adicionales según la ganancia seleccionada
    uint8_t pulses = (gain_factor == HX711_GAIN_128) ? 1 :
                     (gain_factor == HX711_GAIN_64)  ? 3 : 2;

    for (uint8_t i = 0; i < pulses; i++) {
        HX711PortSCK_High();
        delay_us(HX711_DELAY_US);
        HX711PortSCK_Low();
        delay_us(HX711_DELAY_US);
    }

    // Extensión de signo para convertir 24 bits a 32 bits con signo
    if (data & 0x800000)
        data |= 0xFF000000;

    return data;
}




