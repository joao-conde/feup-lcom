#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "MinixVice.h"


/**
 * @brief Displays the game score.
 */
void displayScore(int offsetX, int offsetY);


/**
 * @brief Displays date
 */
void displayDate();


/**
 * @brief Displays hour
 */
void displayHour();


/**
 * @brief Displays number of cones shot
 */
void displayConesShot();

/**
 * @brief Draws the player bitmap on the screen
 */
void drawPlayer();


/* BARREL METHODS */

/**
 * @brief Draws the barrel on the screen
 *
 * @param barrel a barrel pointer with the bitmap to be drawn
 */
void drawBarrel(Barrel* barrel);

/**
 * @brief Draws the cone on the screen
 *
 * @param cone a cone pointer with the bitmap to be drawn
 */
void drawCone(Cone* cone);

/**
 * @brief Draws the barrel entities
 */
void drawBarrels();


/**
 * @brief Draws the cone entities
 */
void drawCones();

#endif /* GRAPHICS_H */
