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

	ColliderBox* playBtn;
	ColliderBox* quitBtn;
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
 * @brief Draws the player bitmap on the screen
 */
void drawPlayer();


/* BARREL METHODS */

/**
 * @brief Draws the barrel on the screen
 *
 * @param barrel a barrel pointer with the bitmap to be drawn
 */
void drawBarrel(Barrel* barrel);

/**
 * @brief Draws the cone on the screen
 *
 * @param cone a cone pointer with the bitmap to be drawn
 */
void drawCone(Cone* cone);



#endif /* ENTITIES_H */
