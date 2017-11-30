#ifndef STB_IMAGE_WRAP_H
#define STB_IMAGE_WRAP_H


//load PNG image file
unsigned char* stb_loadPNG(char* path, int* width, int* height);

//free image
void stb_free(char* image);

#endif //STB_IMAGE_WRAP_H
