#include "logic.h"

extern st_game gameState;

void updateMinixVice() {

	MinixVice* game = getGame();

	game->timer->ticked = 0;

	readRTC();

	interruptsHandler();

	handleEvents();

}

void handleEvents() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();
	int i;

	switch (gameState) {

	case MAIN_MENU:

		if (clicked(game->main_menu->playBtn->button, m)) {
			updateGameState(SELECT_CAR);
		}

		if (clicked(game->main_menu->quitBtn->button, m)) {
			updateGameState(TERMINATE);
		}

		if (game->scancode == H_BREAK)
			updateGameState(HELP);

		break;

	case HELP_MENU:

		if (game->scancode == ESC_BREAK)
			updateGameState(MAIN);

		break;

	case SELECT_MENU:

		if (clicked(game->select_menu->select_red->button, m)) {
			loadCarBitmaps(1);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_lamb->button, m)) {
			loadCarBitmaps(2);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_mercedes->button, m)) {
			loadCarBitmaps(3);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		break;

	case GAME:

		if (game->timer->ticked) {
			calculateScore();
			updateBarrelsPos();
			updateConesPos();

			updateShotAnimations();


		}

		if (game->bonus) {
			game->bonus = 0;
			game->score += CONESHOT_BONUS;
		}

		for (i = 0; i < NUMBER_OF_BARRELS; i++) {

			if (collide(game->car->body, game->barrels[i]->body)) {
				updateGameState(TERMINATE);
				game->scancode = ESC_BREAK;
			}

			//if barrel out of game screen re-calculate coordinates
			if (game->barrels[i]->y > vg_getVRES()) {
				recalculateBarrelPos(game->barrels[i]);
			}

		}

		for (i = 0; i < NUMBER_OF_CONES; i++) {

			if (collide(game->car->body, game->cones[i]->body)) {
				updateGameState(TERMINATE);
				game->scancode = ESC_BREAK;
			}

			//shot cone
			if (clicked(game->cones[i]->body, m)) {
				game->bonus = 1;
				game->conesShot++;

				startShotAnimation(game->cones[i]->x, game->cones[i]->y);
				recalculateConePos(game->cones[i]);


			}

			//if cone out of game screen re-calculate coordinates
			if (game->cones[i]->y > vg_getVRES()) {
				recalculateConePos(game->cones[i]);
			}
		}

		break;

	case STATS_MENU:

		if (game->scancode != ESC_BREAK) {
			startNewGame();
		}

		break;

	case OVER:
		game->done = 1;
		break;

	}
}

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


void startShotAnimation(int x, int y){
	MinixVice* game = getGame();

	int i, index;
	for (i = 0; i < 3; i++) {

		if (game->shotAnimations[i]->useAnimation == 0) {
			game->shotAnimations[i]->x = x;
			game->shotAnimations[i]->y = y;
			game->shotAnimations[i]->bmpIndex = 0;
			game->shotAnimations[i]->useAnimation = 1;
			return;
		}
	}
}

void updateShotAnimations() {
	MinixVice* game = getGame();

	int i, index;
	for (i = 0; i < 3; i++) {

		if (game->shotAnimations[i]->useAnimation != 0) {
			if (game->shotAnimations[i]->bmpIndex == 15)
				game->shotAnimations[i]->useAnimation = 0;
			else
				game->shotAnimations[i]->bmpIndex++;
		}
	}
}

