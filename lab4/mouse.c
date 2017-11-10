#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>

int mouse_hookID;

unsigned long packet[PACKET_SIZE];
unsigned int packet_index = 0;
int synched = FALSE;

int cleanOBF() {
	return mouse_readOBF();
}

int mouse_subscribe_int(void) {

	mouse_hookID = MOUSE_BIT_ORDER;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookID)
			!= OK) {
		printf("mouse_subscribe_int(): Failure setting policy\n");
		return FAIL_SET_POLICY;
	}

	if (sys_irqenable(&mouse_hookID) != OK) {
		printf("mouse_subscribe_int(): Failure enabling IRQ line\n");
		return FAIL_ENABLE_IRQ;
	}

	return BIT(MOUSE_BIT_ORDER);
}

int mouse_unsubscribe_int(void) {

	if (sys_irqdisable(&mouse_hookID) != OK) {
		printf("mouse_unsubscribe_int(): Failure disabling IRQ line\n");
		return FAIL_DISABLE_IRQ;
	}

	if (sys_irqrmpolicy(&mouse_hookID) != OK) {
		printf("mouse_unsubscribe_int(): Failure removing policy\n");
		return FAIL_REMOVE_POLICY;
	}

	return OK;
}

long mouse_readOBF() {
	unsigned long byte, status;
	int retry = 0;
	while (retry < 5) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("mouse_readOBF(): failure to read STAT_REG\n");
			return -1;
		}

		if (status & OBF) {

			if (sys_inb(OUT_BUF, &byte) != OK) {
				printf("mouse_readOBF(): failure to read OUT_BUF\n");
				return -1;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return byte;
			} else
				return ERROR_STATUS;

			retry++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
	}

	return -1;
}

long mouse_kbc_polling() {

	unsigned long status, data;

	int retry = 0;
	while (1) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return FAIL_READ_STATUS;
		}

		if (status & OBF) {

			if (sys_inb(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return FAIL_READ_OUTBUF;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				tickdelay(micros_to_ticks(DELAY_US));
				return data;
			} else
				return ERROR_STATUS;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		retry++;
	}

	printf("failure polling\n");
	return -1;
}

void mouseIH() {

	long byte = mouse_readOBF();
	if (byte == -1)
		return;

	if (packet_index > 2) {
		synched = FALSE;
	}

	synch_packet(byte);

	if (synched) {
		packet[packet_index] = byte;
		packet_index++;
	}

}

void synch_packet(long byte) {

	if (synched)
		return;

	if (FIRSTBYTE & byte) {
		packet_index = 0;
		synched = TRUE;
	}
}

void display_packet(unsigned long *packet) {

	//print of the 3 bytes of the packet
	printf("B1=0x%02x ", *packet);
	printf("B2=0x%02x ", *(packet + 1));
	printf("B3=0x%02x ", *(packet + 2));

	//analyze of each packet
	printf("LB=%u ", (*packet & BIT(0)) ? 1 : 0);
	printf("MB=%u ", (*packet & BIT(2)) ? 1 : 0);
	printf("RB=%u ", (*packet & BIT(1)) ? 1 : 0);

	printf("XOVF=%u ", (*packet & BIT(6)) ? 1 : 0);
	printf("YOVF=%u ", (*packet & BIT(7)) ? 1 : 0);

	if (*packet & BIT(4))
		printf("X=-%u ", (*(packet + 1) ^= 0xFF) + 1);
	else
		printf("X=%u ", *(packet + 1));

	if (*packet & BIT(5))
		printf("Y=-%u\n", (*(packet + 2) ^= 0xFF) + 1);
	else
		printf("Y=%u\n", *(packet + 2));

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

int mouse_write_cmd(unsigned long cmd, unsigned long word) {

	unsigned long response = 0;
	int retries = 10;

	do {
		kbc_write(STAT_REG, cmd);
		kbc_write(INP_BUF, word);
		//tickdelay(micros_to_ticks(DELAY_US));

		response = mouse_readOBF();
		retries--;

	} while (response != ACK);

	if (response == ACK)
		return OK;

	return -1;

}

int enable_DataReporting() {
	mouse_write_cmd(WRITE_BYTE, ENABLE_DATAREPORT);
	return OK;
}

int enable_mouse() {
	kbc_write(STAT_REG, ENABLE_MOUSE);
	return OK;
}

int disable_DataReporting() {
	mouse_write_cmd(WRITE_BYTE, DISABLE_DATAREPORT);
	return OK;
}

int setRemoteMode() {
	mouse_write_cmd(WRITE_BYTE, ENABLE_REMOTE);
	return OK;
}

int setStreamMode() {
	mouse_write_cmd(WRITE_BYTE, ENABLE_STREAM);
	return OK;
}

