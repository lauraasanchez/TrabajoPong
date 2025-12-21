#include "pala.h"

void Paddle_Inicial(Paddle *p, int x){
		p->x = x;        // Pala derecha o izquierda
	    p->y = 100;      // Posición vertical inicial
	    p->ancho = 5;    //Tamaño pala
	    p->alto  = 40;
}
void Paddle_Update(Paddle *p, uint16_t adc){
	// Convertir ADC a posición Y válida en pantalla
	    p->y = (adc * (SCREEN_H - p->alto)) / 4095;
}

