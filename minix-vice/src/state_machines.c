#include "state_machines.h"

st_game gameState = MAIN_MENU;

st_player playerState = DEFAULT;

void updatePlayerState(st_player newState) {
	playerState = newState;
}
