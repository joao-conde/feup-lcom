#ifndef UTILS_H
#define UTILS_H

#define SCORE_INCREASE(s) (0.5 + s*s/10000)

#define GRAPHIC_MODE 279

#define INITIAL_SPEED 3
#define TURN_SPEED 5
#define BRAKE_SPEED 1
#define ACCELERATE_SPEED 0.5

#define LEFT_ROAD_LIMIT 197
#define RIGHT_ROAD_LIMIT 823
#define ORIGIN_COORDS   0

#define SCORE_OFFSET 50
#define CAR_OFFSET 10

#define PLAYBTNX1 95
#define PLAYBTNY1 145
#define PLAYBTNX2 392
#define PLAYBTNY2 230

#define QUITBTNX1 95
#define QUITBTNY1 257
#define QUITBTNX2 392
#define QUITBTNY2 340


#define LAMBSELECTX1 156
#define LAMBSELECTY1 230
#define LAMBSELECTX2 319
#define LAMBSELECTY2 568

#define BLUESELECTX1 431
#define BLUESELECTY1 230
#define BLUESELECTX2 579
#define BLUESELECTY2 568

#define MERCEDESSELECTX1 705
#define MERCEDESSELECTY1 230
#define MERCEDESSELECTX2 860
#define MERCEDESSELECTY2 568






int fileExists(const char* filename);

const char* getImgPath(const char* img);

int generateRandomPos(int lowerBound, int higherBound);


#endif /* UTILS_H */
