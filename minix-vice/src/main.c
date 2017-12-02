#include <stdio.h>
#include <time.h>

#include "MinixVice.h"
#include "mouse.h"
#include "video_gr.h"

int main(int argc, char **argv) {
	printf("Start of MINIX-VICE!\n");

	srand(time(NULL));
	sef_startup();

	vg_init(283);

	MinixVice* game = initMinixVice();

	while (!game->done) {

		if (game->draw) {
			drawMinixVice(game);
		}

		//flipDB();
		updateMinixVice(game);
	}

	endMinixVice(game);

	vg_exit();
	return 0;
}
