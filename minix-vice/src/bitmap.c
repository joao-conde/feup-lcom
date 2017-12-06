#include "bitmap.h"

#include "stdio.h"
#include "video_gr.h"
#include "utils.h"

Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
	if (bmp == NULL)
		return;

	unsigned horizontalRes = vg_getHRES();
	unsigned verticalRes = vg_getVRES();

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > horizontalRes || y + height < 0 || y > verticalRes)
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > horizontalRes)
			drawWidth = horizontalRes;
	} else if (x + drawWidth >= horizontalRes) {
		drawWidth = horizontalRes - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= verticalRes)
			continue;

		bufferStartPos = getGraphicBuffer();
		bufferStartPos += x * 2 + pos * horizontalRes * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

		//paint everything except pink (set as background of sprites);
		int j;
		for (j = 0; j < drawWidth * 2; j = j + 2) {

			if (imgStartPos[j + 1] != 0xFFFFFFF8) {
				bufferStartPos[j] = imgStartPos[j];
				bufferStartPos[j + 1] = imgStartPos[j + 1];
			}

		}
		//memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}


void drawBackgroundBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
	if (bmp == NULL)
		return;

	unsigned horizontalRes = vg_getHRES();
	unsigned verticalRes = vg_getVRES();

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > horizontalRes || y + height < 0 || y > verticalRes)
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > horizontalRes)
			drawWidth = horizontalRes;
	} else if (x + drawWidth >= horizontalRes) {
		drawWidth = horizontalRes - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= verticalRes)
			continue;

		bufferStartPos = getGraphicBuffer();
		bufferStartPos += x * 2 + pos * horizontalRes * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

		memcpy(bufferStartPos, 0, drawWidth * 2); //TODO: CHANGE 0 TO IMGSTARTPOS (current pixel color)
	}
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}