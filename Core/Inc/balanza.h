/**
 * @file balanza.h
 * @brief Prototipos del módulo de alto nivel de la balanza
 */
#ifndef INC_BALANZA_H_
#define INC_BALANZA_H_

#define DELAY_MEDICION_PESO		100 //Delay no bloqueante para 10Hz de medicion de peso
#define DELAY_MEF				100 //Delay no bloqueante para manejar maquina de estados

/**
 * @brief Inicializa todos los módulos necesarios y la FSM
 */
void balanzaInit(void);

/**
 * @brief Ejecuta el ciclo principal del sistema
 *
 * Debe ser llamado periódicamente desde el main loop.
 */
void balanzaLoop(void);

#endif /* INC_BALANZA_H_ */
