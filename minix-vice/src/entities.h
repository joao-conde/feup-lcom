#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"


typedef struct{

	int x,y;
	int width, height;
	int speed;

	Bitmap* bitmap;


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


/* PLAYER METHODS */

void movePlayerLeft(Player* car);
void movePlayerRight(Player* car);
void drawPlayer(Player* car);


/* BARREL METHODS */

void drawBarrel(Barrel* barrel);


#endif /* ENTITIES_H */
