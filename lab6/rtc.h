#ifndef RTC_H
#define RTC_H

#define BIT(n) (0x01<<(n))


/* Access to RTC */

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


/* RTC’s Internal Address Space */

/* REGISTER */
#define REGA 10
#define REGB 11

/* INPUTS */
#define DAY   7
#define MONTH 8
#define YEAR  9

#define HOURS   4
#define MINUTES 2
#define SECONDS 0

/* FLAGS */
#define REGA_UIP BIT(7)
#define REGB_BIN BIT(2)


/* CURENT MILLENIUM  */
#define CURRENT_MILLENIUM 2000



int isRTCUpdating();

int isBCD();

unsigned long BCDtoBin(unsigned long* bcd);

void getDate(unsigned long *day, unsigned long *month, unsigned long *year);

void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds);



#endif /* RTC_H */
