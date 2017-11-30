#include "stb_image_wrap.h"

//load PNG image file
char* stb_loadPNG(int* width, int* height, char* path){
	stbi_load(width, height, path);
}

//free image
void stb_free(char* image){
	stbi_image_free(image);
}
