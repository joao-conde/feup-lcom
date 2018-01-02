#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "MinixVice.h"

/** @defgroup graphics graphics
 * @{
 *
 * Draw functions for the game entities and others.
 */


/**
 * @brief Draws the game.
 */
void drawMinixVice();


/**
 * @brief Draws the main menu.
 */
void drawMainMenu();


/**
 * @brief Draws the select car menu.
 */
void drawSelectMenu();


/**
 * @brief Draws the main game screen.
 */
void drawGameScreen();


/**
 * @brief Draws the stats(game over) menu.
 */
void drawStatsMenu();


/**
 * @brief Draws the animations.
 */
void drawAnimations();


/**
 * @brief Draws the bonus animations.
 */
void drawBonusAnimations();


/**
 * @brief Draws the shot animations.
 */
void drawShotAnimations();


/**
 * @brief Draws the player.
 */
void drawPlayer();


/**
 * @brief Draws a barrel.
 *
 * @param barrel a barrel pointer with the barrel to be drawn
 */
void drawBarrel(Barrel* barrel);


/**
 * @brief Draws a cone.
 *
 * @param cone a cone pointer with the cone to be drawn
 */
void drawCone(Cone* cone);


/**
 * @brief Draws the barrel entities.
 */
void drawBarrels();


/**
 * @brief Draws the cone entities.
 */
void drawCones();


/**
 * @brief Draws information on the screen with and offset from a starting position.
 *  If the element to be displayed is less than 10 then a '0' is added.
 *  Also separates each component with a divisor bitmap.
 *
 *  @param info  the number to be drawn
 *  @param startX X starting position on the screen
 *  @param startY Y starting position on the screen
 *  @param offsetX offset counter, how many times to move
 *  @param div  divisor bitmap placed at the 'end' (starts drawing from the back)
 *  @param lastElem boolean value indicating if the element is the last or not, if 1 divisor is not drawn
 *
 *  @return
 */
int drawInfo(int info, int startX, int startY, int offsetX, Bitmap* div, int lastElem);


/**
 * @brief Displays the game score.
 *
 * @param offsetX X offset from the end of the screen
 * @param offsetY Y offset from the end of the screen
 */
void displayScore(int offsetX, int offsetY);


/**
 * @brief Displays the day, month and year.
 */
void displayDate();


/**
 * @brief Displays hour, minutes and seconds.
 */
void displayHour();


/**
 * @brief Displays number of cones shot.
 */
void displayConesShot();


/**
 * @brief Draws a moving background giving the sense of movement to the game.
 */
void drawMovingBackground();


#endif /* GRAPHICS_H */
