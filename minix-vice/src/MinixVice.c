#include <minix/drivers.h>

#include "MinixVice.h"
#include "mouse.h"

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

	return game;
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

	if (game->scancode != 0) {
		if (game->scancode == ESC_BREAK)
			game->done = 1;
	}
}





void drawMinixVice(MinixVice* game) {
	drawSquare(200, 200, 10, 10);

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
	}
}



void endMinixVice(MinixVice* game) {
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();

	deleteMouse();
	free(game->timer);
	free(game);
}
