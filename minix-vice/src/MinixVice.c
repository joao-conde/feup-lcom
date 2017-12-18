#include <minix/drivers.h>

#include "MinixVice.h"

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

int numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);

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

	createBarrels();

	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void createBarrels() {
	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i] = (Barrel*) malloc(sizeof(Barrel));
	}
}

void loadBarrelsBitmaps() {
	int i;
	//Loads bitmap only once but stores it multiple times (not relevant)
	Bitmap* barrelBitmap = loadBitmap(getImgPath("barrel"));
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i]->bitmap = barrelBitmap;
	}
}

void loadBitmaps() {

	game->background = loadBitmap(getImgPath("road"));
	game->menu_background = loadBitmap(getImgPath("main-menu"));
	game->settings_background = loadBitmap(getImgPath("temporary"));

	loadBarrelsBitmaps();

	game->car->bmpForward = loadBitmap(getImgPath("blue-car"));
	game->car->bmpTLeft = loadBitmap(getImgPath("blue-car-tl"));
	game->car->bmpTRight = loadBitmap(getImgPath("blue-car-tr"));

}

void deleteBitmaps() {
	deleteBitmap(game->background);
	deleteBitmap(game->menu_background);
	deleteBitmap(game->car->bmpForward);
	deleteBitmap(game->car->bmpTLeft);
	deleteBitmap(game->car->bmpTRight);
	deleteBitmap(game->barrels[0]->bitmap);
}

void initPlayer() {
	int carWidth, carHeight;

	MinixVice* game = getGame();

	carWidth = game->car->bmpForward->bitmapInfoHeader.width;
	carHeight = game->car->bmpForward->bitmapInfoHeader.height;

	game->car->x = vg_getHRES() / 2 - carWidth / 2;
	game->car->y = vg_getVRES() - 125 - carHeight;

	game->car->body = newColliderBox(game->car->x, game->car->y,
			game->car->x + carWidth, game->car->y + carHeight);
}

void initGameProperties() {
	MinixVice* game = getGame();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;
	game->score = 0;

	game->speed = INITIAL_SPEED;
}

void initMainMenu() {
	MinixVice* game = getGame();

	game->main_menu->playBtn = newColliderBox(95, 145, 392, 230);
	game->main_menu->quitBtn = newColliderBox(95, 257, 392, 340);
}

void initBarrels() {
	int i, barrelWidth, barrelHeight;

	barrelWidth = game->barrels[0]->bitmap->bitmapInfoHeader.width;
	barrelHeight = game->barrels[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < numberOfBarrels; i++) {

		game->barrels[i]->x = vg_getHRES() / 2 + i * 100;
		game->barrels[i]->y = 0;

		game->barrels[i]->body = newColliderBox(game->barrels[i]->x,
				game->barrels[i]->y, game->barrels[i]->x + barrelWidth,
				game->barrels[i]->y + barrelHeight);

	}
}

void initTimer(){
	MinixVice* game = getGame();

	game->timer->ticked = 0;
	game->timer->counter = 0;
}

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();

	createEntities();

	loadBitmaps();

	initPlayer();

	initGameProperties();

	initMainMenu();

	initBarrels();

	initTimer();

	return game;
}

MinixVice* getGame() {

	if (!game) {
		game = initMinixVice();
	}

	return game;
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

void brake() {
	MinixVice* game = getGame();

	if (game->speed <= 1) {
		game->speed = 1;
		return;
	}

	game->speed -= 1;
}

void accelerate() {
	MinixVice* game = getGame();

	game->speed += 0.5;
}

void updateMinixVice() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();

	int i, ipc_status, r = 0;
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

	switch (gameState) {
	case MAIN_MENU:

		if (clicked(game->main_menu->playBtn, m)) {
			updateGameState(PLAY);
		}

		if (clicked(game->main_menu->quitBtn, m)) {
			game->done = 1;
		}

		break;

	case GAME:

		if(game->timer->ticked){
			updateBarrelsPos();
		}

		for (i = 0; i < numberOfBarrels; i++) {
			if (collide(game->car->body, game->barrels[i]->body)) {
				printf("Car collided with barrel\n");
				game->done = 1;
			}
		}

		break;

	default: //TODO remove this?
		break;
	}

}

void drawMovingBackground() {
	static float y = 0;

	MinixVice* game = getGame();

	y += game->speed;

	if (y >= vg_getVRES())
		y = 0;

	drawBackgroundBitmap(game->background, 0, y, ALIGN_LEFT);
	drawBackgroundBitmap(game->background, 0, y - vg_getVRES(), ALIGN_LEFT);
}

void updateBarrelsPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i]->y += game->speed;
		game->barrels[i]->body->y1 += game->speed;
		game->barrels[i]->body->y2 += game->speed;
	}

}

void drawBarrels() {

	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		drawBarrel(game->barrels[i]);
	}

}

void drawMinixVice() {

	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		//TODO: draw menu function
		drawBackgroundBitmap(game->menu_background, 0, 0, ALIGN_LEFT);
		break;

	case OPTIONS:
		drawBackgroundBitmap(game->settings_background, 0, 0, ALIGN_LEFT);
		break;

	case GAME:
		if (game->timer->ticked)
			drawMovingBackground();

		drawPlayer(game->car);
		drawBarrels();

		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
		game->score++;
		//TODO display score
		flipDB();
	}

}


void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);

//	int i;
//	for (i = 0; i < numberOfBarrels; i++) {
//		free(game->barrels[i]);
//	}

	free(game);

}

