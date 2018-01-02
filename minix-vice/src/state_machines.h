#ifndef STATE_H
#define STATE_H


/** @defgroup state_machines state_machines
 * @{
 *
 * Implementation of game, mouse and player states and states update
 */

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
 * Mouse and player can go from any of their states to another one, therefore no event required, just a change of state.
 */

/**
 * @brief Changes player state to a new one
 *
 * @param newState the new player state
 */
void updatePlayerState(st_player newState);


/**
 * @brief Changes mouse state to a new one
 *
 * @param newState the new mouse state
 */
void updateMouseState(st_mouse newState);


/**
 * @brief Changes game state to a new one
 *
 * @param event the game event, to be analyzed making a gameState update
 */
void updateGameState(game_event_t event);



#endif /* STATE_H */
