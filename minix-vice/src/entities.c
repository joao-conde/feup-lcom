#include "entities.h"

void movePlayerLeft(Player* car) {
	car->x = car->x - car->speed;
}

void movePlayerRight(Player* car) {
	car->x = car->x + car->speed;
}

void drawPlayer(Player* car) {
	drawBitmap(car->bitmap, car->x, car->y, ALIGN_LEFT);
}

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

