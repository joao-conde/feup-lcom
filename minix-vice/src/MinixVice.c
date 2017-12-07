#include <minix/drivers.h>

#include "MinixVice.h"

const int FPS = 60;

/* SINGLETON GAME IMPLEMENTATION */
MinixVice* game = NULL;

MinixVice* initMinixVice() {

	/* GAME INITIALIZATION */
	game = (MinixVice*) malloc(sizeof(MinixVice));

	game->irq_kbd = kbd_subscribe_int();
	game->irq_timer = timer_subscribe_int();
	game->irq_mouse = mouse_subscribe_int();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;
	game->background = loadBitmap("/home/minix-vice/res/images/road.bmp");


	/* ONE BARREL INIT */
	Barrel* barrel = (Barrel*) malloc(sizeof(Barrel));
	barrel->x = vg_getHRES() / 2;
	barrel->y = (vg_getVRES() / 2) - 200;
	barrel->bitmap = loadBitmap("/home/minix-vice/res/images/barrel.bmp");
	barrel->width = barrel->bitmap->bitmapInfoHeader.width;
	barrel->height = barrel->bitmap->bitmapInfoHeader.height;

	game->barrel = barrel;

	/* TIMER INITIALIZATION */
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->ticked = 0;
	timer->counter = 0;
	game->timer = timer;


	/* PLAYER INITIALIZATION */
	Player* player = (Player*) malloc(sizeof(Player));
	player->x = vg_getHRES() / 2;
	player->y = vg_getVRES() / 2;

//	player->y = 0;
//	player->x = 0;

	player->speed = PLAYER_SPEED;
	player->bitmap = loadBitmap("/home/minix-vice/res/images/blue-car.bmp");
	player->width = player->bitmap->bitmapInfoHeader.width;
	player->height = player->bitmap->bitmapInfoHeader.height;


	game->car = player;

	return game;
}

MinixVice* getGame() {

	if (!game) {
		game = initMinixVice();
	}

	return game;
}

//TODO: change this to work with mouse object
void mouseIH(){
	mouseIntHandler();
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
			break;
		case D_MAKE:
			movePlayerRight(game->car);
			break;
		}
	}

}

void updateMinixVice() {

	MinixVice* game = getGame();

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
}

void drawMinixVice() {

	MinixVice* game = getGame();

	drawBackgroundBitmap(game->background, 0, 0, ALIGN_LEFT);
	drawPlayer(game->car);
	drawBarrel(game->barrel);

	//printf("BARREL SIZE: %d * %d\n", game->barrel->bitmap->bitmapInfoHeader.width , game->barrel->bitmap->bitmapInfoHeader.height);

	if (game->timer->ticked) {
		if(getMouse()->draw){
			drawMouse();
		}
		game->timer->ticked = 0;
	}

	flipDB();
}


//first attempt at collisions
int check_collisions(){
	MinixVice* game = getGame();

	int playerX = game->car->x;
	int playerY = game->car->y;

	int xi = game->barrel->x;
	int xf = xi + game->barrel->width;

	int yi = game->barrel->y;
	int yf = yi + game->barrel->height;


	printf("PLAYER: %d - %d\n",playerX, playerY);
	printf("BARREL: %d - %d ; %d - %d\n",xi, yi, xf, yf);

	if(playerX > xf)
		game->done = 1;

	return 0;
}

void endMinixVice() {
	//TODO: FREE ALL GAME ENTITIES AND BITMAPS
	MinixVice* game = getGame();

	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();

	deleteMouse();
	free(game->timer);
	free(game->car);
	free(game);
}

