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

enum direction {
	VERTICAL, HORIZONTAL
};

typedef enum direction direction_t;

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int video_test_init(unsigned short mode, unsigned short delay) {

	if (vg_init(mode) == NULL)
		return -1;

	sleep(delay);

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int video_test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawSquare(x, y, size, color);
	video_dump_fb();
	waitEscapeKey();

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawLine(xi, yi, xf, yf, color);
	waitEscapeKey();

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawXPM(xi, yi, xpm);
	waitEscapeKey();

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short s, unsigned short f) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	direction_t direction;

	//vertical move (0 movement in X)
	if ((xf - xi) == 0)
		direction = VERTICAL;

	//horizontal move (0 movement in Y)
	if ((yf - yi) == 0)
		direction = HORIZONTAL;

	int ipc_status, r, timer_irq_set, kbd_irq_set, int_counter = 0;
	message msg;

	if ((timer_irq_set = timer_subscribe_int()) == -1)
		return -1;

	if ((kbd_irq_set = kbd_subscribe_int()) == -1)
		return -1;

	float incX = xi;
	float incY = yi;
	float speed;

	unsigned long scancode = 0;

	if (s < 0)
		speed = 1.0 / (abs(s));
	else
		speed = s;

	float pixelsInc = (speed * f) / TIMER_DEF_FREQ_DOUBLE;

	while ((incX != xf || incY != yf) && scancode != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & timer_irq_set) {

					int_counter++;

					switch (direction) {

					case VERTICAL:
						eraseXPM(xi, (int) incY, xpm);
						incY += pixelsInc;
						drawXPM(xi, (int) incY, xpm);
						break;

					case HORIZONTAL:
						eraseXPM(xi, (int) incX, xpm);
						incX += pixelsInc;
						drawXPM(xi, (int) incX, xpm);
						break;
					}

				}

				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					scancode = kbc_read();
					printf("READ SCANCODE: 0x%x\n");
				}

				break;

			default:
				break;
			}
		}
	}

	if (timer_unsubscribe_int() != OK)
		return -1;

	if (kbd_unsubscribe_int() != OK)
		return -1;

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int test_controller() {

	vbe_info_t vbe_info;

	printf("\nVBE INFO ADD: 0x%x\n", &vbe_info);
	vbe_get_info(VBE_MODE105, &vbe_info);

	printf("\nSTRUCTURE VBE INFO\n");

	printf("version: %x\n", vbe_info.VESAVersion);

	printf("total memory: %d\n", vbe_info.TotalMemory);

	mmap_t m;
	struct reg86u r;
	char* modes;

	lm_init();
	lm_alloc(256, &m);

	r.u.w.es = PB2BASE(m.phys);
	r.u.w.di = PB2OFF(m.phys);

	memcpy(modes, m.virtual, 256);

	lm_free(&m);

	printf("Modes: %s \n", modes);

	return 0;
}

