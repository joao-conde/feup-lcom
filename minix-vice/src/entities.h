#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"
#include "ColliderBox.h"
#include "state_machines.h"

typedef struct{

	ColliderBox* playBtn;
	ColliderBox* quitBtn;
	Bitmap* menu_background;

}MainMenu;

typedef struct{

	ColliderBox* select_lamb;
	ColliderBox* select_blue;
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


void drawMovingBackground();

/* PLAYER METHODS */

void movePlayerLeft();
void movePlayerRight();
void drawPlayer();


/* BARREL METHODS */

void drawBarrel(Barrel* barrel);
void drawBarrels();
void updateBarrelsPos();



#endif /* ENTITIES_H */
