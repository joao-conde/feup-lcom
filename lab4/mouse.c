#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include "timer.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>

int mouse_hookID;

unsigned long packet[3];
unsigned int packet_index = 0;
int synched = FALSE;


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

		if (sys_inb(STAT_REG, &status) != OK)
			return -1;

		if (status & OBF) {
			if (sys_inb(OUT_BUF, &byte) != OK)
				return -1;

			return byte;
		}

		retry++;
		tickdelay(micros_to_ticks(DELAY_US));
	}

	return -1;
}



int mouse_kbc_polling(unsigned long period) {

	unsigned long status, data;

	//add retrys
	while (1) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("kbc_read(): Failure reading status register of KBC\n");
			return FAIL_READ_STATUS;
		}

		//loop while KBC output buffer is empty
		if (status & OBF) {

			if (sys_inb(OUT_BUF, &data) != OK) {
				printf("kbc_read(): Failure reading output buffer of KBC\n");
				return FAIL_READ_OUTBUF;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return data;
			} else
				return ERROR_STATUS;
		}

		tickdelay(micros_to_ticks(period*MS_TO_MICRO));
	}
}

unsigned long* create_remotePacket(unsigned long period){

	unsigned long byte, *packet;

	mouse_write_cmd(WRITE_BYTE,0xEB);

	int i = 0;
	while(i < 3){
		byte = mouse_kbc_polling(period);
		tickdelay(micros_to_ticks(period*MS_TO_MICRO));

		if(byte != -1){
			*(packet+i) = byte;
			i++;
		}
	}

	return packet;
}




void mouseIH() {

	long byte = mouse_readOBF();

	if (byte == -1)
		return;

	if (packet_index > 2){
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


void display_packet(long *packet) {

	printf("B1=%02x ", *packet);
	printf("B2=%02x ", *(packet + 1));
	printf("B3=%02x ", *(packet + 2));
	printf("LB=%u ", *packet & BIT(0));
	printf("MB=%u ", *packet & BIT(2));
	printf("RB=%u ", *packet & BIT(1));
	printf("XOVF=%u ", *packet & BIT(6));
	printf("YOVF=%u ", *packet & BIT(7));

	if (BIT(7) & *(packet + 1)) {
		*(packet + 1) = *(packet + 1) ^ 255;
		*(packet + 1) = -*(packet + 1);
	}
	if (BIT(7) & *(packet + 2)) {
		*(packet + 2) = *(packet + 2) ^ 255;
		*(packet + 2) = -*(packet + 2);
	}

	printf("X=%ld ", *(packet + 1));
	printf("Y=%ld\n", *(packet + 2));
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

}

int mouse_write_cmd(unsigned long cmd, unsigned long word) {
	kbc_write(STAT_REG, cmd);
	kbc_write(INP_BUF, word);
}

void enable_DataReporting() {
	mouse_write_cmd(WRITE_BYTE,ENABLE_DATAREPORT);
}

void enable_mouse(){
	kbc_write(STAT_REG, ENABLE_MOUSE);
}

void disable_DataReporting(){
	mouse_write_cmd(WRITE_BYTE,DISABLE_DATAREPORT);
}

void setRemoteMode(){
	mouse_write_cmd(WRITE_BYTE,ENABLE_REMOTE);
}

void setStreamMode(){
	mouse_write_cmd(WRITE_BYTE,ENABLE_STREAM);
}



