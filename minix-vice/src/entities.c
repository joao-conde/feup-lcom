#include "entities.h"


void movePlayerLeft(Player* car){
	int deltaX, oldX = car->x;
	car->x = car->x - car->speed;
	car->deltaX = car->x - oldX;
}

void movePlayerRight(Player* car){
	int deltaX, oldX = car->x;
	car->x = car->x + car->speed;
	car->deltaX = car->x - oldX;
}

void drawPlayer(Player* car){

	int oldX = car->x - car->deltaX;
	int oldY = car->y - car->deltaY;

	eraseBitmap(car->bitmap, oldX, oldY, ALIGN_LEFT);
	drawBitmap(car->bitmap, car->x, car->y, ALIGN_LEFT);
}

