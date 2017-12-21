#ifndef MINIXVICE_H
#define MINIXVICE_H

#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"
#include "utils.h"
#include "state_machines.h"



typedef struct {

	int irq_kbd, irq_timer, irq_mouse;

	int done, draw;

	float speed;
	int score;

	MainMenu* main_menu;
	SelectMenu* select_menu;

	unsigned long scancode;

	Bitmap* background;

	Bitmap* digits[10];

	Player* car;
	Timer* timer;

	Barrel* barrels[2];
	Cone* cones[2];

} MinixVice;

MinixVice* initMinixVice();

MinixVice* getGame();

void updateMinixVice();

void drawMinixVice();

void brake();

void accelerate();

void createBarrels();

void deleteBitmaps();


void endMinixVice();

#endif /* MINIXVICE_H */
