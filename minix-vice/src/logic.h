#ifndef LOGIC_H
#define LOGIC_H

#include "MinixVice.h"

/** @defgroup logic logic
 * @{
 *
 * Input/Output basic functions. Interrupt handling for mouse, keyboard and timer and polling for Real-Time Clock (RTC).
 */

/**
 * @brief Updates game. Reads RTC, calls IH's and events handler.
 */
void updateMinixVice();

/**
 * @brief Handles game events. Calls a different handler for each game state.
 */
void handleEvents();

/**
 * @brief Handles main menu events. Checks button clicks or keyboard 'help' shortcut ('H').
 */
void handleMainMenuEvents();

/**
 * @brief Handles help menu events. Checks keyboard 'ESC' shortcut to return to min menu.
 */
void handleHelpMenuEvents();

/**
 * @brief Handles select menu events. Checks car selection clicks.
 */
void handleSelectMenuEvents();

/**
 * @brief Handles game screen events. Updates game entities, animations and checks for collisions or shots.
 */
void handleGameScreenEvents();

/**
 * @brief Handles stats(gameover) menu events. Forces display for an amount of seconds.
 * After that any keyboard key returns to main menu.
 */
void handleStatsMenuEvents();

/**
 * @brief Checks for game collisions between player and other entities.
 */
void checkCollisions();

/**
 * @brief Checks for collisions between player and barrels.
 */
void checkBarrelsCollisions();

/**
 * @brief Checks for collisions between player and cones.
 */
void checkConesCollisions();

/**
 * @brief De-accelerates the car.
 */
void brake();

/**
 * @brief Accelerates the car.
 */
void accelerate();

/**
 * @brief Calculates the game score.
 */
void calculateScore();

/**
 * @brief Recalculates a barrel position, used when it exits game screen.
 *
 * @param barrel a barrel pointer to the barrel to recalculate the position
 */
void recalculateBarrelPos(Barrel* barrel);

/**
 * @brief Recalculates a cone position, used when it exits game screen.
 *
 * @param cone a cone pointer to the cone to recalculate the position
 */
void recalculateConePos(Cone* cone);

/**
 * @brief Recalculates barrels positions, used when they exit game screen.
 */
void recalculateBarrelsPos();

/**
 * @brief Recalculates cones positions, used when they exit game screen.
 */
void recalculateConesPos();

/**
 * @brief Updates barrel entities positions.
 */
void updateBarrelsPos();

/**
 * @brief Updates cone entities positions.
 */
void updateConesPos();

/**
 * @brief Begins a cone shot animation, with explosion and bonus points.
 *
 * @param x animation X position
 * @param y animation Y position
 */
void startConeShotAnimations(int x, int y);

/**
 * @brief Begins a shot animation, with explosion.
 *
 * @param x animation X position
 * @param y animation Y position
 */
void startShotAnimation(int x, int y);

/**
 * @brief Begins a bonus animation, with extra score gain.
 *
 * @param x animation X position
 * @param y animation Y position
 */
void startBonusAnimation(int x, int y);

/**
 * @brief Updates all animations in progress. Terminates those who are over.
 */
void updateAnimations();

/**
 * @brief Updates shot animations in progress. Terminates those who are over.
 */
void updateShotAnimations();

/**
 * @brief Updates bonus animations in progress. Terminates those who are over.
 */
void updateBonusAnimations();

/**
 * @brief Updates bonus animations in progress. Terminates those who are over.
 *
 * @param seconds seconds where user is forced to see gameover screen
 */
void forceDisplay(int seconds);

#endif /* LOGIC_H */
