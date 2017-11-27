#include <minix/drivers.h>

#include "MinixVice.h"
#include "kbd.h"

const int FPS = 60;

MinixVice* initMinixVice() {
	MinixVice* game = (MinixVice*) malloc(sizeof(MinixVice));

	game->irq_set_kbd = kbd_subscribe_int();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	return game;
}

void updateMinixVice(MinixVice* game) {

	int ipc_status, r = 0;
	message msg;

	if (driver_receive(ANY, &msg, &ipc_status) != 0)
		return;

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & game->irq_set_kbd) {
				game->scancode = kbc_read();
			}
			break;
		default:
			break;
		}
	}

	if (game->scancode != 0) {
		if (game->scancode == ESC_BREAK)
			game->done = 1;
	}
}

void drawMinixVice(MinixVice* game) {
	drawSquare(0, 0, 10, 10);
}

void endMinixVice(MinixVice* game) {
	kbd_unsubscribe_int();

	free(game);
}
