#include <minix/sysutil.h>
#include <minix/drivers.h>

#include "kbd.h"
#include "i8042.h"
#include "timer.h"
#include "i8254.h"

int kbd_test_scan(unsigned short ass) {

	int ipc_status, r, irq_set = kbd_subscribe_int();
	message msg;

	unsigned long scancode;

	if (irq_set == -1) {
		printf("kbd_subscribe_int(): Failure\n");
		return -1;
	}

	while (scancode != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					if (ass) {
						//INVOKE ASSEMBLY INTERRUPT - collect code in "scancode"
					} else
						scancode = kbc_read();

					print_scancode(scancode);

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

	printf("kbd_test_scan(): exit\n");
	return 0;
}

int kbd_test_poll() {
	/* To be completed */
}

int kbd_test_timed_scan(unsigned short n) {

	int ipc_status, r;
	message msg;
	unsigned long scancode;
	unsigned int counter = 0;

	int kbd_irq_set = kbd_subscribe_int();
	int timer_irq_set = timer_subscribe_int();

	if (kbd_irq_set == -1) {
		printf("kbd_subscribe_int(): Failure\n");
		return -1;
	}

	if (timer_irq_set == -1) {
		printf("timer_subscribe_int(): Failure\n");
		return -1;
	}

	while (scancode != ESC_BREAK && (counter < (n * TIMER0_DEFAULT_FREQ))) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed keyboard interrupt */
					scancode = kbc_read();
					print_scancode(scancode);
					counter = 0;
				}

				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed timer interrupt */
					counter++;
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}


	if (timer_unsubscribe_int() == -1) {
		printf("timer_unsubscribe_int(): Failure\n");
		return -1;
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return -1;
	}

	printf("kbd_test_timed_scan(): exit\n");
	return 0;
}
