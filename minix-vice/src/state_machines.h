#ifndef STATE_H
#define STATE_H

typedef enum {
	MAIN_MENU, GAME
} st_game;


typedef enum {
	TLEFT, TRIGHT, DEFAULT
} st_player;


typedef enum{
	KAPA
} event_t_game;


/*
 * Because he can go from one state to any other
 * it's a simple set of the current state
*/
void updatePlayerState(st_player newState);

void updateGameState();

#endif /* STATE_H */
