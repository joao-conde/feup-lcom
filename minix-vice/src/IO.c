#include "IO.h"

extern st_game gameState;

void subscribeInterrupts() {
	MinixVice* game = getGame();

	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();
}

void unsubscribeInterrupts() {
	MinixVice* game = getGame();

	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();
}

void mouseIH() {
	mouseIntHandler();
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

		if (gameState == GAME) {
			switch (game->scancode) {

			case A_MAKE:
				movePlayerLeft(game->car);
				updatePlayerState(TLEFT);
				break;

			case D_MAKE:
				movePlayerRight(game->car);
				updatePlayerState(TRIGHT);
				break;

			case W_MAKE:
				accelerate();
				break;

			case S_MAKE:
				brake();
				break;

			default:
				updatePlayerState(DEFAULT);
				break;
			}
		}

	}

}

void readRTC() {
	MinixVice* game = getGame();

	do {
		if (!isRTCUpdating()) {
			getDate(game->day, game->month, game->year);
			getHour(game->hours, game->minutes, game->seconds);
		}

	} while (isRTCUpdating());

}

void interruptsHandler() {
	MinixVice* game = getGame();

	int ipc_status, r = 0;
	message msg;

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
}
