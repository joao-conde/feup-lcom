#ifndef ENTITIES_H
#define ENTITIES_H


typedef struct{

	int x,y;

}Player;


typedef struct{

	int x,y;

}Oil;


typedef struct{

	int x,y;
	int extraPts;

}ExtraPtsBoost;



void movePlayerLeft(Player* car);
void movePlayerRight(Player* car);


#endif /* ENTITIES_H */
