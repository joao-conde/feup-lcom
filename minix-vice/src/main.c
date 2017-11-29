#include <stdio.h>
#include <time.h>

#include "MinixVice.h"
#include "mouse.h"

int main(int argc, char **argv) {
	printf("Start of MINIX-VICE!\n");

	srand(time(NULL));
	sef_startup();

	vg_init(0x105);

	MinixVice* game = initMinixVice();

	while (!game->done) {

		if (game->draw){
			//drawMinixVice(game);
		}

		drawMouse();

		updateMinixVice(game);
	}

	endMinixVice(game);

	vg_exit();
	return 0;
}
