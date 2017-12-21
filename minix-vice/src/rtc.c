#include "rtc.h"

#include <minix/drivers.h>


//TODO check if rtc is updating, verifying the UIP in regA
int isRTCUpdating(){

	unsigned long regA;

	sys_outb(0x70, 10);
	sys_inb(0x71, &regA);

	if ((regA & BIT(7)) != 0)
		return 1;

	return 0;
}

//TODO develop a function to get the date from rtc
void getDate(unsigned long *day, unsigned long *month, unsigned long *year){

	sys_outb(0x70, 7);
	sys_inb(0x71, day);
	sys_outb(0x70, 8);
	sys_inb(0x71, month);
	sys_outb(0x70, 9);
	sys_inb(0x71, year);

}

//TODO develop a function to get the hour from rtc
void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds){

	sys_outb(0x70, 4);
	sys_inb(0x71, hour);
	sys_outb(0x70, 2);
	sys_inb(0x71, minutes);
	sys_outb(0x70, 0);
	sys_inb(0x71, seconds);

}
