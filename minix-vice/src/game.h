#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <time.h>

#include "video_gr.h"
#include "MinixVice.h"


/** @defgroup game game
 * @{
 *
 * Program execution functions
 */


/**
 * @brief Program entry point, calls the methods to begin game, loops in it, exits it
 */
void play();


/**
 * @brief Initializes the game
 *
 * @return a pointer to the game created
 */
MinixVice* startGame();


/**
 * @brief Loops through the game
 *
 * @param game a pointer to the game created
 */
void gameLoop(MinixVice* game);


/**
 * @brief Exits the game
 *
 * @param game a pointer to the game created
 */
void endGame(MinixVice* game);


#endif /* GAME_H */
