#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"



/* Private global variables */

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode) {

	vbe_mode_info_t vbe_mode;

	v_res = V_RES;
	h_res = H_RES;
	bits_per_pixel = BITS_PER_PIXEL;

	unsigned int vram_size; /* VRAM's size, but you can use
	 the frame-buffer size, instead */

	struct reg86u reg;
	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	vram_size = h_res * v_res * (bits_per_pixel / 8);

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

	return (void*) VRAM_PHYS_ADDR;

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
		return 0;
}


int paintPixel(unsigned short x, unsigned short y, unsigned long color) {

	/*if ((x < 0 || x >= h_res) || (y < 0 || y >= v_res)) // input check
		return -1;
	*/
	char *ptr = video_mem;

	//contas
	ptr += (x + h_res * y) * (bits_per_pixel / 8);

	*ptr = color;
	return 0;

}
