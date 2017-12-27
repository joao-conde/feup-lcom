#include "rtc.h"

#include <minix/drivers.h>

int isRTCUpdating(){

	unsigned long regA;

	sys_outb(RTC_ADDR_REG, REGA);
	sys_inb(RTC_DATA_REG, &regA);

	//check UIP flag - 1 if updating
	return (regA & REGA_UIP);

}

int isBCD(){
	unsigned long regB;

	sys_outb(RTC_ADDR_REG, REGB);
	sys_inb(RTC_DATA_REG, &regB);


	return (!(regB & REGB_BIN));
}


unsigned long BCDtoBin(unsigned long* bcd){
	/* From:
	 * https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
	*/
	return (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);
}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year){

	sys_outb(RTC_ADDR_REG, DAY);
	sys_inb(RTC_DATA_REG, day);

	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, month);

	sys_outb(RTC_ADDR_REG, YEAR);
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

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, hour);

	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, minutes);

	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, seconds);

	if (isBCD()) {
		(*hour) = BCDtoBin(hour);
		(*minutes) = BCDtoBin(minutes);
		(*seconds) = BCDtoBin(seconds);
	}

}
