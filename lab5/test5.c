#include "video_gr.h"
#include "test5.h"
#include "pixmap.h"
#include "vbe.h"

#include <math.h>
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


	short xcoord = ceil(x - size/2) + vg_getHRES()/2;
	short ycoord = ceil(y - size/2) + vg_getVRES()/2;

	if(xcoord < 0) xcoord = 0;

	if(ycoord < 0) ycoord = 0;

	int col, row;
	for (col = 0; col < size; col++) {

		for (row = 0; row < size; row++) {
			paintPixel(xcoord + col, ycoord + row, color);
		}
	}


	sleep(10);


	if(vg_exit() != 0)
		return -1;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	
	vg_init(VBE_MODE105);


	drawLine(xi, yi, xf, yf, color);
	sleep(5);

	vg_exit();

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

	printf("\nVBE INFO ADD: 0x%x\n",&vbe_info);
	vbe_get_info(VBE_MODE105, &vbe_info);

	printf("\nSTRUCTURE VBE INFO\n");

	printf("version: %x\n",vbe_info.VESAVersion);
	printf("\n pointer value: %d\n", vbe_info.VideoModePtr);
	printf("mode: %s", vbe_info.VideoModePtr);
	printf("total memory: %d\n", vbe_info.TotalMemory);

	return 0;
}	




























	
