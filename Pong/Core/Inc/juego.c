#include "juego.h"
#include "main.h"


// Variable GLOBAL del juego (privada dentro de este archivo)
static Juego_t juego;

// Devuelve el puntero para que 'display.c' pueda dibujar
Juego_t* Game_GetStruct(void) {
    return &juego;
}

void Game_Inicial(void) {
    juego.estadoActual = INICIO;
    juego.puntos1 = 0;
    juego.puntos2 = 0;

    // Inicializar objetos
    Ball_Inicial(&juego.bola);
    Paddle_Inicial(&juego.pala1, 5);          // Pala izq en X=5
    Paddle_Inicial(&juego.pala2, 320 - 5 - 5); // Pala der en X=310
}

void Game_Update(uint16_t adc1, uint16_t adc2) {
	extern volatile uint8_t boton_pulsado_flag;

    // --- MÁQUINA DE ESTADOS ---
    switch (juego.estadoActual) {

        case INICIO:
        	if (boton_pulsado_flag == 1)
        	            {
        	        	    juego.estadoActual = JUEGO;
        	                boton_pulsado_flag = 0;
        	            }

            break;

        case JUEGO:
        	if (boton_pulsado_flag == 1) {
        	    juego.estadoActual = PAUSA; // Cambio de estado
        	    boton_pulsado_flag = 0;     // "Consumimos" la bandera (la bajamos)
        	   }
        	else {
        		 // 1. Actualizar Palas con los ADC
        		            Paddle_Update(&juego.pala1, adc1);
        		            Paddle_Update(&juego.pala2, adc2);

        		            // 2. Mover Bola
        		            Ball_Update(&juego.bola);

        		            // 3. DETECTAR COLISIONES (Lo que faltaba en bola.c)

        		            // -- Colisión Pala 1 (Izquierda) --
        		            // Si la bola está a la izquierda Y dentro de la altura de la pala
        		            if (juego.bola.x <= (juego.pala1.x + juego.pala1.ancho) &&
        		                juego.bola.y >= juego.pala1.y &&
        		                juego.bola.y <= (juego.pala1.y + juego.pala1.alto)) {

        		                juego.bola.dx = -juego.bola.dx; // Rebotar horizontal
        		                juego.bola.x = juego.pala1.x + juego.pala1.ancho + 2; // Despegar bola
        		            }

        		            // -- Colisión Pala 2 (Derecha) --
        		            if (juego.bola.x >= juego.pala2.x &&
        		                juego.bola.y >= juego.pala2.y &&
        		                juego.bola.y <= (juego.pala2.y + juego.pala2.alto)) {

        		                juego.bola.dx = -juego.bola.dx;
        		                juego.bola.x = juego.pala2.x - 2;
        		            }

        		            // 4. DETECTAR GOL (Salida por los lados)
        		            if (juego.bola.x < 0) {
        		                juego.puntos2++; // Punto para el jugador 2
        		                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        		                HAL_Delay(200);
        		                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

        		                Ball_Reset(&juego.bola); // Saque de centro
        		            }
        		            if (juego.bola.x > 320) {
        		                juego.puntos1++; // Punto para el jugador 1
        		                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
        		                HAL_Delay(200);
        		               	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
        		                Ball_Reset(&juego.bola);
        		            }

        		            // 5. CONDICIÓN DE VICTORIA (Ejemplo a 5 puntos)
        		            if (juego.puntos1 >= 5) {
        		                juego.estadoActual = GANAAZUL;
        		            }
        		            if (juego.puntos2 >= 5) {
        		                juego.estadoActual = GANAROJO;
        		               }



        	}

            break;

        case PAUSA:


                    // Solo escuchamos el botón para salir
                    if (boton_pulsado_flag == 1) {
                        juego.estadoActual = JUEGO; // Volvemos a la acción
                        boton_pulsado_flag = 0;     // Bajamos la bandera
                    }
                    break;


        case GANAROJO:
        	 if (boton_pulsado_flag == 1) {
        	     boton_pulsado_flag = 0;
        	     juego.puntos1 = 0;
        	     juego.puntos2 = 0;// Bajamos la bandera
        	     juego.estadoActual = JUEGO;
        	 }


            break;

        case GANAAZUL:
        	 if (boton_pulsado_flag == 1) {
        	    boton_pulsado_flag = 0;    // Bajamos la bandera
        	    juego.puntos1 = 0;
        	     juego.puntos2 = 0;
        	     juego.estadoActual = JUEGO;
        	  }


        	break;
    }
}
