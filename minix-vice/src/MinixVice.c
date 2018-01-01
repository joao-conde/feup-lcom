#include "MinixVice.h"

//extern st_game gameState;

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

	game->scancode = 0;
	game->speed = INITIAL_SPEED;
	game->bonus = 0;
	game->score = 0;
	game->conesShot = 0;

	updateGameState(PLAY);
	updateMouseState(MENU);
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

