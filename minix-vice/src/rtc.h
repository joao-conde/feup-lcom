#ifndef RTC_H
#define RTC_H

#define BIT(n) (0x01<<(n))


int isRTCUpdating();

int isBCD();

unsigned long BCDtoBin(unsigned long* bcd);

void getDate(unsigned long *day, unsigned long *month, unsigned long *year);

void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds);



#endif /* RTC_H */
