#include "game.h"

void play(){
	MinixVice* game = startGame();
	gameLoop(game);
	endGame(game);
}

MinixVice* startGame() {
	srand(time(NULL));
	sef_startup();

	/* 16 bit graphic modes
	 *
	 * 270(0x010E) - 320*200
	 * 273(0x0111) - 640*480
	 * 276(0x0114) - 800*600
	 * 279(0x0117) - 1024*768
	 * 282(0x011A) - 1280*1024
	 *
	 */
	vg_init(279);
	printf("%d * %d\n", vg_getHRES(), vg_getVRES());

	MinixVice* game = getGame();


	return game;
}

void gameLoop(MinixVice* game) {
	while (!game->done) {

		if (game->draw) {
			drawMinixVice(game);
		}

		updateMinixVice(game);
	}

}


void endGame(MinixVice* game){
	endMinixVice(game);
	vg_exit();
}
