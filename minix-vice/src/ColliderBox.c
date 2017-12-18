#include "ColliderBox.h"

ColliderBox* newColliderBox(int x1, int y1, int x2, int y2) {

	ColliderBox* collider = (ColliderBox*) malloc(sizeof(ColliderBox));

	collider->x1 = x1;
	collider->y1 = y1;
	collider->x2 = x2;
	collider->y2 = y2;

	return collider;
}

int clicked(ColliderBox* collider, Mouse* mouse) {

	int x = mouse->x;
	int y = mouse->y;

	if (!mouse->LBtnDown)
		return 0;

	if (!(x >= collider->x1 && x <= collider->x2))
		return 0;

	if (!(y >= collider->y1 && y <= collider->y2))
		return 0;

	return 1;
}

int collide(ColliderBox* c1, ColliderBox* c2) {

	/*
	 * For a ColliderBox A and a ColliderBox B - any one of four conditions guarantees
	 * that NO OVERLAP(collide) can exist:
	 * 1 - A's left edge is to the right of the B's right edge - A to the right Of B
	 * 2 - A's right edge is to the left of the B's left edge - A to the left Of B
	 * 3 - A's top edge is below B's bottom edge - A below B
	 * 4 - A's bottom edge is above B's top edge - A above B
	 */

	//check condition 1
	if(c1->x1 > c2->x2)
		return 0;

	//check condition 2
	if(c1->x2 < c2->x1)
		return 0;

	//check condition 3
	if(c1->y1 > c2->y2)
		return 0;

	//check condition 4
	if(c1->y2 < c2->y1)
		return 0;

	return 1;
}

void deleteColliderBox(ColliderBox* collider) {
	free(collider);
}
