#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"
#include "ColliderBox.h"
#include "state_machines.h"

typedef struct{

	ColliderBox* playBtn;
	ColliderBox* quitBtn;

}MainMenu;

typedef struct{

	int x,y;
	int width, height;

	Bitmap* bmpForward;
	Bitmap* bmpTLeft;
	Bitmap* bmpTRight;

	ColliderBox* body;

}Player;


typedef struct{

	int x,y;
	int width, height;
	Bitmap* bitmap;

	ColliderBox* body;

}Barrel;


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
