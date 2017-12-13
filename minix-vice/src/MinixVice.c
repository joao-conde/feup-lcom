#include <minix/drivers.h>

#include "MinixVice.h"

const int FPS = 60;

extern st_game gameState;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;


void subscribeInterrupts(){
	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();
}

void unsubscribeInterrupts(){
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();
}

void createEntities(){
	game->main_menu = (MainMenu*) malloc(sizeof(MainMenu));
	game->barrel = (Barrel*) malloc(sizeof(Barrel));
	game->timer = (Timer*) malloc(sizeof(Timer));
	game->car = (Player*) malloc(sizeof(Player));
}

void loadBitmaps(){

	game->background = loadBitmap(getImgPath("road"));
	game->menu_background = loadBitmap(getImgPath("main-menu"));
	game->barrel->bitmap = loadBitmap(getImgPath("barrel"));
	game->car->bmpForward = loadBitmap(getImgPath("blue-car"));
	game->car->bmpTLeft = loadBitmap(getImgPath("blue-car-tl"));
	game->car->bmpTRight = loadBitmap(getImgPath("blue-car-tr"));

	game->car->width = game->car->bmpForward->bitmapInfoHeader.width;
	game->car->height = game->car->bmpForward->bitmapInfoHeader.height;

}

void deleteBitmaps(){
	deleteBitmap(game->background);
	deleteBitmap(game->menu_background);
	deleteBitmap(game->car->bmpForward);
	deleteBitmap(game->car->bmpTLeft);
	deleteBitmap(game->car->bmpTRight);
	deleteBitmap(game->barrel->bitmap);
}


void moveBackground(){

	MinixVice* game = getGame();

	game->backgroundY++;

	int backgroundHeight = game->background->bitmapInfoHeader.height;

    //FIXME: MOVING BACKGROUND WRONG
	if(game->backgroundY == vg_getVRES()/4){
		game->backgroundY = 0;
	}

}

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	subscribeInterrupts();
	createEntities();
	loadBitmaps();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	game->backgroundY = 0;


	/* MAIN MENU INIT */
	game->main_menu->playBtn = newColliderBox(95, 145, 392, 230);
	game->main_menu->quitBtn = newColliderBox(95, 257, 392, 340);

	/* ONE BARREL INIT */

	game->barrel->x = vg_getHRES() / 2;
	game->barrel->y = (vg_getVRES() / 2) - 200;
	game->barrel->width = game->barrel->bitmap->bitmapInfoHeader.width;
	game->barrel->height = game->barrel->bitmap->bitmapInfoHeader.height;

	game->timer->ticked = 0;
	game->timer->counter = 0;


	game->car->x = vg_getHRES() / 2;
	game->car->y = vg_getVRES() / 2;

	game->car->speed = PLAYER_SPEED;

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
	mouseIntHandler();

	Mouse* m = getMouse();
	MinixVice* game = getGame();

	if(m->RBtnDown)	game->done = 1;

//	if(m->LBtnDown){
//		printf("CLICKED AT COORDS X-Y: %d-%d\n",m->x, m->y);
//	}

}

//void mouseIH(){
//
//	Mouse* m = getMouse();
//
//	long byte = mouse_readOBF();
//
//	if (byte == -1)
//		return;
//
//	if (m->packet_index > 2) {
//		m->mouse_packets_synched = 0;
//	}
//
//	synch_packet(byte);
//
//	if (m->mouse_packets_synched) {
//		m->packet[m->packet_index] = byte;
//		m->packet_index++;
//	}
//}
//
//void synch_packet(long byte) {
//
//	Mouse* m = getMouse();
//
//	if (m->mouse_packets_synched)
//		return;
//
//	if (FIRSTBYTE & byte) {
//		m->packet_index = 0;
//		m->mouse_packets_synched = 1;
//	}
//}

void timerIH() {
	MinixVice* game = getGame();

	game->timer->counter++;
	game->timer->ticked = 1;
	moveBackground();

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
		default:
			updatePlayerState(DEFAULT);
			break;
		}

	}

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

	//early version commented for now
	//check_collisions();

	if (game->timer->ticked) {
		updateMouse();
		//drawMouse();
		//game->timer->ticked = 0;
		//flipDB();
	}

	if (m->LBtnDown) {

		if (clicked(game->main_menu->playBtn, m->x, m->y)) {
			//printf("CLICKED PLAY\n");
			updateGameState(PLAY);
		}

		if (clicked(game->main_menu->quitBtn, m->x, m->y)) {
			//printf("CLICKED QUIT\n");
			game->done = 1;
		}
	}

}

void drawMinixVice() {

	MinixVice* game = getGame();

	switch (gameState) {

	case MAIN_MENU:
		//TODO: draw menu
		drawBackgroundBitmap(game->menu_background, 0, 0, ALIGN_LEFT);
		break;

	case GAME:
		//FIXME: FIX MOVING BACKGROUND
		drawBackgroundBitmap(game->background, 0, game->backgroundY, ALIGN_LEFT);
		drawPlayer(game->car);
		drawBarrel(game->barrel);
		break;

	}

//	drawBackgroundBitmap(game->background, 0, 0, ALIGN_LEFT);
//	drawPlayer(game->car);
//	drawBarrel(game->barrel);

	//printf("BARREL SIZE: %d * %d\n", game->barrel->bitmap->bitmapInfoHeader.width , game->barrel->bitmap->bitmapInfoHeader.height);

	if (game->timer->ticked) {
		if (getMouse()->draw) {
			drawMouse();
		}
		game->timer->ticked = 0;
	}

	flipDB();
}

//first attempt at collisions
int check_collisions() {
	MinixVice* game = getGame();

	int playerX = game->car->x;
	int playerY = game->car->y;

	int xi = game->barrel->x;
	int xf = xi + game->barrel->width;

	int yi = game->barrel->y;
	int yf = yi + game->barrel->height;

	printf("PLAYER: %d - %d\n", playerX, playerY);
	printf("BARREL: %d - %d ; %d - %d\n", xi, yi, xf, yf);


	//TODO: AN ACTUAL COLLISION
	if (playerX > xf)
		game->done = 1;

	return 0;
}

void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	unsubscribeInterrupts();

	deleteBitmaps();
	deleteMouse();

	free(game->timer);
	free(game->car);
	free(game->barrel);
	free(game);

}

