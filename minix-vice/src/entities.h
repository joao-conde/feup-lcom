#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"

typedef struct{

	int x,y;
	int deltaX, deltaY; //TODO: is it needed, given that we dont need to erase old position anymore cuz db?
	int speed;

	Bitmap* bitmap;


}Player;


typedef struct{

	int x,y;

}Oil;


typedef struct{

	int x,y;
	int extraPts;

}ExtraPtsBoost;


/* PLAYER METHODS */

void movePlayerLeft(Player* car);
void movePlayerRight(Player* car);
void drawPlayer(Player* car);


#endif /* ENTITIES_H */
