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

	createBarrels();

	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void createBarrels() {
	//TODO create a function to create the barrels
	int i, numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i] = (Barrel*) malloc(sizeof(Barrel));
		printf("Created barrel %d!\n", i);
	}
}

void loadBitmaps() {

	game->background = loadBitmap(getImgPath("road"));
	game->menu_background = loadBitmap(getImgPath("main-menu"));

	//TODO create a function to create the barrels bitmaps
	int i, numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i]->bitmap = loadBitmap(getImgPath("barrel"));
		printf("Created barrel bitmap %d!\n", i);
	}

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

	//TODO create a function to delete the barrels bitmaps
	int i, numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
	for (i = 0; i < numberOfBarrels; i++) {
		deleteBitmap(game->barrels[i]->bitmap);
		printf("Deleted barrel bitmap %d!\n", i);
	}
}

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();
	createEntities();
	loadBitmaps();

	/* GAME PROPERTIES INIT */
	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	game->speed = INITIAL_SPEED;

	/* MAIN MENU INIT */
	game->main_menu->playBtn = newColliderBox(95, 145, 392, 230);
	game->main_menu->quitBtn = newColliderBox(95, 257, 392, 340);

	/* TWO BARRELS INIT */

	//TODO create a function to initialize the barrels properties
	int i, numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i]->x = vg_getHRES() / 2 + i * 100;
		game->barrels[i]->y = (vg_getVRES() / 2) - 200;

		game->barrels[i]->width =
				game->barrels[i]->bitmap->bitmapInfoHeader.width;
		game->barrels[i]->height =
				game->barrels[i]->bitmap->bitmapInfoHeader.height;

		//TODO create a box around the barrels, not the center point
		game->barrels[i]->body = newColliderBox(game->barrels[i]->x,
				game->barrels[i]->y, game->barrels[i]->x, game->barrels[i]->y);

		printf("Initialized barrel %d!\n", i);
	}

	/* TIMER INIT */
	game->timer->ticked = 0;
	game->timer->counter = 0;

	/* PLAYER INIT */
	game->car->x = vg_getHRES() / 2
			- game->car->bmpForward->bitmapInfoHeader.width / 2;
	game->car->y = vg_getVRES() - 125
			- game->car->bmpForward->bitmapInfoHeader.height;
	game->car->body = newColliderBox(game->car->x, game->car->y, game->car->x,
			game->car->y);
	//TODO create a box around the car, not the center point

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

		//TODO TEST COLLISIONS TO ALL OBJECTS

//		if(collide(game->car->body,game->barrel->body)){
//			printf("Car collided with barrel\n");
//			//game->done = 1; TODO uncomment, for now every collision is true
//		}

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

		//TODO create a function to draw the barrels
		int i, numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
		for (i = 0; i < numberOfBarrels; i++) {
			drawBarrel(game->barrels[i]);
			printf("Drew barrel %d!\n", i);
		}

		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
		flipDB();
	}

}

////first attempt at collisions
//int check_collisions() {
//	MinixVice* game = getGame();
//
//	int playerX = game->car->x;
//	int playerY = game->car->y;
//
//	int xi = game->barrel->x;
//	int xf = xi + game->barrel->width;
//
//	int yi = game->barrel->y;
//	int yf = yi + game->barrel->height;
//
//	printf("PLAYER: %d - %d\n", playerX, playerY);
//	printf("BARREL: %d - %d ; %d - %d\n", xi, yi, xf, yf);
//
//	//TODO: AN ACTUAL COLLISION
//	if (playerX > xf)
//		game->done = 1;
//
//	return 0;
//}

void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);
//	free(game->barrel);
	free(game);

}

