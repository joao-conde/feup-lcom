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

	int conesShot;
	float speed;
	float score;

	MainMenu* main_menu;
	SelectMenu* select_menu;
	Bitmap* help_screen;
	Bitmap* stats_screen;

	Bitmap* background;
	Bitmap* digits[10];
	Bitmap* utils[2];

	Player* car;
	Barrel* barrels[NUMBER_OF_BARRELS];
	Cone* cones[NUMBER_OF_CONES];

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



/**
 * @brief Displays the game score.
 */
void displayScore(int offsetX, int offsetY);


/**
 * @brief Displays date
 */
void displayDate();


/**
 * @brief Displays hour
 */
void displayHour();


/**
 * @brief Displays number of cones shot
 */
void displayConesShot();


/**
 * @brief Initializes some game properties
 */
void initGameProperties();




/* ENTITIES METHODS */


/**
 * @brief Creates game entities
 */
void createEntities();


/**
 * @brief Creates barrel entities
 */
void createBarrels();


/**
 * @brief Creates cone entities
 */
void createCones();


/**
 * @brief Initializes player variables
 */
void initPlayer();


/**
 * @brief Initializes main menu variables
 */
void initMainMenu();


/**
 * @brief Initializes select menu variables
 */
void initSelectMenu();


/**
 * @brief Initializes barrel entities variables
 */
void initBarrels();


/**
 * @brief Initializes cone entities variables
 */
void initCones();


/**
 * @brief Initializes struct Timer variables
 */
void initTimer();

/**
 * @brief Restarts the game, initializing new game variable
 */
void startNewGame();


/**
 * @brief Draws the barrel entities
 */
void drawBarrels();


/**
 * @brief Draws the cone entities
 */
void drawCones();


/**
 * @brief Frees the barrel entities
 */
void freeBarrels();


/**
 * @brief Frees the cone entities
 */
void freeCones();


/* BACKGROUND */


/**
 * @brief Draws a moving background giving the sense of movement
 */
void drawMovingBackground();


/* BITMAP LOADING */

/**
 * @brief Loads bitmaps
 */
void loadBitmaps();


/**
 * @brief Loads barrel entities bitmaps
 */
void loadBarrelsBitmaps();


/**
 * @brief Loads cone entities bitmaps
 */
void loadConesBitmaps();


/**
 * @brief Loads digits bitmaps
 */
void loadDigitBitmaps();


/**
 * @brief Loads car bitmaps according to the selected car
 *
 * @param selectedCar a number matching the selected car
 */
void loadCarBitmaps(int selectedCar);


/* BITMAP DELETING */


/**
 * @brief Deletes bitmaps
 */
void deleteBitmaps();


/**
 * @brief Deletes digit bitmaps
 */
void deleteDigitBitmaps();


/**
 * @brief Deletes barrel entities bitmaps
 */
void deleteBarrelsBitmaps();


/**
 * @brief Deletes cone entities bitmaps
 */
void deleteConesBitmaps();



#endif /* MINIXVICE_H */
