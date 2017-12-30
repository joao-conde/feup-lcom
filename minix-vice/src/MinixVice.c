#include "MinixVice.h"

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

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

	if (game == NULL) {
		game = initMinixVice();
	}

	return game;
}

void updateMinixVice() {

	MinixVice* game = getGame();

	game->timer->ticked = 0;

	readRTC();

	interruptsHandler();

	handleEvents();

}

void drawMinixVice() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();

	switch (gameState) {

	case MAIN_MENU:
		drawBackgroundBitmap(game->main_menu->menu_background, 0, 0,
				ALIGN_LEFT);

		if (hovered(game->main_menu->playBtn->button, m))
			drawBitmap(game->main_menu->playBtn->hover,
					game->main_menu->playBtn->button->x1,
					game->main_menu->playBtn->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->main_menu->playBtn->normal,
					game->main_menu->playBtn->button->x1,
					game->main_menu->playBtn->button->y1, ALIGN_LEFT);

		if (hovered(game->main_menu->quitBtn->button, m))
			drawBitmap(game->main_menu->quitBtn->hover,
					game->main_menu->quitBtn->button->x1,
					game->main_menu->quitBtn->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->main_menu->quitBtn->normal,
					game->main_menu->quitBtn->button->x1,
					game->main_menu->quitBtn->button->y1, ALIGN_LEFT);

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
			drawPlayer(game->car);
			drawBarrels();
			drawCones();
			displayScore(SCOREX_OFFSET,SCOREY_OFFSET);
		}

		break;

	case STATS_MENU:
		drawBackgroundBitmap(game->stats_screen, 0, 0, ALIGN_LEFT);
		displayConesShot();
		displayDate();
		displayHour();
		displayScore(FINALSCOREX_OFFSET,FINALSCOREY_OFFSET);
		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		flipDB();
		game->timer->ticked = 0;
	}

}

void endMinixVice() {
	MinixVice* game = getGame();

	cleanOBF();
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
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		free(game->barrels[i]);
	}
}

void freeCones() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		free(game->cones[i]);
	}
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


void drawBarrels() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		drawBarrel(game->barrels[i]);
	}

}

void drawCones() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		drawCone(game->cones[i]);
	}

}

void displayScore(int offsetX, int offsetY) {
	MinixVice* game = getGame();

	int score = (int) game->score;
	int i = 0, startX, startY;
	startX = vg_getHRES() - offsetX;
	startY = vg_getVRES() - offsetY;

	if (score == 0) {
		drawBitmap(game->digits[0],
				startX - CHAR_DISTANCE * i,
				startY,
				ALIGN_LEFT);
		return;
	}

	while (score >= 1) {
		drawBitmap(game->digits[score % 10], startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		score /= 10;
		i++;
	}
}

void displayDate() {
	MinixVice* game = getGame();

	unsigned long day = *(game->day);
	unsigned long month = *(game->month);
	unsigned long year = *(game->year);

	int i = 0, startX, startY;
	startX = vg_getHRES()/2 + DATEX_OFFSET;
	startY = vg_getVRES()/2 + DATEY_OFFSET;

	while (year >= 1) {
		drawBitmap(game->digits[year % 10],
				startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		year /= 10;
		i++;
	}

	drawBitmap(game->utils[0], startX - CHAR_DISTANCE * i,
			startY, ALIGN_LEFT);
	i++;

	while (month >= 1) {
		drawBitmap(game->digits[month % 10],
				startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		month /= 10;
		i++;
	}

	drawBitmap(game->utils[0], startX - CHAR_DISTANCE * i,
			startY, ALIGN_LEFT);
	i++;

	while (day >= 1) {
		drawBitmap(game->digits[day % 10], startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		day /= 10;
		i++;
	}
}

void displayHour() {
	MinixVice* game = getGame();

	unsigned long hours = *(game->hours);
	unsigned long minutes = *(game->minutes);
	unsigned long seconds = *(game->seconds);

	int i = 0, startX, startY;
	startX = vg_getHRES()/2 + HOURX_OFFSET;
	startY = vg_getVRES()/2 + HOURY_OFFSET;

	while (seconds >= 1) {
		drawBitmap(game->digits[seconds % 10],
				startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
		seconds /= 10;
		i++;
	}

	drawBitmap(game->utils[1], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (minutes >= 1) {
		drawBitmap(game->digits[minutes % 10],
				startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
		minutes /= 10;
		i++;
	}

	drawBitmap(game->utils[1], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (hours >= 1) {
		drawBitmap(game->digits[hours % 10],
				startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
		hours /= 10;
		i++;
	}
}

void displayConesShot() {
	MinixVice* game = getGame();

	int conesShot = game->conesShot;
	int i = 0, startX, startY;
	startX = vg_getHRES() - CONESX_OFFSET;
	startY = vg_getVRES() - CONESY_OFFSET;

	if (conesShot == 0) {
		drawBitmap(game->digits[0],
				startX - CHAR_DISTANCE * i,
				startY,	ALIGN_LEFT);
		return;
	}

	while (conesShot >= 1) {
		drawBitmap(game->digits[conesShot % 10],
				startX - CHAR_DISTANCE * i, startY,	ALIGN_LEFT);
		conesShot /= 10;
		i++;
	}
}

void startNewGame() {

	recalculateBarrelsPos();
	recalculateConesPos();

	game->speed = INITIAL_SPEED;
	game->bonus = 0;
	game->score = 0;
	game->conesShot = 0;

	updateGameState(PLAY);
	updateMouseState(MENU);
}


void handleEvents() {

	MinixVice* game = getGame();
	Mouse* m = getMouse();
	int i;

	switch (gameState) {

	case MAIN_MENU:

		if (clicked(game->main_menu->playBtn->button, m)) {
			updateGameState(SELECT_CAR);
		}

		if (clicked(game->main_menu->quitBtn->button, m)) {
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

		if (clicked(game->select_menu->select_red, m)) {
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

		for (i = 0; i < NUMBER_OF_BARRELS; i++) {

			if (collide(game->car->body, game->barrels[i]->body)) {
				updateGameState(TERMINATE);
			}

			//if barrel out of game screen re-calculate coordinates
			if (game->barrels[i]->y > vg_getVRES()) {
				recalculateBarrelPos(game->barrels[i]);
			}

		}

		for (i = 0; i < NUMBER_OF_CONES; i++) {

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

		if (game->scancode == ESC_BREAK) {
			startNewGame();
		}

		break;

	case OVER:
		game->done = 1;
		break;

	}
}


void createEntities() {
	MinixVice* game = getGame();

	game->main_menu = (MainMenu*) malloc(sizeof(MainMenu));
	game->main_menu->playBtn = (Button*) malloc(sizeof(Button));
	game->main_menu->quitBtn = (Button*) malloc(sizeof(Button));

	game->select_menu = (SelectMenu*) malloc(sizeof(SelectMenu));

	createBarrels();
	createCones();

	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void createBarrels() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		game->barrels[i] = (Barrel*) malloc(sizeof(Barrel));
	}
}

void createCones() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		game->cones[i] = (Cone*) malloc(sizeof(Cone));
	}
}

void loadBarrelsBitmaps() {
	MinixVice* game = getGame();

	int i;
	Bitmap* barrelBitmap = loadBitmap(getImgPath("barrel"));
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		game->barrels[i]->bitmap = barrelBitmap;
	}
}

void loadConesBitmaps() {
	MinixVice* game = getGame();

	int i;
	Bitmap* coneBitmap = loadBitmap(getImgPath("traffic-cone"));
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		game->cones[i]->bitmap = coneBitmap;
	}
}

void loadDigitBitmaps() {
	MinixVice* game = getGame();

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

	MinixVice* game = getGame();

	switch (selectedCar) {
	case 1: //red car
		game->car->bmpForward = loadBitmap(getImgPath("red"));
		game->car->bmpTLeft = loadBitmap(getImgPath("red-tl"));
		game->car->bmpTRight = loadBitmap(getImgPath("red-tr"));
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
	MinixVice* game = getGame();

	game->background = loadBitmap(getImgPath("road"));

	game->main_menu->menu_background = loadBitmap(getImgPath("main-menu"));
	game->main_menu->playBtn->normal = loadBitmap(getImgPath("playbtn"));
	game->main_menu->quitBtn->normal = loadBitmap(getImgPath("quitbtn"));

	game->main_menu->playBtn->hover = loadBitmap(getImgPath("playbtnhover"));
	game->main_menu->quitBtn->hover = loadBitmap(getImgPath("quitbtnhover"));

	game->select_menu->select_background = loadBitmap(getImgPath("carselect"));
	game->help_screen = loadBitmap(getImgPath("help"));
	game->stats_screen = loadBitmap(getImgPath("stats"));

	game->utils[0] = loadBitmap(getImgPath("slash"));
	game->utils[1] = loadBitmap(getImgPath("points"));

	loadDigitBitmaps();

	loadBarrelsBitmaps();
	loadConesBitmaps();

}

void deleteDigitBitmaps() {
	MinixVice* game = getGame();

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
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		deleteBitmap(game->barrels[i]->bitmap);
	}
}

void deleteConesBitmaps() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		deleteBitmap(game->cones[i]->bitmap);
	}
}

void deleteBitmaps() {
	MinixVice* game = getGame();
//
//	deleteBitmap(game->background);
//	deleteBitmap(game->main_menu->menu_background);
//	deleteBitmap(game->select_menu->select_background);


	deleteDigitBitmaps();
//	deleteBarrelsBitmaps();
//	deleteConesBitmaps();

	deleteBitmap(game->car->bmpForward);
	deleteBitmap(game->car->bmpTLeft);
	deleteBitmap(game->car->bmpTRight);
}

void initPlayer() {
	MinixVice* game = getGame();

	int carWidth, carHeight;

	carWidth = game->car->bmpForward->bitmapInfoHeader.width;
	carHeight = game->car->bmpForward->bitmapInfoHeader.height;

	game->car->x = vg_getHRES() / 2 - carWidth / 2;
	game->car->y = vg_getVRES() - CAR_OFFSET - carHeight;

	game->car->body = newColliderBox(game->car->x + COLBOX_MARGIN,
			game->car->y + COLBOX_MARGIN,
			game->car->x + carWidth - COLBOX_MARGIN,
			game->car->y + carHeight - COLBOX_MARGIN);
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

	game->main_menu->playBtn->button = newColliderBox(PLAYBTNX1, PLAYBTNY1,
	PLAYBTNX2,
	PLAYBTNY2);

	game->main_menu->quitBtn->button = newColliderBox(QUITBTNX1, QUITBTNY1,
	QUITBTNX2,
	QUITBTNY2);

}

void initSelectMenu() {
	MinixVice* game = getGame();

	game->select_menu->select_lamb = newColliderBox(LAMBSELECTX1, LAMBSELECTY1,
	LAMBSELECTX2, LAMBSELECTY2);

	game->select_menu->select_red = newColliderBox(REDSELECTX1, REDSELECTY1,
	REDSELECTX2, REDSELECTY2);

	game->select_menu->select_mercedes = newColliderBox(MERCEDESSELECTX1,
	MERCEDESSELECTY1, MERCEDESSELECTX2, MERCEDESSELECTY2);

}

void initBarrels() {
	MinixVice* game = getGame();

	int i, barrelWidth, barrelHeight;

	barrelWidth = game->barrels[0]->bitmap->bitmapInfoHeader.width;
	barrelHeight = game->barrels[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < NUMBER_OF_BARRELS; i++) {

		game->barrels[i]->x = generateRandomPos(LEFT_ROAD_LIMIT,
		RIGHT_ROAD_LIMIT - barrelWidth);

		game->barrels[i]->y = -generateRandomPos(50, 600);

		game->barrels[i]->body = newColliderBox(game->barrels[i]->x,
				game->barrels[i]->y, game->barrels[i]->x + barrelWidth,
				game->barrels[i]->y + barrelHeight);

	}
}

void initCones() {
	MinixVice* game = getGame();

	int i, coneWidth, coneHeight;

	coneWidth = game->cones[0]->bitmap->bitmapInfoHeader.width;
	coneHeight = game->cones[0]->bitmap->bitmapInfoHeader.height;

	for (i = 0; i < NUMBER_OF_CONES; i++) {

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

