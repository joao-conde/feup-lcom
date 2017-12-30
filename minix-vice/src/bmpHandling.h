#ifndef BMPHAND_H
#define BMPHAND_H

#include "MinixVice.h"


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
