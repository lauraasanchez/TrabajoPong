#include "juego.h"
#include "main.h"
#include "puntuacion.h" // Necesario para Score_SetValues

// Variable GLOBAL del juego
static Juego_t juego;

// Devuelve el puntero para que 'display.c' pueda dibujar
Juego_t* Game_GetStruct(void) {
    return &juego;
}

void Game_Inicial(void) {
    juego.estadoActual = INICIO;
    juego.puntos1 = 0;
    juego.puntos2 = 0;

    // INICIALIZAMOS EL TIMER
        juego.timer_activo = 0;   // Apagado al principio
        juego.tiempo_restante = 60; // 1 minuto

    // Inicializamos el marcador a 0-0
    // (Score_Init ya se llamó en main.c, pero no hace daño reiniciarlo aquí)
    Score_SetValues(0, 0);

    // Inicializar objetos del juego
    Ball_Inicial(&juego.bola);
    Paddle_Inicial(&juego.pala1, 5);           // Pala izq
    Paddle_Inicial(&juego.pala2, 320 - 5 - 5); // Pala der
}

void Game_Update(uint16_t adc1, uint16_t adc2) {
    extern volatile uint8_t boton_pulsado_flag;

    // --- ACTUALIZACIÓN DE DATOS PARA EL TIMER ---
    // Le pasamos los puntos actuales al módulo Score.
    // El Timer leerá estos valores automáticamente 200 veces por segundo.
    Score_SetValues(juego.puntos1, juego.puntos2);

        // ============================================================
        // === LÓGICA DEL TEMPORIZADOR "REY DE LA PISTA" ===
        // ============================================================



    // --- MÁQUINA DE ESTADOS ---
    switch (juego.estadoActual) {

        case INICIO:
            if (boton_pulsado_flag == 1) {
                juego.estadoActual = JUEGO;
                boton_pulsado_flag = 0;
            }
            break;

        case JUEGO:

        	// 1. ¿Hay empate? -> APAGAR Y REINICIAR TIMER
        	        if (juego.puntos1 == juego.puntos2) {
        	            juego.timer_activo = 0;
        	            juego.tiempo_restante = 60; // Recargamos para la próxima
        	        }
        	        // 2. ¿Alguien va ganando? -> ENCENDER TIMER
        	        else {
        	            // Si estaba apagado, lo encendemos ahora mismo
        	            if (juego.timer_activo == 0) {
        	                juego.timer_activo = 1;
        	                juego.ultimo_tick = HAL_GetTick(); // Marcamos el tiempo actual
        	            }

        	            // Si ya está activo, comprobamos si ha pasado 1 segundo (1000 ms)
        	            if (HAL_GetTick() - juego.ultimo_tick >= 1000) {
        	                juego.tiempo_restante--;      // Restamos 1 segundo
        	                juego.ultimo_tick = HAL_GetTick(); // Reiniciamos cuenta del segundo

        	                // 3. ¿SE ACABÓ EL TIEMPO? -> ¡VICTORIA!
        	                if (juego.tiempo_restante <= 0) {

        	                	juego.timer_activo = 0;

        	                    if (juego.puntos1 > juego.puntos2) {

        	                        juego.estadoActual = GANAAZUL;

        	                    } else {

        	                        juego.estadoActual = GANAROJO;
        	                    }
        	                    return;
        	                }
        	            }
        	        }
            if (boton_pulsado_flag == 1) {
                juego.estadoActual = PAUSA;
                boton_pulsado_flag = 0;
            }
            else {
                // 1. Actualizar Palas
                Paddle_Update(&juego.pala1, adc1);
                Paddle_Update(&juego.pala2, adc2);

                // 2. Mover Bola
                Ball_Update(&juego.bola);

                // 3. DETECTAR COLISIONES CON PALAS
                // -- Pala 1 (Izquierda) --
                if (juego.bola.x <= (juego.pala1.x + juego.pala1.ancho) &&
                    juego.bola.y >= juego.pala1.y &&
                    juego.bola.y <= (juego.pala1.y + juego.pala1.alto)) {

                    juego.bola.dx = -juego.bola.dx;
                    juego.bola.x = juego.pala1.x + juego.pala1.ancho + 2;
                }

                // -- Pala 2 (Derecha) --
                if (juego.bola.x >= juego.pala2.x &&
                    juego.bola.y >= juego.pala2.y &&
                    juego.bola.y <= (juego.pala2.y + juego.pala2.alto)) {

                    juego.bola.dx = -juego.bola.dx;
                    juego.bola.x = juego.pala2.x - 2;
                }

                // 4. DETECTAR GOL
                // Al salir por la izquierda (Gol del Rojo/J2)
                if (juego.bola.x < 0) {
                    juego.puntos2++;

                    // Feedback visual (LED)
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

                    // PAUSA DE GOL:
                    // Ahora usamos HAL_Delay normal. El Timer interrumpirá
                    // este delay para refrescar el display, así que no se apagará.
                    HAL_Delay(2000);

                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
                    Ball_Reset(&juego.bola);
                }

                // Al salir por la derecha (Gol del Azul/J1)
                if (juego.bola.x > 320) {
                    juego.puntos1++;

                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

                    HAL_Delay(200); // Pausa normal

                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
                    Ball_Reset(&juego.bola);
                }

                // 5. CONDICIÓN DE VICTORIA
                if (juego.puntos1 >= 5) {
                	juego.timer_activo = 0;
                    juego.estadoActual = GANAAZUL;
                }
                if (juego.puntos2 >= 5) {
                	juego.timer_activo = 0;
                    juego.estadoActual = GANAROJO;
                }
            }
            break;

        case PAUSA:
            if (boton_pulsado_flag == 1) {
                juego.estadoActual = JUEGO;
                boton_pulsado_flag = 0;
            }
            break;

        case GANAROJO:
            if (boton_pulsado_flag == 1) {
                boton_pulsado_flag = 0;
                juego.puntos1 = 0;
                juego.puntos2 = 0;
                juego.estadoActual = JUEGO;
            }
            break;

        case GANAAZUL:
            if (boton_pulsado_flag == 1) {
                boton_pulsado_flag = 0;
                juego.puntos1 = 0;
                juego.puntos2 = 0;
                juego.estadoActual = JUEGO;
            }
            break;
    }
}
