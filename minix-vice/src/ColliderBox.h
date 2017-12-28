#ifndef COLLIDERBOX_H
#define COLLIDERBOX_H

#include "mouse.h"

/** @defgroup ColliderBox ColliderBox
 * @{
 *
 * Basic functions to operate with collider boxes of game entities
 */

typedef struct{
	int x1,y1,x2,y2;
} ColliderBox;


/**
 * @brief Creates a ColliderBox struct with top left corner at (x1,y1)
 * and bottom right corner at (x2,y2)
 *
 * @param x1 top left corner X
 * @param y1 top left corner Y
 * @param x2 right bottom corner X
 * @param y2 right bottom corner Y
 *
 * @return a ColliderBox pointer
 */
ColliderBox* newColliderBox(int x1, int y1, int x2, int y2);


/**
 * @brief Checks if a ColliderBox was "clicked" which means a mouse
 * click occurred in the box area
 *
 * @param collider a ColliderBox pointer
 * @param mouse a Mouse pointer (contains mouse information)
 *
 * @return 1 if clicked, 0 if not
 */
int clicked(ColliderBox* collider, Mouse* mouse);

/**
 * @brief Checks if a ColliderBox is being mouse 'hovered' (mouse on it)
 *
 * @param collider a ColliderBox pointer
 * @param mouse a Mouse pointer (contains mouse information)
 *
 * @return 1 if hovering, 0 if not
 */
int hovered(ColliderBox* collider, Mouse* mouse);


/**
 * @brief Checks if a ColliderBox collided with another (overlap)
 *
 * @param c1 first ColliderBox pointer
 * @param c2 second ColliderBox pointer
 *
 * @return 1 if there is collision, 0 if not
 */
int collide(ColliderBox* c1, ColliderBox* c2);


/**
 * @brief Deletes the ColliderBox
 *
 * @param collider ColliderBox pointer to delete
 * @param c2 second ColliderBox pointer
 */
void deleteColliderBox(ColliderBox* collider);


#endif /* COLLIDERBOX_H */
