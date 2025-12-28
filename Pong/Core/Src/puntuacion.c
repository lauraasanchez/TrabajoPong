#include "puntuacion.h"

// === CONFIGURACIÓN DE PINES (Cátodo Común) ===
#define SCORE_PORT   GPIOE
#define PIN_A      GPIO_PIN_7
#define PIN_B      GPIO_PIN_8
#define PIN_C      GPIO_PIN_9
#define PIN_D      GPIO_PIN_10
#define PIN_E      GPIO_PIN_11
#define PIN_F      GPIO_PIN_12
#define PIN_G      GPIO_PIN_13

#define PIN_D1     GPIO_PIN_0  // Digito Izq
#define PIN_D4     GPIO_PIN_6  // Digito Der

const uint8_t segmentMap[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static uint8_t val_p1 = 0;
static uint8_t val_p2 = 0;

void Score_Init(void) {
    val_p1 = 0;
    val_p2 = 0;
    // Apagamos todo (SET apaga dígitos en Cátodo Común)
    HAL_GPIO_WritePin(SCORE_PORT, PIN_D1 | PIN_D4, GPIO_PIN_SET);
}

void Score_SetValues(uint8_t p1, uint8_t p2) {
    val_p1 = p1;
    val_p2 = p2;
}

static void WriteSegment(uint8_t num) {
    if(num > 9) num = 0;
    uint8_t code = segmentMap[num];
    // Lógica Cátodo Común (1 = SET)
    HAL_GPIO_WritePin(SCORE_PORT, PIN_A, (code & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_B, (code & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_C, (code & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_D, (code & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_E, (code & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_F, (code & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SCORE_PORT, PIN_G, (code & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// ESTA FUNCIÓN AHORA SE LLAMARÁ DESDE LA INTERRUPCIÓN (TIM3)
// Ya no usa Delay, simplemente cambia de turno.
void Score_Refresh(void) {
    static uint8_t turno = 0; // 0 = Digito 1, 1 = Digito 4

    // 1. Apagamos ambos dígitos primero para evitar fantasmas
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0 | GPIO_PIN_6, GPIO_PIN_SET);

    if (turno == 0) {
        // Turno del Jugador 1 (Azul)
        WriteSegment(val_p1);
        HAL_GPIO_WritePin(SCORE_PORT, PIN_D1, GPIO_PIN_RESET); // Encender D1
        turno = 1; // El próximo turno le toca al otro
    }
    else {
        // Turno del Jugador 2 (Rojo)
        WriteSegment(val_p2);
        HAL_GPIO_WritePin(SCORE_PORT, PIN_D4, GPIO_PIN_RESET); // Encender D4
        turno = 0; // El próximo turno le toca al primero
    }
}
