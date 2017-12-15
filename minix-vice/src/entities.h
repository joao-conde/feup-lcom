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

}Player;


typedef struct{

	int x,y;
	int width, height;
	Bitmap* bitmap;

}Barrel;


typedef struct{

	int x,y;
	int width, height;
	int extraPts;

}ExtraPtsBoost;


void drawMovingBackground();

/* PLAYER METHODS */

void movePlayerLeft(Player* car);
void movePlayerRight(Player* car);
void drawPlayer(Player* car);


/* BARREL METHODS */

void drawBarrel(Barrel* barrel);


#endif /* ENTITIES_H */
