#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include "timer.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdlib.h>


unsigned long assIH();

unsigned long scancode = 0;

void kbc_int_handler(){
	scancode = kbc_read();
}

int kbd_test_scan(unsigned short assembly) {

	int ipc_status, r, irq_set = kbd_subscribe_int();
	message msg;


	if (irq_set == -1) {
		printf("kbd_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
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

					if (assembly){
						assIH();
					}
					else
						kbc_int_handler();


					print_scancode(scancode);

				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}


#ifdef LAB3
	if(!assembly) print_sysinb_calls();
#endif


	if (kbd_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	printf("\nkbd_test_scan(): exit\n");
	return OK;
}

int kbd_test_poll() {

	int scancode = 0;

	while (scancode != ESC_BREAK) {
		scancode = kbc_polling();
		print_scancode(scancode);
	}

	print_sysinb_calls();

	printf("kbd_test_poll(): exit\n");
	return OK;
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
		return FAIL_SUB_INT;
	}

	if (timer_irq_set == -1) {
		printf("timer_subscribe_int(): Failure\n");
		return FAIL_SUB_INT;
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

	if(scancode == ESC_BREAK)
		printf("ESC RELEASED\n");
	else
		printf("TIMEOUT\n");


	if (timer_unsubscribe_int() == -1) {
		printf("timer_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return FAIL_UNSUB_INT;
	}

	printf("kbd_test_timed_scan(): exit\n");
	return OK;
}
