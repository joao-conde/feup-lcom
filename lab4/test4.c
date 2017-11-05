#include "i8254.h"
#include "i8042.h"
#include "test4.h"
#include "mouse.h"



#include <stdio.h>
#include <stdlib.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>

int mouse_test_packet(unsigned short cnt) {

	extern unsigned int packet_index;
	extern unsigned long packet[3];

	int ipc_status, r, irq_set = mouse_subscribe_int();
	message msg;

	if (irq_set == -1) {
		printf("mouse_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}


	if (sys_outb(STAT_REG, 0xA8) != OK)
		printf("\nERRO na primeira escrita\n");
	if (sys_outb(STAT_REG, 0xD4) != OK)
		printf("\nERRO na segunda escrita\n");
	if (sys_outb(INP_BUF, 0xF4) != OK)
		printf("\nERRO na terceira escrita\n");

	while (1) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					mouseIH();

					if(packet_index == 2)
						display_packet(packet);
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	if(sys_outb(STAT_REG, 0xD4)!=OK) printf("ERROR on writing to 0x64");
	if(sys_outb(INP_BUF,0xF5)!=OK) printf("ERROR on writing to 0x60");

	if (mouse_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	printf("\nmouse_test_packet(): exit\n");
	return OK;
}

int mouse_test_async(unsigned short idle_time) {
	printf("\nHELLO IM MOUSE_TEST_ASYNC()\n");
	printf("\nARGUMENT IDLE_TIME: %lu\n", idle_time);
}

int mouse_test_remote(unsigned long period, unsigned short cnt) {
	printf("\nHELLO IM MOUSE_TEST_REMOTE()\n");
	printf("\nARGUMENT PERIOD: %lu\n", period);
	printf("\nARGUMENT CNT: %lu\n", cnt);
}

int mouse_test_gesture(short length) {
	printf("\nHELLO IM MOUSE_TEST_GESTURE()\n");
	printf("\nARGUMENT LENGTH: %d\n", length);
}
