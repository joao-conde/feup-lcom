#include "MinixVice.h"

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();

	createEntities();

	loadBitmaps();

	initGameProperties();

	initMainMenu();

	initSelectMenu();

	initBarrels();

	initCones();

	initTimer();

	return game;
}

MinixVice* getGame() {

	if (game == NULL) {
		game = initMinixVice();
	}

	return game;
}

void updateMinixVice() {

	MinixVice* game = getGame();

	game->timer->ticked = 0;

	readRTC();

	interruptsHandler();

	handleEvents();

}

void drawMinixVice() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();

	switch (gameState) {

	case MAIN_MENU:
		drawBackgroundBitmap(game->main_menu->menu_background, 0, 0,
				ALIGN_LEFT);

		if (hovered(game->main_menu->playBtn->button, m))
			drawBitmap(game->main_menu->playBtn->hover,
					game->main_menu->playBtn->button->x1,
					game->main_menu->playBtn->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->main_menu->playBtn->normal,
					game->main_menu->playBtn->button->x1,
					game->main_menu->playBtn->button->y1, ALIGN_LEFT);

		if (hovered(game->main_menu->quitBtn->button, m))
			drawBitmap(game->main_menu->quitBtn->hover,
					game->main_menu->quitBtn->button->x1,
					game->main_menu->quitBtn->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->main_menu->quitBtn->normal,
					game->main_menu->quitBtn->button->x1,
					game->main_menu->quitBtn->button->y1, ALIGN_LEFT);

		break;

	case SELECT_MENU:
		drawBackgroundBitmap(game->select_menu->select_background, 0, 0,
				ALIGN_LEFT);

		if (hovered(game->select_menu->select_lamb->button, m))
			drawBitmap(game->select_menu->select_lamb->hover,
					game->select_menu->select_lamb->button->x1,
					game->select_menu->select_lamb->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_lamb->normal,
					game->select_menu->select_lamb->button->x1,
					game->select_menu->select_lamb->button->y1, ALIGN_LEFT);


		if (hovered(game->select_menu->select_red->button, m))
			drawBitmap(game->select_menu->select_red->hover,
					game->select_menu->select_red->button->x1,
					game->select_menu->select_red->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_red->normal,
					game->select_menu->select_red->button->x1,
					game->select_menu->select_red->button->y1, ALIGN_LEFT);



		if (hovered(game->select_menu->select_mercedes->button, m))
			drawBitmap(game->select_menu->select_mercedes->hover,
					game->select_menu->select_mercedes->button->x1,
					game->select_menu->select_mercedes->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_mercedes->normal,
					game->select_menu->select_mercedes->button->x1,
					game->select_menu->select_mercedes->button->y1, ALIGN_LEFT);

		break;

	case HELP_MENU:
		drawBackgroundBitmap(game->help_screen, 0, 0, ALIGN_LEFT);
		break;

	case GAME:

		if (game->timer->ticked) {
			drawMovingBackground();
			drawBitmap(game->score_tag, SCOREX_DISPLAY, SCOREY_DISPLAY, ALIGN_LEFT);
			drawPlayer(game->car);
			drawBarrels();
			drawCones();
			displayScore(SCOREX_OFFSET, SCOREY_OFFSET);
		}

		break;

	case STATS_MENU:
		drawBackgroundBitmap(game->stats_screen, 0, 0, ALIGN_LEFT);
		displayConesShot();
		displayDate();
		displayHour();
		displayScore(FINALSCOREX_OFFSET, FINALSCOREY_OFFSET);
		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		flipDB();
		game->timer->ticked = 0;
	}

}

void endMinixVice() {
	MinixVice* game = getGame();

	cleanOBF();
	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);

	freeBarrels();
	freeCones();

	free(game);

}


void startNewGame() {

	recalculateBarrelsPos();
	recalculateConesPos();

	game->speed = INITIAL_SPEED;
	game->bonus = 0;
	game->score = 0;
	game->conesShot = 0;

	updateGameState(PLAY);
	updateMouseState(MENU);
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
		}

		if (game->bonus) {
			game->bonus = 0;
			game->score += CONESHOT_BONUS;
		}

		for (i = 0; i < NUMBER_OF_BARRELS; i++) {

			if (collide(game->car->body, game->barrels[i]->body)) {
				updateGameState(TERMINATE);
			}

			//if barrel out of game screen re-calculate coordinates
			if (game->barrels[i]->y > vg_getVRES()) {
				recalculateBarrelPos(game->barrels[i]);
			}

		}

		for (i = 0; i < NUMBER_OF_CONES; i++) {

			if (collide(game->car->body, game->cones[i]->body)) {
				updateGameState(TERMINATE);
			}

			//shot cone
			if (clicked(game->cones[i]->body, m)) {
				recalculateConePos(game->cones[i]);
				game->bonus = 1;
				game->conesShot++;
			}

			//if cone out of game screen re-calculate coordinates
			if (game->cones[i]->y > vg_getVRES()) {
				recalculateConePos(game->cones[i]);
			}
		}

		break;

	case STATS_MENU:

		if (game->scancode == ESC_BREAK) {
			startNewGame();
		}

		break;

	case OVER:
		game->done = 1;
		break;

	}
}

void initGameProperties() {
	MinixVice* game = getGame();

	game->done = 0;
	game->scancode = 0;
	game->score = 0;

	game->bonus = 0;
	game->conesShot = 0;

	game->hours = (unsigned long*) malloc(sizeof(unsigned long));
	game->minutes = (unsigned long*) malloc(sizeof(unsigned long));
	game->seconds = (unsigned long*) malloc(sizeof(unsigned long));

	game->day = (unsigned long*) malloc(sizeof(unsigned long));
	game->month = (unsigned long*) malloc(sizeof(unsigned long));
	game->year = (unsigned long*) malloc(sizeof(unsigned long));

	game->speed = INITIAL_SPEED;
}

