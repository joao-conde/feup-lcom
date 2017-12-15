#include "entities.h"
#include "MinixVice.h"


extern st_player playerState;

/* PLAYER METHODS */

void movePlayerLeft(Player* car) {
	MinixVice* game = getGame();

	car->x = car->x - 3*game->speed;
}

void movePlayerRight(Player* car) {
	MinixVice* game = getGame();
	car->x = car->x + 3*game->speed;
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


/* ENEMY METHODS */

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

