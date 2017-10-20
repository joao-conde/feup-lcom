#include "kbd.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>

#ifdef LAB3

int sysinb_counter = 0;

int sys_inb_cnt(port_t port, unsigned long *byte){
	sysinb_counter++;
	return sys_inb(port,byte);
}

void print_sysinb_calls(){
	printf("no. of sys_inb() kernel calls: %d\n",sysinb_counter);
}

#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif


int hookID;
int twobytes = 0;


int kbd_subscribe_int(void) {

	hookID = KBD_BIT_ORDER;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID)
			!= OK) {
		printf("kbd_subscribe_int(): Failure setting policy\n");
		return -1;
	}

	if (sys_irqenable(&hookID) != OK) {
		printf("kbd_subscribe_int(): Failure enabling IRQ line\n");
		return -1;
	}

	return BIT(KBD_BIT_ORDER);
}

int kbd_unsubscribe_int(void) {

	if (sys_irqdisable(&hookID) != OK) {
		printf("kbd_unsubscribe_int(): Failure disabling IRQ line\n");
		return -1;
	}

	if (sys_irqrmpolicy(&hookID) != OK) {
		printf("kbd_unsubscribe_int(): Failure removing policy\n");
		return -1;
	}

	return 0;
}

int kbc_read() {

	unsigned long status, data;

	int retry = 0;
	while (retry < 5) {

		if (sys_inb_cnt(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return -1;
		}

		//loop while KBC output buffer is empty
		if (status & OBF) {

			if (sys_inb_cnt(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return -1;
			}


			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return data;
			} else
				return -1;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		retry++;
	}

	return -5;

}

void print_set1code(unsigned long scancode) {

	if (BIT(7) & scancode)
		printf("Break Code: 0x%x\n", scancode);
	else
		printf("Make Code: 0x%x\n", scancode);

}

void print_set2code(unsigned long scancode) {

	if (BIT(7) & scancode)
		printf("Break Code: 0xe0 0x%x\n", scancode);
	else
		printf("Make Code: 0xe0 0x%x\n", scancode);

	twobytes = 0;

}

void print_scancode(unsigned long scancode){

	if(scancode == SET2_SCANCODE){
		twobytes = 1;
		return;
	}

	if(twobytes)
		print_set2code(scancode);
	else
		print_set1code(scancode);

}
