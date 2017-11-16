#include "video_gr.h"
#include <unistd.h>
#include "test5.h"


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
	paintPixel(200,200,0x13);
	sleep(3);
	vg_exit();

	return 0;
}
	
int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	
	/* To be completed */
	
}	

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	/* To be completed */
	
}	

int test_controller() {

	/* To be completed */
	
}	
	
