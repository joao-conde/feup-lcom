#include "graphics.h"

/* OBSTACLES METHODS */

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
