#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>

#include "video_gr.h"
#include "vbe.h"
#include "lmlib.h"

/* Private global variables */

static char* video_mem; /* Process (virtual) address to which VRAM is mapped */
static char* double_buffer;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int vram_size;

char* getGraphicBuffer(){
	return double_buffer;
}

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

	double_buffer = (char*)malloc(vram_size);

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

