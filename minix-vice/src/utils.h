#ifndef UTILS_H
#define UTILS_H


/** @defgroup utils utils
 * @{
 *
 * Constants for MinixVice and basic auxiliary functions.
 */

#define SCORE_INCREASE(s) (0.1 + (s*s)/10000)
#define CONVERT2DECIMAL(x) ((x ^= 0xFF) + 1)


#define GRAPHIC_MODE 0x0117 /* 16-bit (RGB 5:6:5) 1024*768 */

#define NUMBER_OF_BARRELS 4
#define NUMBER_OF_CONES   3
#define MAX_CONESHOT_ANIM 3
#define UTILS_LENGTH      2
#define DECIMAL_ALG       10

#define RANDOM_LOWERB 50
#define RANDOM_UPPERB 600

#define BONUSANIM_OFFSETX 30
#define BONUSANIM_OFFSETY 30

#define SPRITESHEET_SHOT_SIZE 16
#define BONUS_FRAMES_DISPLAY 55

#define INITIAL_SPEED 4
#define TURN_SPEED 5
#define BRAKE_SPEED 1
#define ACCELERATE_SPEED 0.5
#define MIN_SPEED  2

#define CONESHOT_BONUS     10

#define LEFT_ROAD_LIMIT 197
#define RIGHT_ROAD_LIMIT 823
#define ORIGIN_COORDS   0

#define CHAR_DISTANCE 50

#define SCOREX_DISPLAY 840
#define SCOREY_DISPLAY 10
#define SCOREX_OFFSET  70
#define SCOREY_OFFSET  700

#define FINALSCOREX_OFFSET 440
#define FINALSCOREY_OFFSET 310

#define DATEX_OFFSET  200
#define DATEY_OFFSET  -150

#define HOURX_OFFSET  160
#define HOURY_OFFSET  -50

#define CONESX_OFFSET 440
#define CONESY_OFFSET 225

#define CAR_OFFSET    10
#define MOUSE_MARGIN  6
#define COLBOX_MARGIN 5

#define PLAYBTNX1 95
#define PLAYBTNY1 145
#define PLAYBTNX2 392
#define PLAYBTNY2 230

#define QUITBTNX1 95
#define QUITBTNY1 257
#define QUITBTNX2 392
#define QUITBTNY2 340

#define SELECTED_RED      1
#define SELECTED_LAMB     2
#define SELECTED_MERCEDES 3

#define LAMBSELECTX1 156
#define LAMBSELECTY1 230
#define LAMBSELECTX2 319
#define LAMBSELECTY2 568

#define REDSELECTX1 431
#define REDSELECTY1 230
#define REDSELECTX2 579
#define REDSELECTY2 568

#define MERCEDESSELECTX1 705
#define MERCEDESSELECTY1 230
#define MERCEDESSELECTX2 860
#define MERCEDESSELECTY2 568

/* OTHERS */
#define BYTE_MINUS1	    0xFF


/**
 * @brief Checks if a file exists or not.
 *
 * @param filename a "C string" (char pointer) with filename
 *
 * @return 1 if exists 0 otherwise
 */
int fileExists(const char* filename);


/**
 * @brief Returns the image path.
 *
 * @param img image name to create the path
 *
 * @return pointer to "C string" with the full image path
 */
const char* getImgPath(const char* img);


/**
 * @brief Returns a random number between 2 limits.
 *
 * @param lowerBound lower limit of the number generated
 * @param higherBound upper limit of the number generated
 *
 * @return a random number between lower bound and higher bound
 */
int generateRandomPos(int lowerBound, int higherBound);


#endif /* UTILS_H */
