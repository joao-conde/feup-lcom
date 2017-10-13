#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"


unsigned int counter = 0;
int hookID;


int timer_set_square(unsigned long timer, unsigned long freq) {


	if(timer < 0 || timer > 2){
		printf("Timer %lu does not exist. Select a timer from 0 to 2\n\n",timer);
		return 1;
	}


	if(freq < 0){
		printf("Invalid frequency - MUST NOT BE NEGATIVE\n");
		return 1;
	}


	unsigned long controlW, newFreq;
	unsigned char status;

	newFreq = TIMER_FREQ / freq;

	//reading timer configuration to maintain last 4 bits
	timer_get_conf(timer, &status);
	controlW = (unsigned long)status;


	switch(timer){

	case 0:
		controlW |= TIMER_SEL0 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return 1;
		}

		if(sys_outb(TIMER_0,newFreq) != OK){
			printf("Error writing LSB\n");
			return 1;
		}

		if(sys_outb(TIMER_0,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return 1;
		}

		break;

	case 1:
		controlW |= TIMER_SEL1 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return 1;
		}

		if(sys_outb(TIMER_1,newFreq) != OK){
			printf("Error writing LSB\n");
			return 1;
		}

		if(sys_outb(TIMER_1,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return 1;
		}

		break;

	case 2:
		controlW |= TIMER_SEL2 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return 1;
		}

		if(sys_outb(TIMER_2,newFreq) != OK){
			printf("Error writing LSB\n");
			return 1;
		}

		if(sys_outb(TIMER_2,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return 1;
		}

		break;

	}

	return 0;
}

int timer_subscribe_int(void) {

	hookID = TIMER0_IRQ;

	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hookID) != OK){
		printf("Failure setting policy\n");
		return -1;
	}

	if(sys_irqenable(&hookID) != OK){
		printf("Failure enabling IRQ line\n");
		return -1;
	}

	return BIT(TIMER0_IRQ);
}

int timer_unsubscribe_int() {


	if(sys_irqdisable(&hookID) != OK){
		printf("Failure disabling IRQ line\n");
		return -1;
	}

	if(sys_irqrmpolicy(&hookID)!= OK){
		printf("Failure removing policy\n");
		return -1;
	}

	return 0;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	
	if(timer < 0 || timer > 2){
		printf("Timer %lu does not exist. Select a timer from 0 to 2\n\n",timer);
		return 1;
	}

	unsigned long rbcommand, config;

	//building the Read-Back command control word
	rbcommand |= TIMER_RB_CMD;
	rbcommand |= TIMER_RB_COUNT_;
	rbcommand |= TIMER_RB_STATUS_;

	switch(timer){

	case 0:
		rbcommand |= TIMER_0;
		break;

	case 1:
		rbcommand |= TIMER_1;
		break;

	case 2:
		rbcommand |= TIMER_2;
		break;

	}

	//writes the Read-Back command to the control register
	if(sys_outb(TIMER_CTRL,rbcommand) != OK){
		printf("Error writing READ-BACK cmd\n");
		return 1;
	}


	//reading the configuration of the desired timer
	switch(timer){

	case 0:
		sys_inb(TIMER_0,&config);
		break;

	case 1:
		sys_inb(TIMER_1,&config);
		break;

	case 2:
		sys_inb(TIMER_2,&config);
		break;

	}

	*st=(unsigned char)config;

	return 0;
}

int timer_display_conf(unsigned char conf) {
	

	printf("\nTimer Status byte: 0x%x \n", conf);


	//check counter selected
	if(conf & BIT(7))
		printf("Selected Counter: 2\n");
	else
		if(conf & BIT(6))
			printf("Selected Counter: 1\n");
		else
			printf("Selected Counter: 0\n");


	//check type of access
	if(!(conf & BIT(5)))
		printf("Type of Access: LSB\n");
	else
		if(conf & BIT(4))
			printf("Type of Access: LSB followed by MSB\n");
		else
			printf("Type of Access: MSB\n");


	//check operating mode
	if((conf & BIT(1))){
		if(conf & BIT(2))
		    printf("Operating Mode 3 - SQUARE WAVE\n");
		else
			if(conf & BIT(3))
				printf("Operating Mode 5 - HARDWARE TRIGGERED STROBE (RETRIGGERABLE)\n");
			else
				printf("Operating Mode 1 - HARDWARE RETRIGGERABLE ONE-SHOT\n");

	}else{
		if(conf & BIT(2))
			printf("Operating Mode 2 - RATE GENERATOR\n");
		else
			if(conf & BIT(3))
				printf("Operating Mode 4 - SOFTWARE TRIGGERED STROBE\n");
			else
				printf("Operating Mode 0 - INTERRUPT ON TERMINAL COUNT\n");
	}


	//check counting mode
	if(conf & BIT(0))
		printf("Counting Mode: BCD\n\n");
	else
		printf("Counting Mode: Binary\n\n");

	return 0;
}

int timer_test_square(unsigned long freq) {
	unsigned int timer = 0;
	timer_set_square(timer,freq);
	return 0;
}

int timer_test_int(unsigned long time) {

	int ipc_status, r, irq_set;
	message msg;
	irq_set = timer_subscribe_int();

	while(counter < time * TIMER0_DEFAULT_FREQ){

		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ){
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)){ /* received notification */

			switch (_ENDPOINT_P(msg.m_source)){

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set){
					timer_int_handler();
					if((counter % TIMER0_DEFAULT_FREQ) == 0)
						printf("Second elapsed\n");
				}
				break;
			}
		}

	}

	timer_unsubscribe_int();
	printf("achieved here\n");
	
	return 0;
}

int timer_test_config(unsigned long timer) {
	
	unsigned char config;

	if(timer_get_conf(timer,&config) != 0)
		return 1;

	if(timer_display_conf(config) != 0){
		printf("Error displaying timer configuration\n\n");
		return 1;
	}

	return 0;
}
