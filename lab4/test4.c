#include "i8254.h"
#include "i8042.h"
#include "test4.h"
#include "mouse.h"
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>

int mouse_test_packet(unsigned short cnt) {

	extern unsigned int packet_index;
	extern unsigned long packet[3];
	extern int synched;

	int ipc_status, r, irq_set = mouse_subscribe_int();
	message msg;

	if (irq_set == -1) {
		printf("mouse_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}

	enable_mouse();
	enable_DataReporting();

	int i = 0;
	while (i < cnt) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					if (packet_index > 2 && synched) {
						display_packet(packet);
						i++;
					}

					mouseIH();
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	disable_DataReporting();

	if (mouse_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	printf("\nmouse_test_packet(): exit\n");
	return OK;
}

int mouse_test_async(unsigned short idle_time) {
	extern unsigned int packet_index;
	extern unsigned long packet[3];
	extern int synched;

	int ipc_status, r, mouse_irq_set, timer_irq_set;

	unsigned long counter = 0;

	mouse_irq_set = mouse_subscribe_int();
	timer_irq_set = timer_subscribe_int();

	message msg;

	if (mouse_irq_set == -1) {
		printf("mouse_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}

	if (timer_irq_set == -1) {
		printf("timer_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}

	enable_mouse();
	enable_DataReporting();

	int i = 0;
	while (counter < (idle_time * TIMER0_DEFAULT_FREQ)){

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
					counter = 0;

					if (packet_index > 2 && synched) {
						display_packet(packet);
						i++;
					}

					mouseIH();
				}

				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	disable_DataReporting();

	if (mouse_unsubscribe_int() == -1) {
		printf("mouse_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	if (timer_unsubscribe_int() == -1) {
		printf("timer_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	printf("\nmouse_test_async(): exit\n");
	return OK;
}

int mouse_test_remote(unsigned long period, unsigned short cnt) {
	mouse_subscribe_int();

	unsigned long* remote_packet;

	disable_DataReporting();
	setRemoteMode();

	int i = 0;
	while (i < cnt) {

		remote_packet = create_remotePacket(period);
		display_packet(remote_packet);
		i++;
	}

	setStreamMode();
	disable_DataReporting();

	mouse_unsubscribe_int();
}

int mouse_test_gesture(short length) {
	printf("\nHELLO IM MOUSE_TEST_GESTURE()\n");
	printf("\nARGUMENT LENGTH: %d\n", length);
}
