#include "state_machines.h"

//TODO: set to MAIN_MENU
st_game gameState = MAIN_MENU;

st_player playerState = DEFAULT;

void updatePlayerState(st_player newState) {
	playerState = newState;
}

void updateGameState(game_event_t event) {

	switch (gameState) {

	case MAIN_MENU:

		if (event == SETTINGS)
			gameState = OPTIONS;

		if (event == TERMINATE)
			gameState = OVER;

		break;

	case OPTIONS:

		if (event == PLAY)
			gameState = GAME;

		break;

	}
}
