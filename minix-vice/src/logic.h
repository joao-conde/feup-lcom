#ifndef LOGIC_H
#define LOGIC_H

#include "MinixVice.h"


/**
 * @brief Updates game. Calls IH and events handler
 */
void updateMinixVice();


void handleEvents();

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
 * @brief Recalculates a barrel position
 *
 * @param barrel a barrel pointer to the barrel to recalculate the position
 */
void recalculateBarrelPos(Barrel* barrel);


/**
 * @brief Recalculates a cone position
 *
 * @param cone a cone pointer to the cone to recalculate the position
 */
void recalculateConePos(Cone* cone);



void recalculateBarrelsPos();
void recalculateConesPos();


/**
 * @brief Updates barrel entities positions
 */
void updateBarrelsPos();


/**
 * @brief Updates cone entities positions
 */
void updateConesPos();

void startConeShotAnimations(int x, int y);

void startShotAnimation(int x, int y);

void startBonusAnimation(int x, int y);

void updateAnimations();

void updateShotAnimations();

void updateBonusAnimations();


#endif /* LOGIC_H */
