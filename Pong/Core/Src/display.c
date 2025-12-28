#include "display.h"
#include "ILI9341_STM32_Driver.h" // Tus drivers de pantalla
#include "ILI9341_GFX.h"
#include <stdio.h>
#include "fonts.h"    // <--- Necesario para usar Font_11x18


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
	/*
	if (juego->estadoActual == INICIO){
		ILI9341_FillScreen(WHITE);
		  ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
		  ILI9341_DrawText("PONG", FONT4, 140, 110, BLACK, WHITE);
		  ILI9341_FillScreen(WHITE);
	}

    // Solo dibujamos si estamos en JUEGO
    if (juego->estadoActual == JUEGO) {

        // 1. BORRAR posiciones anteriores (pintando de blanco)

        ILI9341_DrawFilledCircle(bola_ant.x, bola_ant.y, bola_ant.radio, WHITE);
        ILI9341_DrawRectangle(pala1_ant.x, pala1_ant.y, pala1_ant.ancho, pala1_ant.alto, WHITE);
        ILI9341_DrawRectangle(pala2_ant.x, pala2_ant.y, pala2_ant.ancho, pala2_ant.alto, WHITE);

        // 2. DIBUJAR posiciones nuevas (pintando de negro)
        ILI9341_DrawFilledCircle(juego->bola.x, juego->bola.y, juego->bola.radio, BLACK);
        ILI9341_DrawRectangle(juego->pala1.x, juego->pala1.y, juego->pala1.ancho, juego->pala1.alto, BLACK);
        ILI9341_DrawRectangle(juego->pala2.x, juego->pala2.y, juego->pala2.ancho, juego->pala2.alto, BLACK);

        // 3. ACTUALIZAR "anteriores" para el siguiente ciclo
        bola_ant = juego->bola;  // Copia la estructura entera
        pala1_ant = juego->pala1;
        pala2_ant = juego->pala2;
        */
	if (juego->estadoActual != estadoAnterior) {

	        // 1. Borramos la pantalla completa (para limpiar lo anterior)
	        ILI9341_FillScreen(WHITE);

	        // 2. Pintamos lo estático según el NUEVO estado
	        if (juego->estadoActual == INICIO) {
	            // Pintamos el título UNA VEZ y ya se queda ahí fijo
	            ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
	            ILI9341_DrawText("PONG", FONT4, 110, 100, BLACK, WHITE);
	            ILI9341_DrawText("Pulsa boton", FONT3, 80, 150, BLACK, WHITE);
	        }
	        else if (juego->estadoActual == GANAROJO) {
	             ILI9341_DrawText("GANA ROJO", FONT4, 80, 110, RED, WHITE);
	        }
	        else if (juego->estadoActual == GANAAZUL) {
	             ILI9341_DrawText("GANA AZUL", FONT4, 80, 110, BLUE, WHITE);
	        }

	        // 3. Actualizamos la memoria para no volver a entrar aquí hasta que cambie algo
	        estadoAnterior = juego->estadoActual;
	    }


	    if (juego->estadoActual == JUEGO) {

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
	    // =========================================
	    // === DIBUJAR EL TEMPORIZADOR ===
	    // =========================================
	    if (juego->estadoActual == JUEGO) {

	    char texto[5];

	    if (juego->timer_activo == 1) {
	        sprintf(texto, "%02d", juego->tiempo_restante);

	        // Verde si sobra tiempo, Rojo si queda poco
	        uint16_t color = (juego->tiempo_restante > 10) ? 0x07E0 : 0xF800;

	        // CAMBIO 1: Función correcta según tu foto
	        // Orden: Texto, Fuente, X, Y, ColorTexto, ColorFondo
	        ILI9341_DrawText(texto, FONT4, 140, 10, color, 0xFFFF);
	    }
	    else {
	        // CAMBIO 2: Borrar con rectángulo
	        // Tu librería usa Coordenadas (Inicio -> Fin), no ancho/alto.
	        // X0=140, Y0=10
	        // X1=180 (140+40), Y1=35 (10+25)
	        ILI9341_DrawFilledRectangleCoord(140, 10, 180, 35, 0xFFFF);
	    }
	 }
}
