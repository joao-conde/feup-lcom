#include <minix/drivers.h>

#include "MinixVice.h"

const int FPS = 60;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;
	game->background = loadBitmap("/home/minix-vice/res/images/ubuntu-desktop.bmp");


	/* TIMER INITIALIZATION */
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->ticked = 0;
	timer->counter = 0;
	game->timer = timer;


	/* PLAYER INITIALIZATION */
	Player* player = (Player*) malloc(sizeof(Player));
	//player->x = vg_getHRES() / 2;
	//player->y = vg_getVRES() / 2;

	player->y = 0;
	player->x = 0;


	player->deltaX = 0;
	player->deltaY = 0;
	player->speed = PLAYER_SPEED;
	player->bitmap = loadBitmap("/home/minix-vice/res/images/blue-car.bmp");
	game->car = player;

	return game;
}

MinixVice* getGame() {

	if (!game) {
		game = initMinixVice();
	}

	return game;
}

void timerIH() {
	MinixVice* game = getGame();

	game->timer->counter++;
	game->timer->ticked = 1;

}

void kbdIH() {
	MinixVice* game = getGame();

	game->scancode = kbc_read();

	if (game->scancode != 0) {

		if (game->scancode == ESC_BREAK) {
			game->done = 1;
		}

		switch (game->scancode) {

		case A_MAKE:
			printf("A pressed\n");
			movePlayerLeft(game->car);
			break;
		case D_MAKE:
			printf("D pressed\n");
			movePlayerRight(game->car);
			break;
		}
	}

}

void updateMinixVice() {

	MinixVice* game = getGame();

	int ipc_status, r = 0;
	message msg;

	game->timer->ticked = 0;

	if (driver_receive(ANY, &msg, &ipc_status) != 0)
		return;

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:

			if (msg.NOTIFY_ARG & game->irq_kbd) {
				kbdIH();
			}

			if (msg.NOTIFY_ARG & game->irq_timer) {
				timerIH();
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
		//drawMouse();
		//game->timer->ticked = 0;
		//flipDB();
	}
}

void drawMinixVice() {

	MinixVice* game = getGame();

	drawBackgroundBitmap(game->background, 0, 0, ALIGN_LEFT);
	drawPlayer(game->car);

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
	}

	flipDB();
}

void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();

	deleteMouse();
	free(game->timer);
	free(game->car);
	free(game);
}

