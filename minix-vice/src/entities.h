#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"
#include "ColliderBox.h"
#include "state_machines.h"
#include "utils.h"


/** @defgroup entities entities
 * @{
 *
 * Definition of game entities and functions to operate on them
 */

typedef struct{

	ColliderBox* button;
	Bitmap* normal;
	Bitmap* hover;

}Button;

typedef struct{

	Button* playBtn;
	Button* quitBtn;
	Bitmap* menu_background;

}MainMenu;

typedef struct{

	ColliderBox* select_lamb;
	ColliderBox* select_red;
	ColliderBox* select_mercedes;
	Bitmap* select_background;

}SelectMenu;

typedef struct{

	int x,y;

	Bitmap* bmpForward;
	Bitmap* bmpTLeft;
	Bitmap* bmpTRight;

	ColliderBox* body;

}Player;


typedef struct{

	int x,y;
	Bitmap* bitmap;

	ColliderBox* body;

}Barrel;


typedef struct{

	int x,y;
	Bitmap* bitmap;

	ColliderBox* body;

}Cone;



/* PLAYER METHODS */

/**
 * @brief Moves the player left (decreases X coordinate)
 */
void movePlayerLeft();

/**
 * @brief Moves the player right (increases X coordinate)
 */
void movePlayerRight();


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



#endif /* ENTITIES_H */
