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
	float score;

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


/* SINGLETON IMPLEMENTATION */

MinixVice* initMinixVice();

MinixVice* getGame();


/* INTERRUPT HANDLING */

void subscribeInterrupts();

void unsubscribeInterrupts();

void mouseIH();

void timerIH();

void kbdIH();

void interruptsHandler();


/* EVENT HANDLING */

void handleEvents();


/* UPDATE-DRAW-END GAME*/

void updateMinixVice();

void drawMinixVice();

void endMinixVice();



/* GAME METHODS */

void brake();

void accelerate();

void displayScore();

void calculateScore();

void initGameProperties();




/* ENTITIES METHODS */

void createEntities();

void createBarrels();

void createCones();

void initPlayer();

void initMainMenu();

void initSelectMenu();

void initBarrels();

void initCones();

void initTimer();

void recalculateBarrelPos(Barrel* barrel);

void recalculateConePos(Cone* cone);

void drawBarrels();

void drawCones();

void freeBarrels();

void freeCones();


/* BACKGROUND */

void drawMovingBackground();

void updateBarrelsPos();

void updateConesPos();



/* BITMAP LOADING */

void loadBitmaps();

void loadBarrelsBitmaps();

void loadConesBitmaps();

void loadDigitBitmaps();

void loadCarBitmaps(int selectedCar);


/* BITMAP DELETING */

void deleteBitmaps();

void deleteDigitBitmaps();

void deleteBarrelsBitmaps();

void deleteConesBitmaps();



#endif /* MINIXVICE_H */
