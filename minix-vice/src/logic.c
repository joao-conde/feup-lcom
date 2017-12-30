#include "logic.h"



void brake() {
	MinixVice* game = getGame();

	if (game->speed < MIN_SPEED) {
		game->speed = MIN_SPEED;
		return;
	}

	game->speed -= BRAKE_SPEED;
}

void accelerate() {
	MinixVice* game = getGame();

	game->speed += ACCELERATE_SPEED;
}

void recalculateBarrelPos(Barrel* barrel) {
	int width, height, newX, newY;

	width = barrel->bitmap->bitmapInfoHeader.width;
	height = barrel->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);

	/* negative values ensure they come a while after disappearing
	 * and is random so they aren't always in the same height cord
	 */
	newY = -generateRandomPos(50, 600);

	barrel->x = newX;
	barrel->y = newY;

	barrel->body->x1 = newX;
	barrel->body->y1 = newY;

	barrel->body->x2 = newX + width;
	barrel->body->y2 = newY + height;

}

void recalculateConePos(Cone* cone) {
	int width, height, newX, newY;

	width = cone->bitmap->bitmapInfoHeader.width;
	height = cone->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);

	/*
	 * negative values for the same reason as the barrels
	 */
	newY = -generateRandomPos(50, 600);

	cone->x = newX;
	cone->y = newY;

	cone->body->x1 = newX;
	cone->body->y1 = newY;

	cone->body->x2 = newX + width;
	cone->body->y2 = newY + height;

}

void recalculateBarrelsPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		recalculateBarrelPos(game->barrels[i]);
	}
}

void recalculateConesPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		recalculateConePos(game->cones[i]);
	}
}

void calculateScore() {
	MinixVice* game = getGame();

	game->score += SCORE_INCREASE(game->speed);
}


void updateBarrelsPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		game->barrels[i]->y += game->speed;
		game->barrels[i]->body->y1 += game->speed;
		game->barrels[i]->body->y2 += game->speed;
	}
}

void updateConesPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		game->cones[i]->y += game->speed;
		game->cones[i]->body->y1 += game->speed;
		game->cones[i]->body->y2 += game->speed;
	}
}

