#ifndef MINIXVICE_H
#define MINIXVICE_H

#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"
#include "utils.h"
#include "state_machines.h"
#include "rtc.h"


typedef struct {

	int irq_kbd, irq_timer, irq_mouse;
	Timer* timer;

	unsigned long scancode;
	unsigned long *hours, *minutes, *seconds;
	unsigned long *day, *month, *year;

	int done;
	int bonus;

	int conesShot; //TODO use coneshot statistic
	float speed;
	float score;

	MainMenu* main_menu;
	SelectMenu* select_menu;
	Bitmap* help_screen;
	Bitmap* stats_screen;

	Bitmap* background;
	Bitmap* digits[10];

	Player* car;
	Barrel* barrels[4];
	Cone* cones[3];

} MinixVice;


/* SINGLETON IMPLEMENTATION */

MinixVice* initMinixVice();

MinixVice* getGame();


/* I/O HANDLING */

void subscribeInterrupts();

void unsubscribeInterrupts();

void mouseIH();

void timerIH();

void kbdIH();

void readRTC();

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

void calculateScore();

void displayScore();

void displayDate();

void displayHour();

void displayConesShot();

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
