#ifndef MINIXVICE_H
#define MINIXVICE_H


#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"


typedef struct{

	int irq_kbd, irq_timer, irq_mouse;

	int done, draw;



	unsigned long scancode;

	Bitmap* test;
	Player* car;
	Timer* timer;

} MinixVice;


MinixVice* initMinixVice();

void updateMinixVice(MinixVice* game);

void drawMinixVice(MinixVice* game);

void endMinixVice(MinixVice* game);

#endif /* MINIXVICE_H */
