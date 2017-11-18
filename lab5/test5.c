#include "video_gr.h"
#include "test5.h"
#include "pixmap.h"
#include "vbe.h"
#include "kbd.h"

#include <unistd.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdlib.h>

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

	int ipc_status = 0;
	int r = 0;
	int irq_set = kbd_subscribe_int();
	printf("R VALUE: %d\n", r);

	message msg;

	unsigned long scancode = 0;

	if (irq_set == -1) {
		printf("kbd_subscribe_int(): Failure\n");
		return -1;
	}
	printf("IRQ SET: %d", irq_set);
	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	drawSquare(x, y, size, color);

	while (scancode != ESC_BREAK) {
		printf("IN CICLE\n");
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		printf("AFTER DRIVER\n");

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					scancode = kbc_read();
					printf("READ SCAN CODE: 0x%x", scancode);
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}


	if (kbd_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return -1;
	}

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	vg_init(VBE_MODE105);

	drawLine(xi, yi, xf, yf, color);
	sleep(5);

	vg_exit();

	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	if (vg_init(VBE_MODE105) == NULL)
		return -1;

	draw_xpm(xi, yi, xpm);

	sleep(5);

	if (vg_exit() != 0)
		return -1;

	return 0;
}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short s, unsigned short f) {

	/* To be completed */
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

