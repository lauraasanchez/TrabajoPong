#include "pala.h"
#include "bola.h"
#include <stdint.h>

#ifndef INC_JUEGO_H_
#define INC_JUEGO_H_

typedef enum {
    INICIO,      // Pantalla de título "PONG"
    JUEGO,   // La bola se mueve
	PAUSA,
	GANAROJO, //Ganador rojo
	GANAAZUL //Ganador azul
} EstadosJuego;

typedef struct {
    EstadosJuego estadoActual;
    Ball bola;
    Paddle pala1; // Jugador Izquierda
    Paddle pala2; // Jugador Derecha
    uint8_t puntos1;
    uint8_t puntos2;
    uint8_t  timer_activo;     // 1 = Hay cuenta atrás, 0 = No hay
    int8_t   tiempo_restante;  // Segundos que quedan (60, 59...)
    uint32_t ultimo_tick; 
} Juego_t;


void Game_Inicial(void);
void Game_Update(uint16_t adc1, uint16_t adc2); // El main le pasa los sensores
Juego_t* Game_GetStruct(void); // Para que el archivo de dibujo pueda ver los datos

#endif /* INC_JUEGO_H_ */


