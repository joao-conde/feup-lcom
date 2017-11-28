#ifndef MINIXVICE_H
#define MINIXVICE_H


#include "kbd.h"
#include "timer.h"

typedef struct{

	int irq_set_kbd, irq_set_timer;

	int done, draw;

	unsigned long scancode;
	Timer* timer;

} MinixVice;


MinixVice* initMinixVice();

void updateMinixVice(MinixVice* game);

void drawMinixVice(MinixVice* game);

void endMinixVice(MinixVice* game);

#endif /* MINIXVICE_H */
