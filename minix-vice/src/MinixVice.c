#include <minix/drivers.h>

#include "MinixVice.h"
#include "rtc.h"

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

int numberOfBarrels = sizeof(game->barrels) / sizeof(Barrel*);
int numberOfCones = sizeof(game->cones) / sizeof(Cone*);

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();

	createEntities();

	loadBitmaps();

	initGameProperties();

	initMainMenu();

	initSelectMenu();

	initBarrels();

	initCones();

	initTimer();

	return game;
}

MinixVice* getGame() {

	if (!game) {
		game = initMinixVice();
	}

	return game;
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
			updateGameState(SETTINGS);
		}

		if (clicked(game->main_menu->quitBtn, m)) {
			game->done = 1;
		}

		break;

	case OPTIONS:

		if (clicked(game->select_menu->select_blue, m)) {
			loadCarBitmaps(1);
			initPlayer();

			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_lamb, m)) {
			loadCarBitmaps(2);
			initPlayer();

			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_mercedes, m)) {
			loadCarBitmaps(3);
			initPlayer();

			updateGameState(PLAY);
		}

		break;

	case GAME:

		if (game->timer->ticked) {
			calculateScore();
			updateBarrelsPos();
			updateConesPos();
		}

		for (i = 0; i < numberOfBarrels; i++) {

			if (collide(game->car->body, game->barrels[i]->body)) {
				game->done = 1;
			}

			//if barrel out of game screen re-calculate coordinates
			if (game->barrels[i]->y > vg_getVRES()) {
				recalculateBarrelPos(game->barrels[i]);
			}
		}

		for (i = 0; i < numberOfCones; i++) {

			if (collide(game->car->body, game->cones[i]->body)) {
				game->done = 1;
			}

			//if cone out of game screen re-calculate coordinates
			if (game->cones[i]->y > vg_getVRES()) {
				recalculateConePos(game->cones[i]);
			}
		}

		break;

	}

}

void drawMinixVice() {

	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		drawBackgroundBitmap(game->main_menu->menu_background, 0, 0,
				ALIGN_LEFT);
		break;

	case OPTIONS:
		drawBackgroundBitmap(game->select_menu->select_background, 0, 0,
				ALIGN_LEFT);
		break;

	case GAME:
		if (game->timer->ticked)
			drawMovingBackground();

		drawPlayer(game->car);
		drawBarrels();
		drawCones();

		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		game->timer->ticked = 0;
		displayScore();
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

void brake() {
	MinixVice* game = getGame();

	if (game->speed <= 1) {
		game->speed = 1;
		return;
	}

	game->speed -= BRAKE_SPEED;
}

void accelerate() {
	MinixVice* game = getGame();

	game->speed += ACCELERATE_SPEED;
}

void recalculateBarrelPos(Barrel* barrel) {
	int width, height, newX;

	width = barrel->bitmap->bitmapInfoHeader.width;
	height = barrel->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);

	barrel->x = newX;
	barrel->y = ORIGIN_COORDS;

	barrel->body->x1 = newX;
	barrel->body->y1 = ORIGIN_COORDS;

	barrel->body->x2 = newX + width;
	barrel->body->y2 = height;
}

void recalculateConePos(Cone* cone) {
	int width, height, newX;

	width = cone->bitmap->bitmapInfoHeader.width;
	height = cone->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);

	cone->x = newX;
	cone->y = ORIGIN_COORDS;

	cone->body->x1 = newX;
	cone->body->y1 = ORIGIN_COORDS;

	cone->body->x2 = newX + width;
	cone->body->y2 = height;
}

void calculateScore(){
	game->score += SCORE_INCREASE(game->speed);
}

void drawMovingBackground() {
	static float y = 0;

	MinixVice* game = getGame();

	y += game->speed;

	if (y >= vg_getVRES())
		y = ORIGIN_COORDS;

	drawBackgroundBitmap(game->background, ORIGIN_COORDS, y, ALIGN_LEFT);
	drawBackgroundBitmap(game->background, ORIGIN_COORDS, y - vg_getVRES(), ALIGN_LEFT);
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

void updateConesPos() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < numberOfCones; i++) {
		game->cones[i]->y += game->speed;
		game->cones[i]->body->y1 += game->speed;
		game->cones[i]->body->y2 += game->speed;
	}
}

void drawBarrels() {

	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		drawBarrel(game->barrels[i]);
	}

}

void drawCones() {

	int i;
	for (i = 0; i < numberOfCones; i++) {
		drawCone(game->cones[i]);
	}

}

void displayScore() {
	MinixVice* game = getGame();

	int score = game->score;
	int offset = SCORE_OFFSET, i = 0;

	while (score >= 1) {
		drawBitmap(game->digits[score % 10],
				(vg_getHRES() - offset) - offset * i, 0, ALIGN_LEFT);
		score /= 10;
		i++;
	}
}

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

void createEntities() {
	game->main_menu = (MainMenu*) malloc(sizeof(MainMenu));
	game->select_menu = (SelectMenu*) malloc(sizeof(SelectMenu));

	createBarrels();
	createCones();

	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void createBarrels() {
	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i] = (Barrel*) malloc(sizeof(Barrel));
	}
}

void createCones() {
	int i;
	for (i = 0; i < numberOfCones; i++) {
		game->cones[i] = (Cone*) malloc(sizeof(Cone));
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

void loadConesBitmaps() {
	int i;
	//Loads bitmap only once but stores it multiple times (not relevant)
	Bitmap* coneBitmap = loadBitmap(getImgPath("traffic-cone"));
	for (i = 0; i < numberOfCones; i++) {
		game->cones[i]->bitmap = coneBitmap;
	}
}

void loadDigitBitmaps() {
	game->digits[0] = loadBitmap(getImgPath("0"));
	game->digits[1] = loadBitmap(getImgPath("1"));
	game->digits[2] = loadBitmap(getImgPath("2"));
	game->digits[3] = loadBitmap(getImgPath("3"));
	game->digits[4] = loadBitmap(getImgPath("4"));
	game->digits[5] = loadBitmap(getImgPath("5"));
	game->digits[6] = loadBitmap(getImgPath("6"));
	game->digits[7] = loadBitmap(getImgPath("7"));
	game->digits[8] = loadBitmap(getImgPath("8"));
	game->digits[9] = loadBitmap(getImgPath("9"));
}

void loadCarBitmaps(int selectedCar) {

	switch (selectedCar) {
	case 1: //blue car
		game->car->bmpForward = loadBitmap(getImgPath("blue-car"));
		game->car->bmpTLeft = loadBitmap(getImgPath("blue-car-tl"));
		game->car->bmpTRight = loadBitmap(getImgPath("blue-car-tr"));
		break;

	case 2: //black lamb
		game->car->bmpForward = loadBitmap(getImgPath("lamb"));
		game->car->bmpTLeft = loadBitmap(getImgPath("lamb-tl"));
		game->car->bmpTRight = loadBitmap(getImgPath("lamb-tr"));
		break;

	case 3: //gray mercedes
		game->car->bmpForward = loadBitmap(getImgPath("mercedes"));
		game->car->bmpTLeft = loadBitmap(getImgPath("mercedes-tl"));
		game->car->bmpTRight = loadBitmap(getImgPath("mercedes-tr"));
		break;

	}

}

void loadBitmaps() {

	game->background = loadBitmap(getImgPath("road"));
	game->main_menu->menu_background = loadBitmap(getImgPath("main-menu"));
	game->select_menu->select_background = loadBitmap(getImgPath("carselect"));

	loadDigitBitmaps();

	loadBarrelsBitmaps();
	loadConesBitmaps();

}

void deleteBitmaps() {
	deleteBitmap(game->background);
	deleteBitmap(game->main_menu->menu_background);
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
	game->car->y = vg_getVRES() - CAR_OFFSET - carHeight;

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

	game->main_menu->playBtn = newColliderBox(PLAYBTNX1, PLAYBTNY1, PLAYBTNX2, PLAYBTNY2);
	game->main_menu->quitBtn = newColliderBox(QUITBTNX1, QUITBTNY1, QUITBTNX2, QUITBTNY2);

}

void initSelectMenu() {
	MinixVice* game = getGame();

	game->select_menu->select_lamb = newColliderBox(156, 230, 319, 568);
	game->select_menu->select_blue = newColliderBox(431, 230, 579, 568);
	game->select_menu->select_mercedes = newColliderBox(705, 230, 860, 568);

}

void initBarrels() {
	int i, barrelWidth, barrelHeight;

	barrelWidth = game->barrels[0]->bitmap->bitmapInfoHeader.width;
	barrelHeight = game->barrels[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < numberOfBarrels; i++) {

		game->barrels[i]->x = generateRandomPos(197, 823 - barrelWidth);
//		game->barrels[i]->y = generateRandomPos(0, vg_getVRES());

		game->barrels[i]->y = 0;

		game->barrels[i]->body = newColliderBox(game->barrels[i]->x,
				game->barrels[i]->y, game->barrels[i]->x + barrelWidth,
				game->barrels[i]->y + barrelHeight);

	}
}

void initCones() {
	int i, coneWidth, coneHeight;

	coneWidth = game->cones[0]->bitmap->bitmapInfoHeader.width;
	coneHeight = game->cones[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < numberOfCones; i++) {

		game->cones[i]->x = generateRandomPos(197, 823 - coneWidth);
//		game->cones[i]->y = generateRandomPos(0, vg_getVRES());

		game->cones[i]->y = 0;

		game->cones[i]->body = newColliderBox(game->cones[i]->x,
				game->cones[i]->y, game->cones[i]->x + coneWidth,
				game->cones[i]->y + coneHeight);

	}
}

void initTimer() {
	MinixVice* game = getGame();

	game->timer->ticked = 0;
	game->timer->counter = 0;
}


