
#ifndef INC_BOLA_H_
#define INC_BOLA_H_

#define SCREEN_W 320
#define SCREEN_H 240


typedef struct {
    int x, y;
    int dx, dy;
    int radio;
} Ball;

void Ball_Inicial(Ball *b);
void Ball_Update(Ball *b);
void Ball_Reset(Ball *b);


#endif /* INC_BOLA_H_ */
