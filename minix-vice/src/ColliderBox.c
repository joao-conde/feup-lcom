#include "ColliderBox.h"


ColliderBox* newColliderBox(int x1, int y1, int x2, int y2){

	ColliderBox* collider = (ColliderBox*) malloc(sizeof(ColliderBox));

	collider->x1 = x1;
	collider->y1 = y1;
	collider->x2 = x2;
	collider->y2 = y2;

	return collider;
}


int clicked(ColliderBox* collider, int x, int y){

	if(!(x >= collider->x1 && x <= collider->x2))
		return 0;

	if(!(y >= collider->y1 && y <= collider->y2))
		return 0;

	return 1;
}


void deleteColliderBox(ColliderBox* collider){
	free(collider);
}