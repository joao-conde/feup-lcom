#include "game.h"

void play(){
	MinixVice* game = startGame();
	gameLoop(game);
	endGame(game);
}

MinixVice* startGame() {

	srand(time(NULL));
	sef_startup();

	vg_init(GRAPHIC_MODE);

	MinixVice* game = getGame();

	return game;
}

void gameLoop(MinixVice* game) {

	while (!game->done) {
		drawMinixVice(game);
		updateMinixVice(game);
	}

}


void endGame(MinixVice* game){
	endMinixVice(game);
	vg_exit();
}
