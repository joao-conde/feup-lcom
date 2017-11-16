#include "video_gr.h"
#include "test5.h"

#include <unistd.h>


int video_test_init(unsigned short mode, unsigned short delay) {
	
	vg_init(mode);
	sleep(delay);
	vg_exit();

	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	vg_init(0x105);
	sleep(3);
	vg_exit();

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	
	vg_init(0x105);
	drawLine(0, 0, 400, 200, 0x10);
	sleep(3);
	vg_exit();

	return 0;
}
	
int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	
	/* To be completed */
	return 0;
}	

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	/* To be completed */
	return 0;
}	

int test_controller() {

	/* To be completed */
	return 0;
}	
	
