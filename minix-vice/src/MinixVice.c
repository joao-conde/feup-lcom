#include <minix/drivers.h>

#include "MinixVice.h"



const int FPS = 60;

MinixVice* init(){
	MinixVice* game = (MinixVice*)malloc(sizeof(MinixVice));


	game->irq_set_kbd = kbd_subscribe_int();

	game->done = 0;
	game->draw = 1;
	game->scancode = 0;

	return game;
}




void update(MinixVice* game){

	int ipc_status, r = 0;
	message msg;


}

void draw(MinixVice* game);

void end(MinixVice* game);
