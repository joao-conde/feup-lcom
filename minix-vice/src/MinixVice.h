#ifndef MINIXVICE_H
#define MINIXVICE_H

#include <minix/drivers.h>

#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "bitmap.h"
#include "entities.h"
#include "utils.h"
#include "state_machines.h"
#include "rtc.h"
#include "IO.h"
#include "logic.h"
#include "graphics.h"
#include "bmpHandling.h"


/** @defgroup MinixVice MinixVice
 * @{
 *
 * Game manager and methods
 */

 /// Represents the main game entity managing all game properties

typedef struct {

	int irq_kbd, irq_timer, irq_mouse;
	Timer* timer;

	unsigned long scancode;
	unsigned long *hours, *minutes, *seconds;
	unsigned long *day, *month, *year;

	int done;
	int bonus;

	int conesShot;
	float speed;
	float score;

	MainMenu* main_menu;
	SelectMenu* select_menu;
	Bitmap* help_screen;
	Bitmap* stats_screen;

	Bitmap* score_tag;
	Bitmap* background;
	Bitmap* digits[DECIMAL_ALG];
	Bitmap* utils[UTILS_LENGTH];

	Player* car;
	Barrel* barrels[NUMBER_OF_BARRELS];
	Cone* cones[NUMBER_OF_CONES];


	Shot* shotAnimations[MAX_CONESHOT_ANIM]; //cone shooting explosion animation
	Bonus* bonusAnimations[MAX_CONESHOT_ANIM]; //'+10' visual bonus

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
 * Singleton design pattern limits the existence of an "object" (MinixVice pointer) to 1.
 *
 * @return a pointer to the existent game
 */
MinixVice* getGame();


/**
 * @brief Ends game. Frees all memory.
 */
void endMinixVice();


/**
 * @brief Restarts the game.
 */
void startNewGame();


/**
 * @brief Initializes some game properties
 */
void initGameProperties();


#endif /* MINIXVICE_H */
