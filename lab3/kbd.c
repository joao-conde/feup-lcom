#include "kbd.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>

int hookID;
int twobytes = 0;
int sysinb_calls = 0;

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
	sysinb_calls = 0;

	int retry = 0;
	while (retry < 5) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return -1;
		}
		sysinb_calls++;

		//loop while KBC output buffer is empty
		if (status & OBF) {

			if (sys_inb(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return -1;
			}
			sysinb_calls++;

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

int kbc_write(unsigned long port, unsigned long word) {

	unsigned long status;
	int retry = 0;

	while (retry < 5) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("kbc_write(): Failure reading from status register\n");
			return -1;
		}

		if (!(status & IBF)) {

			if (sys_outb(port, word) != OK) {
				printf("kbc_write(): Failure writing to port %lu\n", port);
				return -1;
			}

			return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		retry++;
	}

	return -1;
}

int kbc_write_cmd(unsigned long cmd, unsigned long word) {
	/*	kbc_write(KBC_CMD_REG, cmd);
	 kbc_write(INP_BUF, word);
	 */
	unsigned long kbd_response;

	while (1) {

		if (kbc_write(KBC_CMD_REG, cmd) != 0) {
			printf(
					"kbc_write_cmd(): Failure writing command to KBC command register\n");
			return -1;
		}

		kbd_response = kbc_read();
		printf("kbd_response: %x\n", kbd_response);

		if (kbd_response == -1) {
			printf("kbc_write_cmd(): failure writing command\n");
			return -1;
		}

		if (kbd_response != RESEND && kbd_response != ACK
				&& kbd_response != ERROR) {
			printf("kbc_write_cmd(): unexpected kbd response outer loop\n");
			continue;
		}

		if (kbd_response == ACK) {
			printf("received ACK\n");
			while (1) {
				if (kbc_write(INP_BUF, word) != 0) {
					printf(
							"kbc_write_cmd(): failure writing word to the KBC input buffer\n");
					return -1;
				}

				kbd_response = kbc_read();

				if (kbd_response == -1) {
					printf("kbc_write_cmd(): failure writing argument\n");
					return -1;
				}

				if (kbd_response != RESEND && kbd_response != ACK
						&& kbd_response != ERROR) {
					printf(
							"kbc_write_cmd(): unexpected kbd response inner loop\n");
					continue;
				}

				if (kbd_response == RESEND)
					continue;
				if (kbd_response == ACK)
					return 0;

				if (kbd_response == ERROR)
					break;

			}
		}

	}

	return -1;
}


int kbc_get_sysinbcalls(){
	return sysinb_calls;
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
