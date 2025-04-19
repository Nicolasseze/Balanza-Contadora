/**
 * @file balanza.h
 * @brief Prototipos del módulo de alto nivel de la balanza
 */
#ifndef INC_BALANZA_H_
#define INC_BALANZA_H_


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
