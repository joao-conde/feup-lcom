#ifndef RTC_H
#define RTC_H


//TODO check if rtc is updating, verifying the UIP in regA
int isRTCUpdating();

//TODO develop a function to get the date from rtc
void getDate(unsigned long *day, unsigned long *month, unsigned long *year);

//TODO develop a function to get the hour from rtc
void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds);



#endif /* RTC_H */
