#include "entities.h"
#include "MinixVice.h"


extern st_player playerState;

/* PLAYER METHODS */

void movePlayerLeft() {
	MinixVice* game = getGame();

	game->car->x = game->car->x - 3*game->speed;
}

void movePlayerRight() {
	MinixVice* game = getGame();
	game->car->x = game->car->x + 3*game->speed;
}

void drawPlayer() {

	MinixVice* game = getGame();

	switch (playerState) {
	case TLEFT:
		drawBitmap(game->car->bmpTLeft, game->car->x, game->car->y, ALIGN_LEFT);
		break;
	case TRIGHT:
		drawBitmap(game->car->bmpTRight, game->car->x, game->car->y, ALIGN_LEFT);
		break;
	default:
		drawBitmap(game->car->bmpForward, game->car->x, game->car->y, ALIGN_LEFT);
		break;
	}

}


/* ENEMY METHODS */

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

