#include "utils.h"

#include <stdio.h>


int fileExists(const char* filename){
	FILE* file = fopen(filename,"r");

	if(file){
		fclose(file);
		return 1;
	}

	return 0;
}


const char* getImgPath(const char* img){

	char num[256];
	sprintf(num, "/home/minix-vice/res/images/%s.bmp",img);

	char* str = (char*) malloc(256);
	strcpy(str,num);

	return str;
}

int generateRandomPos(int lowerBound, int higherBound) {
	return rand() % (higherBound - lowerBound) + lowerBound;
}
