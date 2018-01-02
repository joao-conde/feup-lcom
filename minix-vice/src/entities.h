#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"
#include "ColliderBox.h"
#include "state_machines.h"
#include "utils.h"


/** @defgroup entities entities
 * @{
 *
 * Definition of game entities and basic functions to operate with them
 */


 /// Represents a bonus score gain animation
typedef struct{
	int x, y;
	int frame;
	int useAnimation;

	Bitmap* bonusBmp;
}Bonus;


/// Represents a cone shot with small explosion animation

typedef struct{
	int x, y; //shot location
	int bmpIndex; //current bmp to be drawn off of sprites array
	int useAnimation; //display or not

	Bitmap* sprites[SPRITESHEET_SHOT_SIZE]; //animation bmps

}Shot;


/// Represents a button

typedef struct{

	ColliderBox* button;
	Bitmap* normal;
	Bitmap* hover;

}Button;


/// Represents the main menu

typedef struct{

	Button* playBtn;
	Button* quitBtn;
	Bitmap* menu_background;

}MainMenu;


/// Represents the select car menu

typedef struct{

	Button* select_lamb;
	Button* select_red;
	Button* select_mercedes;
	Bitmap* select_background;

}SelectMenu;


/// Represents the player

typedef struct{

	int x,y;

	Bitmap* bmpForward;
	Bitmap* bmpTLeft;
	Bitmap* bmpTRight;

	ColliderBox* body;

}Player;


/// Represents a barrel obstacle

typedef struct{

	int x,y;
	Bitmap* bitmap;

	ColliderBox* body;

}Barrel;

/// Represents a cone obstacle

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
 * @brief Creates animations
 */
void createAnimations();


/**
 * @brief Creates bonus animations
 */
void createBonusAnimations();


/**
 * @brief Creates shot animations
 */
void createShotAnimations();


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
 * @brief Frees the barrel entities
 */
void freeBarrels();


/**
 * @brief Frees the cone entities
 */
void freeCones();



#endif /* ENTITIES_H */
