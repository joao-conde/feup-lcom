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

	switch (gameState) {

	case MAIN_MENU:
		handleMainMenuEvents();
		break;

	case HELP_MENU:
		handleHelpMenuEvents();
		break;

	case SELECT_MENU:
		handleSelectMenuEvents();
		break;

	case GAME:
		handleGameScreenEvents();
		break;

	case STATS_MENU:
		handleStatsMenuEvents();
		break;

	case OVER:
		game->done = 1;
		break;

	}
}

void handleMainMenuEvents() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();

	if (clicked(game->main_menu->playBtn->button, m)) {
		updateGameState(SELECT_CAR);
	}

	if (clicked(game->main_menu->quitBtn->button, m)) {
		updateGameState(TERMINATE);
	}

	if (game->scancode == H_BREAK)
		updateGameState(HELP);
}

void handleHelpMenuEvents() {
	MinixVice* game = getGame();

	if (game->scancode == ESC_BREAK)
		updateGameState(MAIN);
}

void handleSelectMenuEvents() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();

	if (clicked(game->select_menu->select_red->button, m)) {
		loadCarBitmaps(SELECTED_RED);
		initPlayer();

		updateMouseState(TARGET);
		updateGameState(PLAY);
	}

	if (clicked(game->select_menu->select_lamb->button, m)) {
		loadCarBitmaps(SELECTED_LAMB);
		initPlayer();

		updateMouseState(TARGET);
		updateGameState(PLAY);
	}

	if (clicked(game->select_menu->select_mercedes->button, m)) {
		loadCarBitmaps(SELECTED_MERCEDES);
		initPlayer();

		updateMouseState(TARGET);
		updateGameState(PLAY);
	}
}

void handleGameScreenEvents() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();

	if (game->timer->ticked) {
		calculateScore();
		updateBarrelsPos();
		updateConesPos();
		updateAnimations();
	}

	if (game->bonus) {
		game->bonus = 0;
		game->score += CONESHOT_BONUS;
	}

	checkCollisions();
}

void handleStatsMenuEvents() {
	MinixVice* game = getGame();

	forceDisplay(GAMEOVERSCREEN_WAIT);
}

void checkCollisions() {
	checkBarrelsCollisions();
	checkConesCollisions();
}

void checkBarrelsCollisions() {
	MinixVice* game = getGame();
	int i;

	for (i = 0; i < NUMBER_OF_BARRELS; i++) {

		if (collide(game->car->body, game->barrels[i]->body)) {
			updateGameState(TERMINATE);
			updateMouseState(INVISIBLE);
		}

		//if barrel out of game screen re-calculate coordinates
		if (game->barrels[i]->y > vg_getVRES()) {
			recalculateBarrelPos(game->barrels[i]);
		}

	}
}

void checkConesCollisions() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();
	int i;

	for (i = 0; i < NUMBER_OF_CONES; i++) {

		if (collide(game->car->body, game->cones[i]->body)) {
			updateGameState(TERMINATE);
			updateMouseState(INVISIBLE);
		}

		//shot cone
		if (clicked(game->cones[i]->body, m)) {
			game->bonus = 1;
			game->conesShot++;

			//Cone shot + bonus animation
			startConeShotAnimations(game->cones[i]->x, game->cones[i]->y);

			recalculateConePos(game->cones[i]);
		}

		//if cone out of game screen re-calculate coordinates
		if (game->cones[i]->y > vg_getVRES()) {
			recalculateConePos(game->cones[i]);
		}
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
	newY = -generateRandomPos(RANDOM_LOWERB, RANDOM_UPPERB);

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
	newY = -generateRandomPos(RANDOM_LOWERB, RANDOM_UPPERB);

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

void startConeShotAnimations(int x, int y) {
	startBonusAnimation(x, y);
	startShotAnimation(x, y);
}

void startBonusAnimation(int x, int y) {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {

		if (game->bonusAnimations[i]->useAnimation == 0) {
			game->bonusAnimations[i]->x = x + BONUSANIM_OFFSETX;
			game->bonusAnimations[i]->y = y + BONUSANIM_OFFSETY;
			game->bonusAnimations[i]->frame = 0;
			game->bonusAnimations[i]->useAnimation = 1;
			return;
		}
	}
}

void startShotAnimation(int x, int y) {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {

		if (game->shotAnimations[i]->useAnimation == 0) {
			game->shotAnimations[i]->x = x;
			game->shotAnimations[i]->y = y;
			game->shotAnimations[i]->bmpIndex = 0;
			game->shotAnimations[i]->useAnimation = 1;
			return;
		}
	}
}

void updateAnimations() {
	updateShotAnimations();
	updateBonusAnimations();
}

void updateShotAnimations() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {

		if (game->shotAnimations[i]->useAnimation != 0) {
			if (game->shotAnimations[i]->bmpIndex == SPRITESHEET_SHOT_SIZE - 1)
				game->shotAnimations[i]->useAnimation = 0;
			else
				game->shotAnimations[i]->bmpIndex++;
		}
	}
}

void updateBonusAnimations() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {

		if (game->bonusAnimations[i]->useAnimation != 0) {
			if (game->bonusAnimations[i]->frame == BONUS_FRAMES_DISPLAY)
				game->bonusAnimations[i]->useAnimation = 0;
			else {
				game->bonusAnimations[i]->frame++;
				game->bonusAnimations[i]->y++;
			}
		}
	}
}


void forceDisplay(int seconds){
	MinixVice* game = getGame();

	//blocks user for an amount of seconds specified so he can see the gameover screen.
	static int ticks = 0;

	if(game->timer->ticked){
		ticks++;
		game->scancode = ESC_BREAK;
	}

	if (ticks > SECONDS_TO_TICKS(seconds)) {

		if (game->scancode != ESC_BREAK) {
			ticks = 0;
			startNewGame();
		}
	}
}

