#ifndef STATE_H
#define STATE_H

typedef enum {
	MAIN_MENU, GAME, OVER
} st_game;


typedef enum {
	TLEFT, TRIGHT, DEFAULT
} st_player;


typedef enum {
	PLAY, TERMINATE
} game_event_t;

/*
 * Because he can go from one state to any other
 * it's a simple set of the current state
*/
void updatePlayerState(st_player newState);

void updateGameState(game_event_t event);

#endif /* STATE_H */
