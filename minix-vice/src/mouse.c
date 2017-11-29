#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "video_gr.h"
#include "pixmap.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>

int mouse_hookID;

unsigned long g_packet[PACKET_SIZE];
unsigned int g_packet_index = 0;
int g_synched = FALSE;

/* SINGLETON MOUSE IMPLEMENTATION */
Mouse* mouse = NULL;

Mouse* newMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 200;
	mouse->y = 200;

	mouse->deltaX = 0;
	mouse->deltaY = 0;

	mouse->LBtnDown = 0;
    mouse->MBtnDown = 0;
    mouse->RBtnDown = 0;

    mouse->draw = 0;

	return mouse;
}

Mouse* getMouse() {
	if (!mouse) {
		enable_mouse();
		enable_DataReporting();
		setStreamMode();
		mouse = newMouse();
	}

	return mouse;
}

void drawMouse() {
	Mouse* m = getMouse();

	//erase old cursor position
	eraseXPM(m->x - m->deltaX, m->y - m->deltaY, cross);

	//paint cursor in new position
	drawXPM(m->x, m->y, cross);
	m->draw = 0;
}

void deleteMouse() {
	free(getMouse());
}

void updateMouse() {
	Mouse* m = getMouse();

	memcpy(m->packet, g_packet, sizeof(m->packet));

	m->LBtnDown = m->packet[1] & BIT(0);
	m->RBtnDown = m->packet[1] & BIT(1);
	m->MBtnDown = m->packet[1] & BIT(2);

	//calculate deltas depending on whether its 2's comp or not

	if (m->packet[0] & BIT(4))
		m->deltaX =  -((m->packet[1] ^= 0xFF) + 1);
	else
		m->deltaX = m->packet[1];

	if (m->packet[0] & BIT(5))
		m->deltaY = ((m->packet[2] ^= 0xFF) + 1);
	else
		m->deltaY = - m->packet[2];

	m->x += m->deltaX;
	m->y += m->deltaY;

	if (m->x < 0)
		m->x = 0;

	if (m->x > vg_getHRES())
		m->x = vg_getHRES();


	if (m->y < 0)
		m->y = 0;

	if (m->y > vg_getVRES())
		m->y = vg_getVRES();


	m->draw = 1;

	printf("\nNEW MOVEMENT\n");
	printf("MOUSE LBBtn: %d\nMOUSE RBTN: %d\nMOUSE MBTN: %d\n", m->LBtnDown, m->RBtnDown, m->MBtnDown);
	printf("MOUSE DELTAX: %d\nMOUSE DELTAY: %d\n", m->deltaX, m->deltaY);
	printf("MOUSE X: %d\nMOUSE Y: %d\n", m->x, m->y);
}

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
			return FAIL_READ_STATUS;
		}

		if (status & OBF) {

			if (sys_inb(OUT_BUF, &byte) != OK) {
				printf("mouse_readOBF(): failure to read OUT_BUF\n");
				return FAIL_READ_OUTBUF;
			}

			if ((status & (PAR_ERR | TO_ERR)) == 0) {
				return byte;
			} else
				return ERROR_STATUS;

			retry++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
	}

	return TRIES_EXCEED;
}

void mouseIH() {

	long byte = mouse_readOBF();

	if (byte == -1)
		return;

	if (g_packet_index > 2) {
		g_synched = FALSE;
	}

	synch_packet(byte);

	if (g_synched) {
		g_packet[g_packet_index] = byte;
		g_packet_index++;
	}

}

void synch_packet(long byte) {

	if (g_synched)
		return;

	if (FIRSTBYTE & byte) {
		g_packet_index = 0;
		g_synched = TRUE;
	}
}

void display_packet(unsigned long *g_packet) {

	//print of the 3 bytes of the g_packet
	printf("B1=0x%02x ", *g_packet);
	printf("B2=0x%02x ", *(g_packet + 1));
	printf("B3=0x%02x ", *(g_packet + 2));

	//analyze of each g_packet
	printf("LB=%u ", (*g_packet & BIT(0)) ? 1 : 0);
	printf("MB=%u ", (*g_packet & BIT(2)) ? 1 : 0);
	printf("RB=%u ", (*g_packet & BIT(1)) ? 1 : 0);

	printf("XOVF=%u ", (*g_packet & BIT(6)) ? 1 : 0);
	printf("YOVF=%u ", (*g_packet & BIT(7)) ? 1 : 0);

	if (*g_packet & BIT(4))
		printf("X=-%u ", (*(g_packet + 1) ^= BYTE_MINUS1) + 1);
	else
		printf("X=%u ", *(g_packet + 1));

	if (*g_packet & BIT(5))
		printf("Y=-%u\n", (*(g_packet + 2) ^= BYTE_MINUS1) + 1);
	else
		printf("Y=%u\n", *(g_packet + 2));

}

int kbc_write(unsigned long port, unsigned long word) {

	unsigned long status;
	int retry = 0;

	while (retry < 5) {

		if (sys_inb(STAT_REG, &status) != OK) {
			printf("kbc_write(): Failure reading from status register\n");
			return FAIL_READ_STATUS;
		}

		if (!(status & IBF)) {

			if (sys_outb(port, word) != OK) {
				printf("kbc_write(): Failure writing to port %lu\n", port);
				return FAIL_READ_PORT;
			}

			return OK;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		retry++;
	}

	return TRIES_EXCEED;
}

int mouse_write_cmd(unsigned long cmd, unsigned long word) {

	unsigned long response = 0;
	int retries = 10;

	do {

		kbc_write(STAT_REG, cmd);
		kbc_write(INP_BUF, word);

		response = mouse_readOBF();
		retries--;

	} while (response != ACK);

	if (response == ACK)
		return OK;

	return TRIES_EXCEED;

}

int enable_DataReporting() {
	if (mouse_write_cmd(WRITE_BYTE, ENABLE_DATAREPORT) != OK)
		return FAIL_WRITE_CMD;

	return OK;
}

int enable_mouse() {
	if (kbc_write(STAT_REG, ENABLE_MOUSE) != OK)
		return FAIL_WRITE_CMD;

	return OK;
}

int disable_DataReporting() {
	if (mouse_write_cmd(WRITE_BYTE, DISABLE_DATAREPORT) != OK)
		return FAIL_WRITE_CMD;

	return OK;
}

int setRemoteMode() {
	if (mouse_write_cmd(WRITE_BYTE, ENABLE_REMOTE) != OK)
		return FAIL_WRITE_CMD;

	return OK;
}

int setStreamMode() {
	if (mouse_write_cmd(WRITE_BYTE, ENABLE_STREAM) != OK)
		return FAIL_WRITE_CMD;

	return OK;
}

