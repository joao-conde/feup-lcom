#ifndef UTILS_H
#define UTILS_H


#define singleBit(byte) (0x01 & byte)
#define swap(type,i,j) {type t = i; i = j; j = t;}


#define PLAYER_SPEED 10


int fileExists(const char* filename);


#endif /* UTILS_H */
