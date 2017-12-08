#ifndef COLLIDERBOX_H
#define COLLIDERBOX_H


typedef struct{
	int x1,y1,x2,y2;
} ColliderBox;


ColliderBox* newColliderBox(int x1, int y1, int x2, int y2);

//1 if clicked 0 if not
int clicked(ColliderBox* collider, int x, int y);

void deleteColliderBox(ColliderBox* collider);


#endif /* COLLIDERBOX_H */
