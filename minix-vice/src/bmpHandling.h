#ifndef BMPHAND_H
#define BMPHAND_H

#include "MinixVice.h"


/** @defgroup bmpHandling bmpHandling
 * @{
 * Functions for manipulating the bitmaps used in the game. Loading and deleting of said bitmaps.
 */

/* BITMAP LOADING */

/**
 * @brief Loads bitmaps
 */
void loadBitmaps();


/**
 * @brief Loads barrel entities bitmaps
 */
void loadBarrelsBitmaps();


/**
 * @brief Loads cone entities bitmaps
 */
void loadConesBitmaps();


/**
 * @brief Loads digits bitmaps
 */
void loadDigitBitmaps();


/**
 * @brief Loads car bitmaps according to the selected car
 *
 * @param selectedCar a number matching the selected car
 */
void loadCarBitmaps(int selectedCar);


/**
 * @brief Loads animations bitmaps
 */
void loadAnimations();


/**
 * @brief Loads shot animations bitmaps
 */
void loadShotAnimations();


/**
 * @brief Loads bonus animations bitmaps
 */
void loadBonusAnimations();


/**
 * @brief Loads main menu bitmaps
 */
void loadMainMenuBitmaps();


/**
 * @brief Loads select menu bitmaps
 */
void loadSelectMenuBitmaps();


/**
 * @brief Loads remaining needed bitmaps like screens and other util bmps.
 */
void loadOtherBitmaps();


/* BITMAP DELETING */

/**
 * @brief Deletes bitmaps
 */
void deleteBitmaps();


/**
 * @brief Deletes digit bitmaps
 */
void deleteDigitBitmaps();


/**
 * @brief Deletes barrel entities bitmaps
 */
void deleteBarrelsBitmaps();


/**
 * @brief Deletes cone entities bitmaps
 */
void deleteConesBitmaps();



#endif /* BMPHAND_H */
