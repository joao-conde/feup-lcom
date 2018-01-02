#include "graphics.h"


extern st_game gameState;
extern st_player playerState;

void drawMinixVice() {
	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		drawMainMenu();
		break;

	case SELECT_MENU:
		drawSelectMenu();
		break;

	case HELP_MENU:
		drawBackgroundBitmap(game->help_screen, 0, 0);
		break;

	case GAME:
		drawGameScreen();
		break;

	case STATS_MENU:
		drawStatsMenu();
		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		flipDB();
		game->timer->ticked = 0;
	}

}

void drawMainMenu() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();

	drawBackgroundBitmap(game->main_menu->menu_background, 0, 0);

	if (hovered(game->main_menu->playBtn->button, m))
		drawBitmap(game->main_menu->playBtn->hover,
				game->main_menu->playBtn->button->x1,
				game->main_menu->playBtn->button->y1);
	else
		drawBitmap(game->main_menu->playBtn->normal,
				game->main_menu->playBtn->button->x1,
				game->main_menu->playBtn->button->y1);

	if (hovered(game->main_menu->quitBtn->button, m))
		drawBitmap(game->main_menu->quitBtn->hover,
				game->main_menu->quitBtn->button->x1,
				game->main_menu->quitBtn->button->y1);
	else
		drawBitmap(game->main_menu->quitBtn->normal,
				game->main_menu->quitBtn->button->x1,
				game->main_menu->quitBtn->button->y1);
}

void drawSelectMenu() {
	MinixVice* game = getGame();
	Mouse* m = getMouse();

	drawBackgroundBitmap(game->select_menu->select_background, 0, 0);

	if (hovered(game->select_menu->select_lamb->button, m))
		drawBitmap(game->select_menu->select_lamb->hover,
				game->select_menu->select_lamb->button->x1,
				game->select_menu->select_lamb->button->y1);
	else
		drawBitmap(game->select_menu->select_lamb->normal,
				game->select_menu->select_lamb->button->x1,
				game->select_menu->select_lamb->button->y1);

	if (hovered(game->select_menu->select_red->button, m))
		drawBitmap(game->select_menu->select_red->hover,
				game->select_menu->select_red->button->x1,
				game->select_menu->select_red->button->y1);
	else
		drawBitmap(game->select_menu->select_red->normal,
				game->select_menu->select_red->button->x1,
				game->select_menu->select_red->button->y1);

	if (hovered(game->select_menu->select_mercedes->button, m))
		drawBitmap(game->select_menu->select_mercedes->hover,
				game->select_menu->select_mercedes->button->x1,
				game->select_menu->select_mercedes->button->y1);
	else
		drawBitmap(game->select_menu->select_mercedes->normal,
				game->select_menu->select_mercedes->button->x1,
				game->select_menu->select_mercedes->button->y1);
}

void drawGameScreen() {
	MinixVice* game = getGame();

	if (game->timer->ticked) {
		drawMovingBackground();
		drawPlayer(game->car);
		drawBarrels();
		drawCones();
		drawAnimations();

		//score game 'tag'
		drawBitmap(game->score_tag, SCOREX_DISPLAY, SCOREY_DISPLAY);

		displayScore(SCOREX_OFFSET, SCOREY_OFFSET);

	}
}

void drawStatsMenu() {
	MinixVice* game = getGame();

	drawBackgroundBitmap(game->stats_screen, 0, 0);
	displayConesShot();
	displayDate();
	displayHour();
	displayScore(FINALSCOREX_OFFSET, FINALSCOREY_OFFSET);
}

void drawAnimations() {
	drawShotAnimations();
	drawBonusAnimations();
}

void drawBonusAnimations() {
	MinixVice* game = getGame();

	int i, x, y;

	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {
		if (game->bonusAnimations[i]->useAnimation != 0) {
			x = game->bonusAnimations[i]->x;
			y = game->bonusAnimations[i]->y;
			drawBitmap(game->bonusAnimations[i]->bonusBmp, x, y);

		}
	}
}

void drawShotAnimations() {
	MinixVice* game = getGame();

	int i, x, y, index;
	for (i = 0; i < MAX_CONESHOT_ANIM; i++) {
		if (game->shotAnimations[i]->useAnimation != 0) {
			x = game->shotAnimations[i]->x;
			y = game->shotAnimations[i]->y;
			index = game->shotAnimations[i]->bmpIndex;
			drawBitmap(game->shotAnimations[i]->sprites[index], x, y);

		}
	}
}

void drawPlayer() {

	MinixVice* game = getGame();

	switch (playerState) {
	case TLEFT:
		drawBitmap(game->car->bmpTLeft, game->car->x, game->car->y);
		break;
	case TRIGHT:
		drawBitmap(game->car->bmpTRight, game->car->x, game->car->y);
		break;
	default:
		drawBitmap(game->car->bmpForward, game->car->x, game->car->y);
		break;
	}

}

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y);
}

void drawCone(Cone* cone) {
	drawBitmap(cone->bitmap, cone->x, cone->y);
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

int drawInfo(int info, int startX, int startY, int offsetX, Bitmap* div,
		int lastElem) {
	MinixVice* game = getGame();

	int timesOffset = offsetX;

	if (info < 10) {

		drawBitmap(game->digits[info % 10], startX - CHAR_DISTANCE * offsetX,
				startY);

		offsetX++;
		timesOffset++;

		drawBitmap(game->digits[0], startX - CHAR_DISTANCE * offsetX, startY);

		offsetX++;
		timesOffset++;

	} else {

		while (info > 0) {
			drawBitmap(game->digits[info % 10],
					startX - CHAR_DISTANCE * offsetX, startY);
			info /= 10;
			offsetX++;
			timesOffset++;
		}
	}

	if (!lastElem) {
		drawBitmap(div, startX - CHAR_DISTANCE * offsetX, startY);
		timesOffset++;
	}

	return timesOffset;
}

void displayScore(int offsetX, int offsetY) {
	MinixVice* game = getGame();

	int score = (int) game->score;
	int i = 0, startX, startY;
	startX = vg_getHRES() - offsetX;
	startY = vg_getVRES() - offsetY;

	drawInfo(score, startX, startY, i, NULL, TRUE);

}

void displayDate() {
	MinixVice* game = getGame();

	unsigned long day = *(game->day);
	unsigned long month = *(game->month);
	unsigned long year = *(game->year);

	int offsetX = 0, startX, startY;
	Bitmap* div = game->utils[0];

	startX = vg_getHRES() / 2 + DATEX_OFFSET;
	startY = vg_getVRES() / 2 + DATEY_OFFSET;

	offsetX = drawInfo(year, startX, startY, offsetX, div, FALSE);
	offsetX = drawInfo(month, startX, startY, offsetX, div, FALSE);
	offsetX = drawInfo(day, startX, startY, offsetX, div, TRUE);
}

void displayHour() {
	MinixVice* game = getGame();

	unsigned long hours = *(game->hours);
	unsigned long minutes = *(game->minutes);
	unsigned long seconds = *(game->seconds);

	int offsetX = 0, startX, startY;
	Bitmap* div = game->utils[1];

	startX = vg_getHRES() / 2 + HOURX_OFFSET;
	startY = vg_getVRES() / 2 + HOURY_OFFSET;

	offsetX = drawInfo(seconds, startX, startY, offsetX, div, FALSE);
	offsetX = drawInfo(minutes, startX, startY, offsetX, div, FALSE);
	offsetX = drawInfo(hours, startX, startY, offsetX, div, TRUE);

}

void displayConesShot() {
	MinixVice* game = getGame();

	int conesShot = game->conesShot;
	int offsetX = 0, startX, startY;
	startX = vg_getHRES() - CONESX_OFFSET;
	startY = vg_getVRES() - CONESY_OFFSET;

	drawInfo(conesShot, startX, startY, offsetX, NULL, TRUE);
}

void drawMovingBackground() {
	static float backgroundY = 0;

	MinixVice* game = getGame();

	backgroundY += game->speed;

	if (backgroundY >= vg_getVRES())
		backgroundY = ORIGIN_COORDS;

	drawBackgroundBitmap(game->background, ORIGIN_COORDS, backgroundY);
	drawBackgroundBitmap(game->background, ORIGIN_COORDS,
			backgroundY - vg_getVRES());
}
