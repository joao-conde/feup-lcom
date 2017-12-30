#include "state_machines.h"

st_game gameState = MAIN_MENU;

st_player playerState = DEFAULT;

st_mouse mouseState = MENU;

void updateMouseState(st_mouse newState) {
	mouseState = newState;
}

void updatePlayerState(st_player newState) {
	playerState = newState;
}

void updateGameState(game_event_t event) {

	switch (gameState) {

	case MAIN_MENU:

		if (event == SELECT_CAR)
			gameState = SELECT_MENU;

		if (event == HELP)
			gameState = HELP_MENU;

		if (event == TERMINATE)
			gameState = OVER;

		break;

	case HELP_MENU:

		if(event == MAIN)
			gameState = MAIN_MENU;

		break;

	case SELECT_MENU:

		if (event == PLAY)
			gameState = GAME;

		break;

	case GAME:

		if (event == TERMINATE)
			gameState = STATS_MENU;

		break;

	case STATS_MENU:

		if(event == TERMINATE)
			gameState = OVER;

		if(event == PLAY)
			gameState = MAIN_MENU;

		break;

	}
}

