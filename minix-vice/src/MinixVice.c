#include <minix/drivers.h>

#include "MinixVice.h"

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

void subscribeInterrupts() {
	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();
}

void unsubscribeInterrupts() {
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();
}

void createEntities() {
	game->main_menu = (MainMenu*) malloc(sizeof(MainMenu));
	game->barrel = (Barrel*) malloc(sizeof(Barrel));
	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void loadBitmaps() {

	game->background = loadBitmap(getImgPath("road"));
	game->menu_background = loadBitmap(getImgPath("main-menu"));
	game->barrel->bitmap = loadBitmap(getImgPath("barrel"));
	game->car->bmpForward = loadBitmap(getImgPath("blue-car"));
	game->car->bmpTLeft = loadBitmap(getImgPath("blue-car-tl"));
	game->car->bmpTRight = loadBitmap(getImgPath("blue-car-tr"));

	game->car->width = game->car->bmpForward->bitmapInfoHeader.width;
	game->car->height = game->car->bmpForward->bitmapInfoHeader.height;

}

void deleteBitmaps() {
	deleteBitmap(game->background);
	deleteBitmap(game->menu_background);
	deleteBitmap(game->car->bmpForward);
	deleteBitmap(game->car->bmpTLeft);
	deleteBitmap(game->car->bmpTRight);
	deleteBitmap(game->barrel->bitmap);
}

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();
	createEntities();
	loadBitmaps();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	/* MAIN MENU INIT */
	game->main_menu->playBtn = newColliderBox(95, 145, 392, 230);
	game->main_menu->quitBtn = newColliderBox(95, 257, 392, 340);

	/* ONE BARREL INIT */

	game->barrel->x = vg_getHRES() / 2;
	game->barrel->y = (vg_getVRES() / 2) - 200;
	game->barrel->width = game->barrel->bitmap->bitmapInfoHeader.width;
	game->barrel->height = game->barrel->bitmap->bitmapInfoHeader.height;

	game->timer->ticked = 0;
	game->timer->counter = 0;

	game->car->x = vg_getHRES() / 2;
	game->car->y = vg_getVRES() / 2;

	game->car->speed = PLAYER_SPEED;

	return game;
}

MinixVice* getGame() {

	if (!game) {
		game = initMinixVice();
	}

	return game;
}

//TODO: change this to work with mouse object
void mouseIH() {
	static int counter = 0;

	mouseIntHandler();

	Mouse* m = getMouse();
	MinixVice* game = getGame();

	if (m->RBtnDown)
		game->done = 1;

}


void timerIH() {
	MinixVice* game = getGame();

	game->timer->counter++;
	game->timer->ticked = 1;

	printf("TIMER interrupt\n");


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
			movePlayerLeft(game->car);
			updatePlayerState(TLEFT);
			break;
		case D_MAKE:
			movePlayerRight(game->car);
			updatePlayerState(TRIGHT);
			break;
		default:
			updatePlayerState(DEFAULT);
			break;
		}

	}

}

void updateMinixVice() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();

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


	if (gameState == MAIN_MENU) {
		if (m->LBtnDown) {

			if (clicked(game->main_menu->playBtn, m->x, m->y)) {
				updateGameState(PLAY);
			}

			if (clicked(game->main_menu->quitBtn, m->x, m->y)) {
				game->done = 1;
			}
		}
	}

}

void drawMovingBackground() {
	static int y = 0;
	if (++y == vg_getVRES())
		y = 0;

	drawBackgroundBitmap(game->background, 0, y, ALIGN_LEFT);
	drawBackgroundBitmap(game->background, 0, y - vg_getVRES(), ALIGN_LEFT);
}

void drawMinixVice() {

	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		//TODO: draw menu function
		drawBackgroundBitmap(game->menu_background, 0, 0, ALIGN_LEFT);
		break;

	case GAME:
		if (game->timer->ticked)
			drawMovingBackground();

		drawPlayer(game->car);
		drawBarrel(game->barrel);
		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
		flipDB();
	}

}

//first attempt at collisions
int check_collisions() {
	MinixVice* game = getGame();

	int playerX = game->car->x;
	int playerY = game->car->y;

	int xi = game->barrel->x;
	int xf = xi + game->barrel->width;

	int yi = game->barrel->y;
	int yf = yi + game->barrel->height;

	printf("PLAYER: %d - %d\n", playerX, playerY);
	printf("BARREL: %d - %d ; %d - %d\n", xi, yi, xf, yf);

	//TODO: AN ACTUAL COLLISION
	if (playerX > xf)
		game->done = 1;

	return 0;
}

void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);
	free(game->barrel);
	free(game);

}

