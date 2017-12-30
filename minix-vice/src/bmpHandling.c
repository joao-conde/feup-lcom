#include "bmpHandling.h"


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
