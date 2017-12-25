#include <minix/drivers.h>

#include "MinixVice.h"

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

	game->timer->ticked = 0;

	do {
		if (!isRTCUpdating()) {
			getDate(game->day, game->month, game->year);
			getHour(game->hours, game->minutes, game->seconds);
		}

	} while (isRTCUpdating());

	interruptsHandler();

	handleEvents();

}

void drawMinixVice() {

	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		drawBackgroundBitmap(game->main_menu->menu_background, 0, 0,
				ALIGN_LEFT);
		displayDate();
		break;

	case SELECT_MENU:
		drawBackgroundBitmap(game->select_menu->select_background, 0, 0,
				ALIGN_LEFT);
		break;

	case HELP_MENU:
		drawBackgroundBitmap(game->help_screen, 0, 0, ALIGN_LEFT);
		break;

	case GAME:

		if (game->timer->ticked) {
			drawMovingBackground();
			displayHour();
			drawPlayer(game->car);
			drawBarrels();
			drawCones();
		}

		break;

	case STATS_MENU:
		drawBackgroundBitmap(game->stats_screen, 0, 0, ALIGN_LEFT);
		displayConesShot();
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
	MinixVice* game = getGame();

	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);

	freeBarrels();
	freeCones();

	free(game);

}

void freeBarrels() {
	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		free(game->barrels[i]);
	}
}

void freeCones() {
	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		free(game->cones[i]);
	}
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
	int width, height, newX, newY;

	width = barrel->bitmap->bitmapInfoHeader.width;
	height = barrel->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);

	/* negative values ensure they come a while after disappearing
	 * and is random so they aren't always in the same height cord
	 */
	newY = -generateRandomPos(50, 600);

	barrel->x = newX;
	barrel->y = newY;

	barrel->body->x1 = newX;
	barrel->body->y1 = newY;

	barrel->body->x2 = newX + width;
	barrel->body->y2 = newY + height;

}

void recalculateConePos(Cone* cone) {
	int width, height, newX, newY;

	width = cone->bitmap->bitmapInfoHeader.width;
	height = cone->bitmap->bitmapInfoHeader.height;

	newX = generateRandomPos(LEFT_ROAD_LIMIT, RIGHT_ROAD_LIMIT - width);
	/*
	 * negative values for the same reason as the barrels
	 */
	newY = -generateRandomPos(50, 600);

	cone->x = newX;
	cone->y = newY;

	cone->body->x1 = newX;
	cone->body->y1 = newY;

	cone->body->x2 = newX + width;
	cone->body->y2 = newY + height;

}

void calculateScore() {
	game->score += SCORE_INCREASE(game->speed);
}

void drawMovingBackground() {
	static float y = 0;

	MinixVice* game = getGame();

	y += game->speed;

	if (y >= vg_getVRES())
		y = ORIGIN_COORDS;

	drawBackgroundBitmap(game->background, ORIGIN_COORDS, y, ALIGN_LEFT);
	drawBackgroundBitmap(game->background, ORIGIN_COORDS, y - vg_getVRES(),
			ALIGN_LEFT);
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

	int score = (int) game->score;
	int offset = SCORE_OFFSET, i = 0;

	while (score >= 1) {
		drawBitmap(game->digits[score % 10],
				(vg_getHRES() - offset) - offset * i, 0, ALIGN_LEFT);
		score /= 10;
		i++;
	}
}

void displayDate() {
	MinixVice* game = getGame();

	unsigned long day = *(game->day);
	unsigned long month = *(game->month);
	unsigned long year = *(game->year);

	int offset = SCORE_OFFSET, i = 0;

	while (year >= 1) {
		drawBitmap(game->digits[year % 10],
				(vg_getHRES() - offset) - offset * i, 0, ALIGN_LEFT);
		year /= 10;
		i++;
	}

	while (month >= 1) {
		drawBitmap(game->digits[month % 10],
				(vg_getHRES() - offset) - offset * i, 0, ALIGN_LEFT);
		month /= 10;
		i++;
	}

	while (day >= 1) {
		drawBitmap(game->digits[day % 10], (vg_getHRES() - offset) - offset * i,
				0, ALIGN_LEFT);
		day /= 10;
		i++;
	}
}

void displayHour() {
	MinixVice* game = getGame();

	unsigned long hours = *(game->hours);
	unsigned long minutes = *(game->minutes);
	unsigned long seconds = *(game->seconds);

	int offset = SCORE_OFFSET, i = 0;

	while (seconds >= 1) {
		drawBitmap(game->digits[seconds % 10],
				(vg_getHRES() - offset) - offset * i, 150, ALIGN_LEFT);
		seconds /= 10;
		i++;
	}

	while (minutes >= 1) {
		drawBitmap(game->digits[minutes % 10],
				(vg_getHRES() - offset) - offset * i, 150, ALIGN_LEFT);
		minutes /= 10;
		i++;
	}

	while (hours >= 1) {
		drawBitmap(game->digits[hours % 10],
				(vg_getHRES() - offset) - offset * i, 150, ALIGN_LEFT);
		hours /= 10;
		i++;
	}
}

void displayConesShot() {
	MinixVice* game = getGame();

	int conesShot = game->conesShot;
	int offset = 150, i = 0;

	if(conesShot == 0){
		drawBitmap(game->digits[0], (vg_getHRES() - offset) - offset * i, 300, ALIGN_LEFT);
		return;
	}

	while (conesShot >= 1) {
		drawBitmap(game->digits[conesShot % 10],
				(vg_getHRES() - offset) - offset * i, 300, ALIGN_LEFT);
		conesShot /= 10;
		i++;
	}
}

void handleEvents() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();
	int i;

	switch (gameState) {

	case MAIN_MENU:

		if (clicked(game->main_menu->playBtn, m)) {
			updateGameState(SELECT_CAR);
		}

		if (clicked(game->main_menu->quitBtn, m)) {
			updateGameState(TERMINATE);
		}

		if (game->scancode == H_BREAK)
			updateGameState(HELP);

		break;

	case HELP_MENU:

		if (game->scancode == ESC_BREAK)
			updateGameState(MAIN);

		break;

	case SELECT_MENU:

		if (clicked(game->select_menu->select_blue, m)) {
			loadCarBitmaps(1);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_lamb, m)) {
			loadCarBitmaps(2);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		if (clicked(game->select_menu->select_mercedes, m)) {
			loadCarBitmaps(3);
			initPlayer();

			updateMouseState(TARGET);
			updateGameState(PLAY);
		}

		break;

	case GAME:

		if (game->timer->ticked) {
			calculateScore();
			updateBarrelsPos();
			updateConesPos();
		}

		if (game->bonus) {
			game->bonus = 0;
			game->score += CONESHOT_BONUS;
		}

		for (i = 0; i < numberOfBarrels; i++) {

			if (collide(game->car->body, game->barrels[i]->body)) {
				updateGameState(TERMINATE);
			}

			//if barrel out of game screen re-calculate coordinates
			if (game->barrels[i]->y > vg_getVRES()) {
				recalculateBarrelPos(game->barrels[i]);
			}

		}

		for (i = 0; i < numberOfCones; i++) {

			if (collide(game->car->body, game->cones[i]->body)) {
				updateGameState(TERMINATE);
			}

			//shot cone
			if (clicked(game->cones[i]->body, m)) {
				recalculateConePos(game->cones[i]);
				game->bonus = 1;
				game->conesShot++;
			}

			//if cone out of game screen re-calculate coordinates
			if (game->cones[i]->y > vg_getVRES()) {
				recalculateConePos(game->cones[i]);
			}
		}

		break;

	case STATS_MENU:

		if (game->scancode == ESC_BREAK)
			updateGameState(TERMINATE);

		break;

	case OVER:
		game->done = 1;
		break;

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

//		if (game->scancode == ESC_BREAK) {
//			game->done = 1; //TODO DELETE THIS -> TEST PURPOSES
//		}

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

void interruptsHandler() {
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
	Bitmap* barrelBitmap = loadBitmap(getImgPath("barrel"));
	for (i = 0; i < numberOfBarrels; i++) {
		game->barrels[i]->bitmap = barrelBitmap;
	}
}

void loadConesBitmaps() {
	int i;
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
	game->help_screen = loadBitmap(getImgPath("help-placeholder"));
	game->stats_screen = loadBitmap(getImgPath("stats"));

	loadDigitBitmaps();

	loadBarrelsBitmaps();
	loadConesBitmaps();

}

void deleteDigitBitmaps() {
	deleteBitmap(game->digits[0]);
	deleteBitmap(game->digits[1]);
	deleteBitmap(game->digits[2]);
	deleteBitmap(game->digits[3]);
	deleteBitmap(game->digits[4]);
	deleteBitmap(game->digits[5]);
	deleteBitmap(game->digits[6]);
	deleteBitmap(game->digits[7]);
	deleteBitmap(game->digits[8]);
	deleteBitmap(game->digits[9]);
}

void deleteBarrelsBitmaps() {
	int i;
	for (i = 0; i < numberOfBarrels; i++) {
		deleteBitmap(game->barrels[i]->bitmap);
	}
}

void deleteConesBitmaps() {
	int i;
	for (i = 0; i < numberOfCones; i++) {
		deleteBitmap(game->cones[i]->bitmap);
	}
}

void deleteBitmaps() {
	deleteBitmap(game->background);
	deleteBitmap(game->main_menu->menu_background);
	deleteBitmap(game->select_menu->select_background);

	deleteDigitBitmaps();
//	deleteBarrelsBitmaps(); FIXME: game stops on end
//	deleteConesBitmaps();

	deleteBitmap(game->car->bmpForward);
	deleteBitmap(game->car->bmpTLeft);
	deleteBitmap(game->car->bmpTRight);
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
	game->scancode = 0;
	game->score = 0;

	game->bonus = 0;
	game->conesShot = 0;

	game->hours = (unsigned long*) malloc(sizeof(unsigned long));
	game->minutes = (unsigned long*) malloc(sizeof(unsigned long));
	game->seconds = (unsigned long*) malloc(sizeof(unsigned long));

	game->day = (unsigned long*) malloc(sizeof(unsigned long));
	game->month = (unsigned long*) malloc(sizeof(unsigned long));
	game->year = (unsigned long*) malloc(sizeof(unsigned long));

	game->speed = INITIAL_SPEED;
}

void initMainMenu() {
	MinixVice* game = getGame();

	game->main_menu->playBtn = newColliderBox(PLAYBTNX1, PLAYBTNY1, PLAYBTNX2,
	PLAYBTNY2);
	game->main_menu->quitBtn = newColliderBox(QUITBTNX1, QUITBTNY1, QUITBTNX2,
	QUITBTNY2);

}

void initSelectMenu() {
	MinixVice* game = getGame();

	game->select_menu->select_lamb = newColliderBox(LAMBSELECTX1, LAMBSELECTY1,
	LAMBSELECTX2, LAMBSELECTY2);
	game->select_menu->select_blue = newColliderBox(BLUESELECTX1, BLUESELECTY1,
	BLUESELECTX2, BLUESELECTY2);
	game->select_menu->select_mercedes = newColliderBox(MERCEDESSELECTX1,
	MERCEDESSELECTY1, MERCEDESSELECTX2, MERCEDESSELECTY2);

}

void initBarrels() {
	int i, barrelWidth, barrelHeight;

	barrelWidth = game->barrels[0]->bitmap->bitmapInfoHeader.width;
	barrelHeight = game->barrels[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < numberOfBarrels; i++) {

		game->barrels[i]->x = generateRandomPos(LEFT_ROAD_LIMIT,
		RIGHT_ROAD_LIMIT - barrelWidth);

		game->barrels[i]->y = -generateRandomPos(50, 600);

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

		game->cones[i]->x = generateRandomPos(LEFT_ROAD_LIMIT,
		RIGHT_ROAD_LIMIT - coneWidth);

		game->cones[i]->y = -generateRandomPos(50, 600);

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

