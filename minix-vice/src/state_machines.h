#ifndef STATE_H
#define STATE_H

typedef enum {
	MAIN_MENU, SELECT_MENU, HELP_MENU, GAME, STATS_MENU, OVER
} st_game;


typedef enum {
	PLAY, MAIN, HELP, SELECT_CAR, TERMINATE
} game_event_t;



typedef enum {
	TLEFT, TRIGHT, DEFAULT
} st_player;


typedef enum {
	MENU, TARGET
} st_mouse;


/*
 * Because he can go from one state to any other
 * it's a simple set of the current state (mouse and player)
*/
void updatePlayerState(st_player newState);

void updateMouseState(st_mouse newState);

void updateGameState(game_event_t event);

#endif /* STATE_H */
