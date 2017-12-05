#include "entities.h"


void movePlayerLeft(Player* car){
	car->x = car->x - car->speed;
}

void movePlayerRight(Player* car){
	car->x = car->x + car->speed;
}

void drawPlayer(Player* car){

	int oldX = car->x - car->deltaX;
	int oldY = car->y - car->deltaY;

	eraseBitmap(car->bitmap, oldX, oldY, ALIGN_LEFT);
	drawBitmap(car->bitmap, car->x, car->y, ALIGN_LEFT);
}

