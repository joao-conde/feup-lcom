#include "kbd.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>

#ifdef LAB3

int sysinb_counter = 0;

int sys_inb_cnt(port_t port, unsigned long *byte) {
	sysinb_counter++;
	return sys_inb(port,byte);
}

void print_sysinb_calls() {
	printf("no. of sys_inb() kernel calls: %d\n",sysinb_counter);
}

#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int kbd_hookID;
int twobytes = 0;



int kbd_subscribe_int(void) {

	kbd_hookID = KBD_BIT_ORDER;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hookID)
			!= OK) {
		printf("kbd_subscribe_int(): Failure setting policy\n");
		return -1;
	}

	if (sys_irqenable(&kbd_hookID) != OK) {
		printf("kbd_subscribe_int(): Failure enabling IRQ line\n");
		return -1;
	}

	return BIT(KBD_BIT_ORDER);
}

int kbd_unsubscribe_int(void) {

	if (sys_irqdisable(&kbd_hookID) != OK) {
		printf("kbd_unsubscribe_int(): Failure disabling IRQ line\n");
		return -1;
	}

	if (sys_irqrmpolicy(&kbd_hookID) != OK) {
		printf("kbd_unsubscribe_int(): Failure removing policy\n");
		return -1;
	}

	return OK;
}

int kbc_read() {

	unsigned long status, data;

	int retry = 0;
	while (retry < 5) {

		if (sys_inb_cnt(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return FAIL_READ_STATUS;
		}

		//loop while KBC output buffer is empty
		if (status & OBF) {

			if (sys_inb_cnt(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return FAIL_READ_OUTBUF;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return data;
			} else
				return ERROR_STATUS;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		retry++;
	}

	return TRIES_EXCEED;
}

int kbc_polling() {

	unsigned long status, data;

	while (1) {

		if (sys_inb_cnt(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return FAIL_READ_STATUS;
		}

		//loop while KBC output buffer is empty
		if (status & OBF) {

			if (sys_inb_cnt(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return FAIL_READ_OUTBUF;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return data;
			} else
				return ERROR_STATUS;
		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

}

void print_set1code(unsigned long scancode) {

	if (BREAKCODE & scancode)
		printf("Break Code: 0x%x\n", scancode);
	else
		printf("Make Code: 0x%x\n", scancode);

}

void print_set2code(unsigned long scancode) {

	if (BREAKCODE & scancode)
		printf("Break Code: 0xe0 0x%x\n", scancode);
	else
		printf("Make Code: 0xe0 0x%x\n", scancode);

	twobytes = 0;

}

void print_scancode(unsigned long scancode) {

	if (scancode == SET2_SCANCODE) {
		twobytes = 1;
		return;
	}

	if (twobytes)
		print_set2code(scancode);
	else
		print_set1code(scancode);

}

int waitEscapeKey(){

	unsigned long scancode = 0;

	int ipc_status, r, irq_set = kbd_subscribe_int();
	message msg;


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
					scancode = kbc_read();
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

	return OK;
}
