/* score.h */
#ifndef SCORE_H_
#define SCORE_H_

#include "main.h" // Para tener acceso a HAL y uint types

// Inicializa los pines (si no lo has hecho en el main con CubeMX) y variables
void Score_Init(void);

// Actualiza los valores que queremos mostrar (Puntos J1 y J2)
void Score_SetValues(uint8_t p1, uint8_t p2);

// Esta función se debe llamar CONSTANTEMENTE para encender los LEDs (Multiplexación)
void Score_Refresh(void);

#endif /* SCORE_H_ */
