#include "test4.h"
#include "i8254.h"
#include "i8042.h"
#include "mouse.h"
#include "timer.h"
#include "kbd.h"

#include <minix/sysutil.h>
#include <minix/drivers.h>



int mouse_test_packet(unsigned short cnt){

	mouse_subscribe_int();


	printf("Working\n");
	read_packet();


	mouse_unsubscribe_int();
	return OK;
}
