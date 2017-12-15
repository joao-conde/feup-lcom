#ifndef MINIXVICE_H
#define MINIXVICE_H


#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"
#include "utils.h"
#include "state_machines.h"



typedef struct{

	int irq_kbd, irq_timer, irq_mouse;

	int done, draw;


	MainMenu* main_menu;

	unsigned long scancode;

	int backgroundY, background1Y;
	Bitmap* background;
	Bitmap* background1;
	Bitmap* menu_background;

	Player* car;
	Timer* timer;

	Barrel* barrel;

} MinixVice;


MinixVice* initMinixVice();

MinixVice* getGame();

void updateMinixVice();

void drawMinixVice();

void endMinixVice();

void drawMovingBackground(); // TODO DELETE

#endif /* MINIXVICE_H */
