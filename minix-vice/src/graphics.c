#include "graphics.h"

/* OBSTACLES METHODS */

extern st_game gameState;

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

		if (hovered(game->select_menu->select_lamb->button, m))
			drawBitmap(game->select_menu->select_lamb->hover,
					game->select_menu->select_lamb->button->x1,
					game->select_menu->select_lamb->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_lamb->normal,
					game->select_menu->select_lamb->button->x1,
					game->select_menu->select_lamb->button->y1, ALIGN_LEFT);

		if (hovered(game->select_menu->select_red->button, m))
			drawBitmap(game->select_menu->select_red->hover,
					game->select_menu->select_red->button->x1,
					game->select_menu->select_red->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_red->normal,
					game->select_menu->select_red->button->x1,
					game->select_menu->select_red->button->y1, ALIGN_LEFT);

		if (hovered(game->select_menu->select_mercedes->button, m))
			drawBitmap(game->select_menu->select_mercedes->hover,
					game->select_menu->select_mercedes->button->x1,
					game->select_menu->select_mercedes->button->y1, ALIGN_LEFT);
		else
			drawBitmap(game->select_menu->select_mercedes->normal,
					game->select_menu->select_mercedes->button->x1,
					game->select_menu->select_mercedes->button->y1, ALIGN_LEFT);

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
			drawAnimations();

			drawBitmap(game->score_tag, SCOREX_DISPLAY, SCOREY_DISPLAY,
					ALIGN_LEFT);

			displayScore(SCOREX_OFFSET, SCOREY_OFFSET);

		}

		break;

	case STATS_MENU:
		drawBackgroundBitmap(game->stats_screen, 0, 0, ALIGN_LEFT);
		displayConesShot();
		displayDate();
		displayHour();
		displayScore(FINALSCOREX_OFFSET, FINALSCOREY_OFFSET);
		break;

	}

	if (game->timer->ticked) {
		drawMouse();
		flipDB();
		game->timer->ticked = 0;
	}

}

void drawAnimations() {
	MinixVice* game = getGame();

	int i, x, y, index;
	for (i = 0; i < 3; i++) {
		if (game->shotAnimations[i]->useAnimation != 0) {
			x = game->shotAnimations[i]->x;
			y = game->shotAnimations[i]->y;
			index = game->shotAnimations[i]->bmpIndex;
			drawBitmap(game->shotAnimations[i]->sprites[index], x, y,
					ALIGN_LEFT);

		}
	}
}

void drawBarrel(Barrel* barrel) {
	drawBitmap(barrel->bitmap, barrel->x, barrel->y, ALIGN_LEFT);
}

void drawCone(Cone* cone) {
	drawBitmap(cone->bitmap, cone->x, cone->y, ALIGN_LEFT);
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
		drawBitmap(game->digits[0], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
		return;
	}

	while (score >= 1) {
		drawBitmap(game->digits[score % 10], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
		score /= 10;
		i++;
	}
}


int drawTimeInfo(int info, int startX, int startY, int offsetX, Bitmap* div, int lastElem){
	MinixVice* game = getGame();

	int timesOffset = offsetX;

	if (info < 10) {

		drawBitmap(game->digits[info % 10], startX - CHAR_DISTANCE * offsetX, startY,
				ALIGN_LEFT);

		offsetX++;
		timesOffset++;

		drawBitmap(game->digits[0], startX - CHAR_DISTANCE * offsetX, startY,
				ALIGN_LEFT);
		offsetX++;
		timesOffset++;

	} else {

		while (info > 0) {
			drawBitmap(game->digits[info % 10], startX - CHAR_DISTANCE * offsetX,
					startY, ALIGN_LEFT);
			info /= 10;
			offsetX++;
			timesOffset++;
		}
	}

	if(!lastElem){
		drawBitmap(div, startX - CHAR_DISTANCE * offsetX, startY, ALIGN_LEFT);
		timesOffset++;
	}

	return timesOffset;
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

	offsetX = drawTimeInfo(year, startX, startY, offsetX, div, FALSE);
	offsetX = drawTimeInfo(month, startX, startY,offsetX, div, FALSE);
	offsetX = drawTimeInfo(day, startX, startY, offsetX, div, TRUE);
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

	offsetX = drawTimeInfo(seconds, startX, startY, offsetX, div, FALSE);
	offsetX = drawTimeInfo(minutes, startX, startY,offsetX, div, FALSE);
	offsetX = drawTimeInfo(hours, startX, startY, offsetX, div, TRUE);

}

void displayConesShot() {
	MinixVice* game = getGame();

	int conesShot = game->conesShot;
	int i = 0, startX, startY;
	startX = vg_getHRES() - CONESX_OFFSET;
	startY = vg_getVRES() - CONESY_OFFSET;

	if (conesShot == 0) {
		drawBitmap(game->digits[0], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
		return;
	}

	while (conesShot >= 1) {
		drawBitmap(game->digits[conesShot % 10], startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		conesShot /= 10;
		i++;
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

//void drawConeBonus(int x, int y){
//	MinixVice* game = getGame();
//
//	drawBitmap(game->utils[2], x, y, ALIGN_LEFT);
//}
