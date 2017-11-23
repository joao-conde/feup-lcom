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
	UP, DOWN, LEFT, RIGHT, STOP
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

	//if both dx and dy are 0 state will be STOP therefore no movement
	direction_t direction = STOP;

	int dx = xf - xi;
	int dy = yf - yi;

	if (dx == 0) {

		if (dy > 0)
			direction = DOWN;

		if (dy < 0)
			direction = UP;

	} else {

		if (dx > 0)
			direction = RIGHT;

		if (dx < 0)
			direction = LEFT;

	}

	int ipc_status, r, timer_irq_set, kbd_irq_set, int_counter = 0;
	message msg;

	float speed, incX = xi, incY = yi;
	float pixelsInc = (speed * f) / TIMER_DEF_FREQ_DOUBLE;
	unsigned long scancode = 0;

	if ((timer_irq_set = timer_subscribe_int()) == -1)
		return -1;

	if ((kbd_irq_set = kbd_subscribe_int()) == -1)
		return -1;

	if (s < 0)
		speed = 1.0 / (abs(s));
	else
		speed = s;


	while (/*(incX != xf || incY != yf) &&*/ scancode != ESC_BREAK) {

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

					case STOP:
						drawXPM(xi, yi, xpm);
						break;

					case DOWN:
						eraseXPM(xi, (int) incY, xpm);
						incY += pixelsInc;
						drawXPM(xi, (int) incY, xpm);
						break;

					case UP:
						eraseXPM(xi, (int) incY, xpm);
						incY -= pixelsInc;
						drawXPM(xi, (int) incY, xpm);
						break;

					case RIGHT:
						eraseXPM((int) incX, yi, xpm);
						incX += pixelsInc;
						drawXPM((int) incX, yi, xpm);
						break;

					case LEFT:
						eraseXPM((int) incX, yi, xpm);
						incX -= pixelsInc;
						drawXPM((int) incX, yi, xpm);
						break;

					}

				}

				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					scancode = kbc_read();
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

	vbe_get_info(VBE_MODE105, &vbe_info);

	printf("\nSTRUCTURE VBE INFO\n");

	printf("version: %x\n", vbe_info.VESAVersion);

	printf("total memory: %d\n", vbe_info.TotalMemory);

	int segment = (*vbe_info.VideoModePtr >> 16) & 0xFFFF;
	int offset = (*vbe_info.VideoModePtr & 0xFFFF) << 4;
	int linear = segment * 16 + offset;

	unsigned* modes = vbe_info.VideoModePtr + linear;

	printf("Modes: 0x%x\n", modes);

	return 0;
}

