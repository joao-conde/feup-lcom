#include <minix/drivers.h>

#include "MinixVice.h"

const int FPS = 60;

MinixVice* initMinixVice() {
	MinixVice* game = (MinixVice*) malloc(sizeof(MinixVice));

	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->ticked = 0;
	timer->counter = 0;
	game->timer = timer;

	Player* player = (Player*) malloc(sizeof(Player));
	player->x = 0;
	player->y = 0;
	game->car = player;

	if ((game->test = loadBitmap("/home/minix-vice/res/images/test.bmp"))
			== NULL)
		printf("NULL BITMAP PTR\n");

	return game;
}

void kbdInputHandler(MinixVice* game) {
	if (game->scancode != 0) {

		if (game->scancode == ESC_BREAK) {
			game->done = 1;
		}

		switch (game->scancode) {

		//		case W_MAKE:
		//			printf("W pressed\n");
		//			break;
		//		case S_MAKE:
		//			printf("S pressed\n");
		//			break;
		case A_MAKE:
			printf("A pressed\n");
			drawSquare(game->car->x, game->car->y, 50, 0);
			movePlayerLeft(game->car);
			break;
		case D_MAKE:
			printf("D pressed\n");
			drawSquare(game->car->x, game->car->y, 50, 0);
			movePlayerRight(game->car);
			break;
		}
	}

}


void updateMinixVice(MinixVice* game) {

	int ipc_status, r = 0;
	message msg;

	game->timer->ticked = 0;

	if (driver_receive(ANY, &msg, &ipc_status) != 0)
		return;

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:

			if (msg.NOTIFY_ARG & game->irq_kbd) {
				game->scancode = kbc_read();
				kbdInputHandler(game);
			}

			if (msg.NOTIFY_ARG & game->irq_timer) {
				game->timer->counter++;
				game->timer->ticked = 1;
			}

			if (msg.NOTIFY_ARG & game->irq_mouse) {
				mouseIH();
			}

			break;
		default:
			break;
		}
	}

	if (game->timer->ticked) {
		updateMouse();
		drawMouse();
		game->timer->ticked = 0;
	}

//	if (game->scancode != 0) {
//
//		if (game->scancode == ESC_BREAK) {
//			game->done = 1;
//		}
//
//		switch (game->scancode) {
//
////		case W_MAKE:
////			printf("W pressed\n");
////			break;
////		case S_MAKE:
////			printf("S pressed\n");
////			break;
//		case A_MAKE:
//			printf("A pressed\n");
//			movePlayerLeft(game);
//			break;
//		case D_MAKE:
//			printf("D pressed\n");
//			break;
//		}
//	}
}

void drawMinixVice(MinixVice* game) {
	drawSquare(game->car->x, game->car->y, 50, 10);
	printf("BEFORE DRAWBITMAP\n");
	drawBitmap(game->test, 0, 0, ALIGN_LEFT);

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
	}
}

void endMinixVice(MinixVice* game) {
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();

	deleteBitmap(game->test);
	deleteMouse();
	free(game->timer);
	free(game->car);
	free(game);
}



