#include "rtc.h"

#include <minix/drivers.h>

// 1 if updating
int isRTCUpdating(){

	unsigned long regA;

	sys_outb(0x70, 10);
	sys_inb(0x71, &regA);

	if ((regA & BIT(7)) != 0)
		return 1;

	return 0;
}

// 1 if BCD
int isBCD(){
	unsigned long regB;

	sys_outb(0x70, 11);
	sys_inb(0x71, &regB);

	if(!(regB & BIT(2)))
		return 1;

	return 0;
}


unsigned long BCDtoBin(unsigned long* bcd){
	return (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);
}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year){

	sys_outb(0x70, 7);
	sys_inb(0x71, day);
	sys_outb(0x70, 8);
	sys_inb(0x71, month);
	sys_outb(0x70, 9);
	sys_inb(0x71, year);

	if(isBCD()){
		(*day) = BCDtoBin(day);
		(*month) = BCDtoBin(month);
		(*year) = BCDtoBin(year);
	}

}

void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds){

	sys_outb(0x70, 4);
	sys_inb(0x71, hour);
	sys_outb(0x70, 2);
	sys_inb(0x71, minutes);
	sys_outb(0x70, 0);
	sys_inb(0x71, seconds);

	if (isBCD()) {
		(*hour) = BCDtoBin(hour);
		(*minutes) = BCDtoBin(minutes);
		(*seconds) = BCDtoBin(seconds);
	}

}
