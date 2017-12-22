#include "entities.h"
#include "MinixVice.h"


extern st_player playerState;

/* PLAYER METHODS */

void movePlayerLeft() {
	MinixVice* game = getGame();

	game->car->x -= TURN_SPEED * game->speed;
	game->car->body->x1 -= TURN_SPEED * game->speed;
	game->car->body->x2 -= TURN_SPEED * game->speed;

	if(game->car->x <= LEFT_ROAD_LIMIT)
		game->car->x = LEFT_ROAD_LIMIT;
}

void movePlayerRight() {
	MinixVice* game = getGame();
	int carWidth = game->car->bmpForward->bitmapInfoHeader.width;


	game->car->x += TURN_SPEED * game->speed;
	game->car->body->x1 += TURN_SPEED * game->speed;
	game->car->body->x2 += TURN_SPEED * game->speed;


	if(game->car->x >= (RIGHT_ROAD_LIMIT - carWidth))
		game->car->x = (RIGHT_ROAD_LIMIT - carWidth);
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


/* OBSTACLES METHODS */

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

void drawCone(Cone* cone) {
	drawBitmap(cone->bitmap, cone->x, cone->y, ALIGN_LEFT);
}

