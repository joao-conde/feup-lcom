#include "stb_image_wrap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//load PNG image file
unsigned char* stb_loadPNG(char* path, int* width, int* height){

	int* channels = (int*)malloc(sizeof(int));

	unsigned char* pngImg = stbi_load(path, width, height, channels, 3);

	if(pngImg == NULL){
		printf("Failure loading PNG: %s", path);
		return NULL;
	}

	return pngImg;
}

//free image
void stb_free(char* image){
	stbi_image_free(image);
}
