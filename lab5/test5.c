#include "video_gr.h"
#include "video_test.h"
#include "test5.h"
#include "pixmap.h"
#include "vbe.h"
#include "kbd.h"
#include "timer.h"
#include "i8254.h"
#include "vbe.h"
#include "lmlib.h"


#include <unistd.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdlib.h>
#include <machine/int86.h>


int video_test_init(unsigned short mode, unsigned short delay) {

	if (vg_init(mode) == NULL)
		return -1;

	sleep(delay);

	if (vg_exit() != OK)
		return -1;

	return OK;
}

int video_test_square(unsigned short x, unsigned short y, unsigned short size,	unsigned long color) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawSquare(x, y, size, color);
	video_dump_fb();
	waitEscapeKey();

	if (vg_exit() != OK)
		return -1;

	return OK;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawLine(xi, yi, xf, yf, color);
	video_dump_fb();
	waitEscapeKey();

	if (vg_exit() != OK)
		return -1;

	return OK;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawXPM(xi, yi, xpm);
	video_dump_fb();
	waitEscapeKey();

	if (vg_exit() != OK)
		return -1;

	return OK;
}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short s, unsigned short f) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	move_xpm(xpm,xi,yi,xf,yf,s,f);

	if (vg_exit() != OK)
		return -1;

	return OK;
}

int test_controller() {

	vbe_info_t vbe_info;

	vbe_get_info(&vbe_info);

	short vbe_version = (vbe_info.VBEVersion >> 8) & 0xFF;
	short vbe_subversion = (vbe_info.VBEVersion << 8) & 0xFF;

	printf("%x.%x\n", vbe_version, vbe_subversion);

	/*
		Originally we tried this solution but the cicle didnt stop where intended, so it read the modes plus garbage.
		This reading uses the VideoModePtr, sums the virtual memory pointer with the linear size of the physical adress,
		by calculating the base and offset of VideoModePtr, a 16-bit pointer.



	 NOTE: vmem was the return of my vbe_info function, a void* to the virtual memory allocated by lm_init, late removed
	 due to no need

	 short* modeListPtr = (short*)vmem + PB2BASE(*(int*)vbe_info.VideoModePtr) + PB2OFF(*(int*)vbe_info.VideoModePtr);

	 while(*modeListPtr != 0xffff){
	 	 printf("0x%x\n",*modeListPtr);
	 	 modeListPtr++;
	 }

	 */


	/*
	 * This solution reads the reserved array of VBE Info Block as also specified in the VBE pdf
	 */
	char* modePtr = vbe_info.reserved;

	while (*modePtr != -1) {
		short mode = *modePtr;
		modePtr++;
		mode += (*modePtr) << 8;
		printf("0x%x:", mode);
		modePtr++;
	}

	printf("\n%d\n", vbe_info.TotalMemory * 64); //each block has 64kb this multiplying total amount of blocks per 64 gives us total memory in kb

	return OK;
}
