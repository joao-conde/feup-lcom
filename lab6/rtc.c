#include "rtc.h"

#include <minix/drivers.h>

// 1 if updating
int isRTCUpdating(){

	unsigned long regA;

	sys_outb(RTC_ADDR_REG, REGA);
	sys_inb(RTC_DATA_REG, &regA);

	//check UIP flag - 1 if updating
	return (regA & BIT(7));

}

// 1 if BCD 0 for binary
int isBCD(){
	unsigned long regB;

	sys_outb(RTC_ADDR_REG, REGB);
	sys_inb(RTC_DATA_REG, &regB);


	return (!(regB & BIT(2)));
}


unsigned long BCDtoBin(unsigned long* bcd){
	return (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);
}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year){

	sys_outb(RTC_ADDR_REG, 7);
	sys_inb(RTC_DATA_REG, day);

	sys_outb(RTC_ADDR_REG, 8);
	sys_inb(RTC_DATA_REG, month);

	sys_outb(RTC_ADDR_REG, 9);
	sys_inb(RTC_DATA_REG, year);

	if(isBCD()){
		(*day) = BCDtoBin(day);
		(*month) = BCDtoBin(month);
		(*year) = BCDtoBin(year);
	}

	*year += CURRENT_MILLENIUM;

}

void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds){

	sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, hour);

	sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, minutes);

	sys_outb(RTC_ADDR_REG, 0);
	sys_inb(RTC_DATA_REG, seconds);

	if (isBCD()) {
		(*hour) = BCDtoBin(hour);
		(*minutes) = BCDtoBin(minutes);
		(*seconds) = BCDtoBin(seconds);
	}

}
