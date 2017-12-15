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

	float speed;

	MainMenu* main_menu;

	unsigned long scancode;

	Bitmap* background;
	Bitmap* menu_background;

	Player* car;
	Timer* timer;

	Barrel* barrel;

} MinixVice;


MinixVice* initMinixVice();

MinixVice* getGame();

void updateMinixVice();

void drawMinixVice();

void brake();

void accelerate();

void endMinixVice();


#endif /* MINIXVICE_H */
