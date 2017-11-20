#include "video_gr.h"
#include "video_test.h"
#include "test5.h"
#include "pixmap.h"
#include "vbe.h"
#include "kbd.h"
#include "timer.h"
#include "i8254.h"

#include <unistd.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdlib.h>

enum direction {
	VERTICAL, HORIZONTAL
};
typedef enum direction direction_t;

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
	sleep(5);

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

	int ipc_status, r, irq_set, int_counter = 0;
	message msg;

	if ((irq_set = timer_subscribe_int()) == -1)
		return -1;

	int incX = xi;
	int incY = yi;
	float pixelsInc = s;
	int int_per_frame = TIMER0_DEFAULT_FREQ / f;

	while (incX != xf || incY != yf) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set) {

					int_counter++;

					if (int_per_frame == int_counter) {
						int_counter = 0;
						switch (direction) {
						case VERTICAL:
							incY += pixelsInc;
							drawXPM(xi, incY, xpm);
							break;
						case HORIZONTAL:
							incX += pixelsInc;
							drawXPM(incX, yi, xpm);
							break;
						}
					}

				}
				break;
			}
		}
	}

	if (timer_unsubscribe_int() != OK)
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
	printf("\n pointer value: %d\n", vbe_info.VideoModePtr);
	printf("mode: %s", vbe_info.VideoModePtr);
	printf("total memory: %d\n", vbe_info.TotalMemory);

	return 0;
}

