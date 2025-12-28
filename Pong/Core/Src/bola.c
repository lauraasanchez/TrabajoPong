#include "bola.h"

void Ball_Inicial(Ball *b){
	//Valores iniciales
	b->x=160;
	b->y=5;
	b->dx=2;
	b->dy=2;
	b->radio=3;
}

void Ball_Update(Ball *b){
	// 1. Actualizar posición
	    b->x += b->dx;
	    b->y += b->dy;

	    // 2. Rebote en borde INFERIOR (240)
	    // Restamos el radio para que rebote al tocar el borde, no el centro
	    if (b->y >= (SCREEN_H - b->radio)) {
	        b->y = SCREEN_H - b->radio; // Corregir posición para que no se quede pegada
	        b->dy = -b->dy;             // Invertir dirección vertical
	    }

	    // 3. Rebote en borde SUPERIOR (0)
	    if (b->y <= b->radio) {
	        b->y = b->radio;            // Corregir posición
	        b->dy = -b->dy;             // Invertir dirección vertical
	    }
}

void Ball_Reset(Ball *b){
	//Reset valores
	b->x=160;
	b->y=5;
	b->dx=2;
	b->dy=2;

}



