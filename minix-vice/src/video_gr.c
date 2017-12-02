#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>

#include "video_gr.h"
#include "read_xpm.h"
#include "vbe.h"
#include "read_xpm.h"
#include "lmlib.h"

/* Private global variables */

static char* video_mem; /* Process (virtual) address to which VRAM is mapped */
static char* double_buffer;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int vram_size;

unsigned vg_getHRES() {
	return h_res;
}

unsigned vg_getVRES() {
	return v_res;
}

direction_t getDirection(xi,yi,xf,yf){

	int dx = xf - xi;
	int dy = yf - yi;

	//if both dx and dy are 0 state will be STOP therefore no movement
	if(dx == 0 && dy == 0)	return STOP;

	//if both dx and dy are different that would be a diagonal movement and the code simply refuses to move xpm
	if(dx != 0 && dy != 0)  return STOP;

	if (dx == 0) {

		if (dy > 0)
			return DOWN;

		if (dy < 0)
			return UP;

	} else {

		if (dx > 0)
			return RIGHT;

		if (dx < 0)
			return LEFT;
	}

	return STOP;
}

void *vg_init(unsigned short mode) {

	vbe_mode_info_t vbe_mode;
	vbe_get_mode_info(mode, &vbe_mode);

	v_res = vbe_mode.YResolution;
	h_res = vbe_mode.XResolution;
	bits_per_pixel = vbe_mode.BitsPerPixel;
	vram_size = h_res * v_res * (bits_per_pixel / BITS_PER_BYTE);

	int r;
	struct mem_range mr;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vbe_mode.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	struct reg86u reg;
	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;

	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	double_buffer = (char*)malloc(sizeof(vram_size));

	return (void*) vbe_mode.PhysBasePtr;
}

int vg_exit() {

	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return OK;
}

void flipDB() {
	memcpy(video_mem, double_buffer, vram_size);
}


int paintPixel(unsigned short x, unsigned short y, unsigned long color) {

	/*
	 * Based on: www.phatcode.net/res/221/files/vbe20.pdf , page 79
	 */

	if (x >= h_res)
		return -1;

	if (y >= v_res)
		return -1;

	char *virtualvramptr = video_mem; //changing to double_buffer should be enough :/

	//virtualvramptr += x + h_res * y;

	virtualvramptr = (x + y * h_res * (bits_per_pixel/8));

	*virtualvramptr = color & 0x000000FF;
	*(virtualvramptr+1) = (color & 0x0000FF00) >> 8;
	*(virtualvramptr+2) = (color & 0x00FF0000) >> 16;

	return OK;

}

int drawLine(int x1, int y1, int x2, int y2, int color) {

	/*
	 * Digital Differential Analyzer (DDA) algorithm
	 * Based on: www.tutorialspoint.com/computer_graphics/line_generation_algorithm.html
	 * and on based onhttp://www.phatcode.net/res/221/files/vbe20.pdf , page 79
	 */

	int d; /*	Decision variable	*/
	int dx, dy; /*	Dx and Dy values for the line	*/
	int Eincr, NEincr; /*	Decision variable increments	*/
	int yincr; /*	Increment for y values	*/
	int t; /*	Counters etc.	*/

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if (dy <= dx) {
		/* We have a line with a slope between -1 and 1
		 *
		 Ensure that we are always scan converting the line from left to
		 right to ensure that we produce the same line from P1 to P0 as the
		 line from P0 to P1.
		 */
		if (x2 < x1) {
			t = x2;
			x2 = x1;
			x1 = t; /*Swap X coordinates*/
			t = y2;
			y2 = y1;
			y1 = t;/*Swap Y coordinates*/

		}
		if (y2 > y1)
			yincr = 1;
		else
			yincr = -1;

		d = 2 * dy - dx; /* Initial decision variable value	*/

		Eincr = 2 * dy; /* Increment to move to E pixel */
		NEincr = 2 * (dy - dx); /* Increment to move to NE pixel */
		paintPixel(x1, y1, color); /* Draw the first point at (x1,y1) */

		/* Incrementally determine the positions of the remaining pixels */
		for (x1++; x1 <= x2; x1++) {
			if (d < 0)
				d += Eincr; /* Choose the Eastern Pixel	*/
			else {
				d += NEincr; /*Choose the North	Eastern	Pixel*/
				y1 += yincr;/* or SE pixel for	dx/dy <	0!)	*/
			}

			paintPixel(x1, y1, color); /* Draw the point	*/
		}
	} else {

		/* We have a line with a slope between -1 and 1 (ie: includes
		 vertical lines). We must swap our x and y coordinates for this.
		 *
		 Ensure that we are always scan converting the line from left to
		 right to ensure that we produce the same line from P1 to P0 as the
		 line from P0 to P1.
		 */
		if (y2 < y1) {
			t = x2;
			x2 = x1;
			x1 = t; /*Swap X coordinates*/
			t = y2;
			y2 = y1;
			y1 = t; /*	Swap Y coordinates	*/
		}
		if (x2 > x1)
			yincr = 1;
		else
			yincr = -1;

		d = 2 * dx - dy; /* Initial decision variable value	*/
		Eincr = 2 * dx; /* Increment to move to E pixel */
		NEincr = 2 * (dx - dy); /* Increment to move to NE pixel */
		paintPixel(x1, y1, color); /* Draw the first point at (x1,y1) */

		/* Incrementally determine the positions of the remaining pixels */
		for (y1++; y1 <= y2; y1++) {

			if (d < 0)
				d += Eincr; /* Choose the Eastern Pixel	*/
			else {
				d += NEincr;/*Choose the North	Eastern	Pixel*/
				x1 += yincr;/* (or SE pixel for	dx/dy <	0!)*/
			}
			paintPixel(x1, y1, color);
		}
	}

	return OK;
}

int drawXPM(unsigned short xi, unsigned short yi, char *xpm[]) {

	int width, height;

	//read_xpm provided by professor
	char *sprite = read_xpm(xpm, &width, &height);

	unsigned int row, col;

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			if (*(sprite + row * width + col) != 0)
				paintPixel(xi + col, yi + row, *(sprite + row * width + col));
		}
	}

	free(sprite);

	return OK;
}

int eraseXPM(unsigned short xi, unsigned short yi, char *xpm[]) {

	int width, height;

	//read_xpm provided by professor
	char *sprite = read_xpm(xpm, &width, &height);

	unsigned int row, col;

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			if (*(sprite + row * width + col) != 0)
				paintPixel(xi + col, yi + row, 0);
		}
	}

	free(sprite);

	return OK;
}

int drawSquare(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	short xcoord = ceil(x - size / 2) + vg_getHRES() / 2;
	short ycoord = ceil(y - size / 2) + vg_getVRES() / 2;

	if (xcoord < 0)
		xcoord = 0;

	if (ycoord < 0)
		ycoord = 0;

	int col, row;
	for (col = 0; col < size; col++) {

		for (row = 0; row < size; row++) {
			paintPixel(xcoord + col, ycoord + row, color);
		}
	}

	return OK;
}
