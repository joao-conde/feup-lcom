#ifndef UTILS_H
#define UTILS_H


#define singleBit(byte) (0x01 & byte)
#define swap(type,i,j) {type t = i; i = j; j = t;}


#define INITIAL_SPEED 2



int fileExists(const char* filename);

const char* getImgPath(const char* img);


#endif /* UTILS_H */
