#include <stdint.h>
#ifndef INC_PALA_H_
#define INC_PALA_H_

#define SCREEN_W 320
#define SCREEN_H 240

typedef struct {
    int x, y;
    int ancho, alto;
} Paddle;

void Paddle_Inicial(Paddle *p, int x);
void Paddle_Update(Paddle *p, uint16_t adc);



#endif /* INC_PALA_H_ */
