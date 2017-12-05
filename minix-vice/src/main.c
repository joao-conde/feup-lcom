#include <stdio.h>
#include <time.h>

#include "MinixVice.h"
#include "mouse.h"
#include "video_gr.h"

int main(int argc, char **argv) {
	printf("Start of MINIX-VICE!\n");
	srand(time(NULL));
	sef_startup();

	/* 16 bit modes
	 *
	 * 270(0x010E) - 320*200
	 * 273(0x0111) - 640*480
	 * 276(0x0114) - 800*600
	 * 279(0x0117) - 1024*768
	 * 282(0x011A) - 1280*1024
	 *
	 */
	vg_init(279);
	printf("%d * %d\n",vg_getHRES(), vg_getVRES());

	MinixVice* game = getGame();

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
