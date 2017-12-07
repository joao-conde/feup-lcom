#include "entities.h"
#include "state_machines.h"

extern st_player playerState;

void movePlayerLeft(Player* car) {
	car->x = car->x - car->speed;
}

void movePlayerRight(Player* car) {
	car->x = car->x + car->speed;
}

void drawPlayer(Player* car) {

	switch (playerState) {
	case TLEFT:
		drawBitmap(car->bmpTLeft, car->x, car->y, ALIGN_LEFT);
		break;
	case TRIGHT:
		drawBitmap(car->bmpTRight, car->x, car->y, ALIGN_LEFT);
		break;
	default:
		drawBitmap(car->bmpForward, car->x, car->y, ALIGN_LEFT);
		break;
	}

}

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

