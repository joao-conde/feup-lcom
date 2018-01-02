#include "entities.h"
#include "MinixVice.h"


extern st_player playerState;

/* PLAYER METHODS */

void movePlayerLeft() {
	MinixVice* game = getGame();

	game->car->x -= TURN_SPEED * game->speed;
	game->car->body->x1 -= TURN_SPEED * game->speed;
	game->car->body->x2 -= TURN_SPEED * game->speed;

	if(game->car->x <= LEFT_ROAD_LIMIT)
		game->car->x = LEFT_ROAD_LIMIT;
}

void movePlayerRight() {
	MinixVice* game = getGame();
	int carWidth = game->car->bmpForward->bitmapInfoHeader.width;


	game->car->x += TURN_SPEED * game->speed;
	game->car->body->x1 += TURN_SPEED * game->speed;
	game->car->body->x2 += TURN_SPEED * game->speed;


	if(game->car->x >= (RIGHT_ROAD_LIMIT - carWidth))
		game->car->x = (RIGHT_ROAD_LIMIT - carWidth);
}



void createEntities() {
	MinixVice* game = getGame();

	game->main_menu = (MainMenu*) malloc(sizeof(MainMenu));
	game->main_menu->playBtn = (Button*) malloc(sizeof(Button));
	game->main_menu->quitBtn = (Button*) malloc(sizeof(Button));

	game->select_menu = (SelectMenu*) malloc(sizeof(SelectMenu));
	game->select_menu->select_lamb = (Button*) malloc(sizeof(Button));
	game->select_menu->select_red = (Button*) malloc(sizeof(Button));
	game->select_menu->select_mercedes = (Button*) malloc(sizeof(Button));

	createBarrels();
	createCones();

	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));

	createAnimations();

}

void createBarrels() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_BARRELS; i++) {
		game->barrels[i] = (Barrel*) malloc(sizeof(Barrel));
	}
}

void createAnimations(){
	createShotAnimations();
	createBonusAnimations();
}

void createBonusAnimations(){
	MinixVice* game = getGame();

	int i;
	for(i = 0; i < MAX_CONESHOT_ANIM; i++){
		game->bonusAnimations[i] = (Bonus*) malloc(sizeof(Bonus));
		game->bonusAnimations[i]->frame = 0;
		game->bonusAnimations[i]->useAnimation = 0;
	}
}


void createShotAnimations(){
	MinixVice* game = getGame();

	int i;
	for(i = 0; i < MAX_CONESHOT_ANIM; i++){
		game->shotAnimations[i] = (Shot*) malloc(sizeof(Shot));
		game->shotAnimations[i]->bmpIndex = 0;
		game->shotAnimations[i]->useAnimation = 0;
	}
}

void createCones() {
	MinixVice* game = getGame();

	int i;
	for (i = 0; i < NUMBER_OF_CONES; i++) {
		game->cones[i] = (Cone*) malloc(sizeof(Cone));
	}
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


	game->select_menu->select_lamb->button = newColliderBox(LAMBSELECTX1, LAMBSELECTY1,
	LAMBSELECTX2, LAMBSELECTY2);

	game->select_menu->select_red->button = newColliderBox(REDSELECTX1, REDSELECTY1,
	REDSELECTX2, REDSELECTY2);

	game->select_menu->select_mercedes->button = newColliderBox(MERCEDESSELECTX1,
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

		game->barrels[i]->y = -generateRandomPos(RANDOM_LOWERB, RANDOM_UPPERB);

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

		game->cones[i]->y = -generateRandomPos(RANDOM_LOWERB, RANDOM_UPPERB);

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

