#include "display.h"
#include "ILI9341_STM32_Driver.h" // Tus drivers de pantalla
#include "ILI9341_GFX.h"
#include <stdio.h>

// Variables para borrar la posición anterior (evitar parpadeo)
static Ball bola_ant;
static Paddle pala1_ant, pala2_ant;
static int estadoAnterior = -1;

void Display_Inicial(void) {
    ILI9341_Init();
    ILI9341_FillScreen(WHITE);
    ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
    // Inicializar "anteriores" fuera de pantalla para que la primera vez dibuje bien
    bola_ant.x = -10;
}

void Display_Update(Juego_t *juego) {

	if (juego->estadoActual != estadoAnterior) {

	        // 1. Borramos la pantalla completa (para limpiar lo anterior)
	        ILI9341_FillScreen(WHITE);

	        // 2. Pintamos lo estático según el NUEVO estado
	        if (juego->estadoActual == INICIO) {
	        	ILI9341_FillScreen(WHITE);
	            // Pintamos el título UNA VEZ y ya se queda ahí fijo
	            ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
	            ILI9341_DrawText("PONG", FONT4, 125, 100, BLACK, WHITE);
	            ILI9341_DrawText("Pulsa boton", FONT3, 110, 150, RED, WHITE);
	        }
	        else if (juego->estadoActual == GANAROJO) {
	        	ILI9341_FillScreen(RED);
	             ILI9341_DrawText("GANA ROJO", FONT4, 90, 110, WHITE, RED);

	        }
	        else if (juego->estadoActual == GANAAZUL) {
	        	ILI9341_FillScreen(BLUE);
	             ILI9341_DrawText("GANA AZUL", FONT4, 90, 110, WHITE, BLUE);

	        }
	        else if(juego->estadoActual == DIFICULTAD){
	        	ILI9341_FillScreen(WHITE);
	        	ILI9341_DrawText("FACIL", FONT4, 128, 85, BLACK, WHITE);
	        	ILI9341_DrawText("DIFICIL", FONT4, 124, 135, RED, WHITE);
	        }
	        else if(juego->estadoActual == PAUSA){
	        	ILI9341_FillScreen(WHITE);
	        	  ILI9341_DrawText("PULSA PARA CONTINUAR", FONT4, 50, 85, BLACK, WHITE);
	        	  ILI9341_DrawText("REINICIAR", FONT4, 124, 135, RED, WHITE);
	         }
	        else if(juego->estadoActual == MENUREINICIO){
	        	ILI9341_FillScreen(WHITE);
	        	 ILI9341_DrawText("REINCIAR", FONT4, 117, 85, BLACK, WHITE);
	        	 ILI9341_DrawText("MENU", FONT4, 124, 135, RED, WHITE);
	        }

	        // 3. Actualizamos la memoria para no volver a entrar aquí hasta que cambie algo
	        estadoAnterior = juego->estadoActual;
	    }


	    if (juego->estadoActual == JUEGO) {

	    	char texto[5];
	    	        if (juego->timer_activo == 1) {
	    	            sprintf(texto, "%02d", juego->tiempo_restante);

	    	            // Verde si sobra tiempo, Rojo si queda poco (menos de 10s)
	    	            uint16_t color = (juego->tiempo_restante > 10) ? 0x07E0 : 0xF800; // 0x07E0=Verde, 0xF800=Rojo

	    	            // Dibujar Texto: X=140, Y=10. Fondo BLANCO (WHITE o 0xFFFF)
	    	            ILI9341_DrawText(texto, FONT4, 140, 10, color, WHITE);
	    	        }
	    	        else {
	    	            // Borrar el área del timer si no está activo
	    	            // Coordenadas: X0=140, Y0=10, X1=180, Y1=35
	    	            ILI9341_DrawFilledRectangleCoord(140, 10, 180, 35, WHITE);
	    	        }


	        // 1. BORRAR posiciones anteriores (
	        // Usamos las variables _ant para saber dónde estaban
	        ILI9341_DrawFilledCircle(bola_ant.x, bola_ant.y, bola_ant.radio, WHITE);
	        ILI9341_DrawRectangle(pala1_ant.x, pala1_ant.y, pala1_ant.ancho, pala1_ant.alto, WHITE);
	        ILI9341_DrawRectangle(pala2_ant.x, pala2_ant.y, pala2_ant.ancho, pala2_ant.alto, WHITE);

	        // 2. DIBUJAR posiciones nuevas
	        ILI9341_DrawFilledCircle(juego->bola.x, juego->bola.y, juego->bola.radio, BLACK);
	        ILI9341_DrawRectangle(juego->pala1.x, juego->pala1.y, juego->pala1.ancho, juego->pala1.alto, BLUE);
	        ILI9341_DrawRectangle(juego->pala2.x, juego->pala2.y, juego->pala2.ancho, juego->pala2.alto, RED);

	        // 3. MOSTRAR PUNTUACIÓN (Opcional, pero recomendado)
	        // Podrías dibujarla solo si cambia, pero por simplicidad:
	        // (Aquí podrías poner un DrawChar pequeño en las esquinas)

	        // 4. ACTUALIZAR "anteriores" para el siguiente ciclo
	        bola_ant = juego->bola;
	        pala1_ant = juego->pala1;
	        pala2_ant = juego->pala2;
	    }


}
