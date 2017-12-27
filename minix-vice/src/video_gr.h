#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/* Constants for VBE */

#define BITS_PER_BYTE 	  8

enum direction {
	UP, DOWN, LEFT, RIGHT, STOP
};


/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);


/**
* @brief Copies the graphic memory from double buffer to VRAM, to display it on screen
*/
void flipDB();


/**
* @brief Returns screen horizontal resolution
*
* @return screen width
*/
unsigned vg_getHRES();

/**
* @brief Returns screen vertical resolution
*
* @return screen height
*/
unsigned vg_getVRES();

/**
* @brief Returns double buffer pointer
*
* @param pointer to double buffer
*/
char* getGraphicBuffer();

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
