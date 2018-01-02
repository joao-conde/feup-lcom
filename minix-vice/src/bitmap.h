#ifndef BITMAP_H
#define BITMAP_H


#define TRANSPARENCY 0xFFFFFFF8

/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps - export images as a 16-bit 5:6:5 .bmp image.
 * Changed original module developed by Henrique Ferrolho (https://github.com/ferrolho) to fit the game needs.
 */

 /// Holds bitmap file information
typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;


/// Holds bitmap image information
typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image.
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position. Checks if pixel is transparent or not.
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 */
void drawBitmap(Bitmap* bitmap, int x, int y);


/**
 * @brief Draws an unscaled, unrotated bitmap at the given position. Does not check for transparency (more efficient).
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 */
void drawBackgroundBitmap(Bitmap* bmp, int x, int y);


/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bitmap bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);


#endif /* BITMAP_H */
