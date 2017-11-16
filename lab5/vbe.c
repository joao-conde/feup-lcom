#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14
#define ABS(a)    ((a) >= 0 ? (a) : -(a))

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	/*
	union REGS in, out;
	struct SREGS segs;
	char far;
	*modeInfo = (char far *)&ModeInfoBlock;
	if (mode < 0x100)
		return 0;

	in.x.ax = 0x4F01;
	in.x.cx = mode;
	in.x.di = FP_OFF(modeInfo);
	segs.es = FP_SEG(modeInfo);
	int86x(0x10, &in, &out, &segs);
	if (out.x.ax != 0x4F)
		return 0;
	if ((ModeInfoBlock.ModeAttributes & 0x1)
			&& ModeInfoBlock.MemoryModel == memPK
			&& ModeInfoBlock.BitsPerPixel == 8
			&& ModeInfoBlock.NumberOfPlanes == 1)
		return 1;
	return 0;*/

}
