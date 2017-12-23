#include <minix/syslib.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"



int main(int argc, char** argv){

	sef_startup();

	unsigned long *day = (unsigned long*) malloc(sizeof(unsigned long));
	unsigned long *month = (unsigned long*) malloc(sizeof(unsigned long));
	unsigned long *year = (unsigned long*) malloc(sizeof(unsigned long));

	unsigned long *hours = (unsigned long*) malloc(sizeof(unsigned long));
	unsigned long *minutes = (unsigned long*) malloc(sizeof(unsigned long));
	unsigned long *seconds = (unsigned long*) malloc(sizeof(unsigned long));

	getDate(day,month,year);
	getHour(hours,minutes,seconds);

	printf("Hello world\n");
	printf("Today is %d/%d/%d\n", *day, *month, *year + 2000);
	printf("The time now is %d hours %d minutes and %d seconds\n", *hours, *minutes, *seconds);

	return 0;
}
