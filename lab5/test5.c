#include "video_gr.h"
#include "test5.h"
#include "pixmap.h"
#include "vbe.h"

#include <unistd.h>


int video_test_init(unsigned short mode, unsigned short delay) {
	
	if(vg_init(mode) == NULL)
		return -1;

	sleep(delay);

	if(vg_exit() != 0)
		return -1;

	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	if(vg_init(VBE_MODE105) == NULL)
		return -1;

	int col, row;
	for (col = 0; col < size; col++) {

		for (row = 0; row < size; row++) {
			paintPixel(x + col, y + row, color);
		}
	}


	sleep(5);


	if(vg_exit() != 0)
		return -1;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	
	if(vg_init(VBE_MODE105) == NULL)
		return -1;

	drawLine(xi, yi, xf, yf, color);
	sleep(5);

	if(vg_exit() != 0)
		return -1;

	return 0;
}
	
int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	
	if(vg_init(VBE_MODE105) == NULL)
		return -1;

	draw_xpm(xi,yi, xpm);

	sleep(5);

	if(vg_exit() != 0)
		return -1;

	return 0;
}	

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	/* To be completed */
	return 0;
}	

int test_controller() {

	vbe_info_t vbe_info;
	vbe_get_info(VBE_MODE105, &vbe_info);

	printf("\nSTRUCTURE VBE INFO\n");

	printf("version: %d\n",vbe_info.VESAVersion);
	printf("mode: %s", vbe_info.VideoModePtr);
	printf("total memory: %d\n", vbe_info.TotalMemory);

	return 0;
}	




























	
