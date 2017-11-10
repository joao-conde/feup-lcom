#include "i8254.h"
#include "i8042.h"
#include "test4.h"
#include "mouse.h"
#include "timer.h"
#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>

extern unsigned int g_packet_index;
extern unsigned long g_packet[PACKET_SIZE];
extern int g_synched;

int mouse_test_packet(unsigned short cnt) {

	int ipc_status, r, irq_set = mouse_subscribe_int();
	message msg;

	if (irq_set == -1) {
		printf("mouse_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}

	enable_mouse();
	enable_DataReporting();
	setStreamMode();

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

					if (g_packet_index > 2 && g_synched) {
						display_packet(g_packet);
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

	//cleaning KBC's output buffer of an eventual non-read byte
	cleanOBF();

	printf("\nmouse_test_g_packet(): exit\n");
	return OK;
}

int mouse_test_async(unsigned short idle_time) {

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
	setStreamMode();


	int i = 0;
	while (counter < (idle_time * TIMER0_DEFAULT_FREQ)) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

					counter = 0;

					if (g_packet_index > 2 && g_synched) {
						display_packet(g_packet);
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

	/*	Although it makes no sense to enable and then immediately disable data reporting
	 *  it doesn't work otherwise and by trial and error this solution was found
	 */
	enable_DataReporting();
	disable_DataReporting();
	setRemoteMode();

	int i = 0;
	while (i < cnt) {

		mouse_write_cmd(WRITE_BYTE, READ_DATA);

		if (g_packet_index > 2 && g_synched) {
			display_packet(g_packet);
			g_packet_index = 0;
			i++;
		}

		while (g_packet_index < 3)
			mouseIH();

		tickdelay(micros_to_ticks(period * MS_TO_MICRO));
	}

	setStreamMode();
	disable_DataReporting();

	mouse_unsubscribe_int();

	printf("\nmouse_test_remote(): exit\n");
	return OK;
}

int mouse_test_gesture(short length) {

	int ipc_status, r, irq_set = mouse_subscribe_int();
	message msg;

	if (irq_set == -1) {
		printf("mouse_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
	}

	enable_mouse();
	enable_DataReporting();
	setStreamMode();

	while (state != COMPLETE) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					if (g_packet_index > 2 && g_synched) {

						display_packet(g_packet);

						//state machine processing input from mouse
						processMouseInput(analyzePacket(g_packet, length));
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
		printf("mouse_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	//cleaning KBC's output buffer of an eventual non-read byte
	cleanOBF();

	printf("\nmouse_test_gesture(): exit\n");
	return OK;
}
