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

/** @defgroup MinixVice MinixVice
 * @{
 *
 * Game manager and methods
 */

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

/**
 * @brief Initializes the game variables
 *
 * @return a pointer to the game initialized
 */
MinixVice* initMinixVice();


/**
 * @brief Returns the existing game. If no game exists, creates a new one. Guarantees singleton design pattern.
 * Singleton DP limits the existence of a "object" (pointer) to 1.
 *
 * @return a pointer to the existent game
 */
MinixVice* getGame();


/* I/O HANDLING */


/**
 * @brief Subscribes all interrupts (keyboard, timer, mouse)
 */
void subscribeInterrupts();

/**
 * @brief Unsubscribes all interrupts (keyboard, timer, mouse)
 */
void unsubscribeInterrupts();

/**
 * @brief Mouse interrupt handler
 */
void mouseIH();

/**
 * @brief Timer interrupt handler
 */
void timerIH();

/**
 * @brief Keyboard interrupt handler
 */
void kbdIH();


/**
 * @brief Read date and hour from the RTC (Real-Time Clock)
 */
void readRTC();

/**
 * @brief All interrupts handler. Calls appropriate handler for each interrupt.
 */
void interruptsHandler();


/* EVENT HANDLING */

/**
 * @brief Handles game events and takes care of switching game states
 */
void handleEvents();


/* UPDATE-DRAW-END GAME*/

/**
 * @brief Updates game. Calls IH and events handler
 */
void updateMinixVice();


/**
 * @brief Draws game.
 */
void drawMinixVice();


/**
 * @brief Ends game. Frees all memory.
 */
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
