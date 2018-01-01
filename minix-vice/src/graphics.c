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

void displayDate() {
	MinixVice* game = getGame();

	unsigned long day = *(game->day);
	unsigned long month = *(game->month);
	unsigned long year = *(game->year);

	int i = 0, startX, startY;
	startX = vg_getHRES() / 2 + DATEX_OFFSET;
	startY = vg_getVRES() / 2 + DATEY_OFFSET;

	while (year >= 1) {
		drawBitmap(game->digits[year % 10], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
		year /= 10;
		i++;
	}

	drawBitmap(game->utils[0], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (month >= 1) {
		drawBitmap(game->digits[month % 10], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
		month /= 10;
		i++;
	}

	drawBitmap(game->utils[0], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (day >= 1) {
		drawBitmap(game->digits[day % 10], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
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
	startX = vg_getHRES() / 2 + HOURX_OFFSET;
	startY = vg_getVRES() / 2 + HOURY_OFFSET;

	while (seconds >= 1) {
		drawBitmap(game->digits[seconds % 10], startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		seconds /= 10;
		i++;
	}

	drawBitmap(game->utils[1], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (minutes >= 1) {
		drawBitmap(game->digits[minutes % 10], startX - CHAR_DISTANCE * i,
				startY, ALIGN_LEFT);
		minutes /= 10;
		i++;
	}

	drawBitmap(game->utils[1], startX - CHAR_DISTANCE * i, startY, ALIGN_LEFT);
	i++;

	while (hours >= 1) {
		drawBitmap(game->digits[hours % 10], startX - CHAR_DISTANCE * i, startY,
				ALIGN_LEFT);
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
