#ifndef ENTITIES_H
#define ENTITIES_H


#include "bitmap.h"

typedef struct{

	int x,y;
	int deltaX, deltaY;
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



void movePlayerLeft(Player* car);
void movePlayerRight(Player* car);


#endif /* ENTITIES_H */
