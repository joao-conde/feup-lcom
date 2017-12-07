#ifndef MINIXVICE_H
#define MINIXVICE_H


#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"
#include "utils.h"


typedef struct{

	int irq_kbd, irq_timer, irq_mouse;

	int done, draw;



	unsigned long scancode;

	Bitmap* background;
	Player* car;
	Timer* timer;

	Barrel* barrel;

} MinixVice;


MinixVice* initMinixVice();

MinixVice* getGame();

void updateMinixVice();

void drawMinixVice();

void endMinixVice();

#endif /* MINIXVICE_H */
