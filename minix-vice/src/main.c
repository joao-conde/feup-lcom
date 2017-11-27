#include <stdio.h>
#include <time.h>

#include "MinixVice.h"

int main(int argc, char **argv) {
	printf("Start of MINIX-VICE!\n");

	srand(time(NULL));
	sef_startup();

	vg_init(0x105);

	MinixVice* game = (MinixVice*) malloc(sizeof(MinixVice));

	while (!game->done) {

		if (game->draw)
			drawMinixVice(game);

		updateMinixVice(game);
	}

	endMinixVice(game);

	vg_exit();
	return 0;
}
