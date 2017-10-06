#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"


unsigned int counter = 0;
int hookID;


int timer_set_frequency(unsigned char timer, unsigned long freq) {


	if(timer < 0 || timer > 2){
		printf("Timer %lu does not exist. Select a timer from 0 to 2\n\n",timer);
		return -1;
	}


	if(freq < 0){
		printf("Invalid frequency - MUST NOT BE NEGATIVE\n");
		return -1;
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
			return -1;
		}

		if(sys_outb(TIMER_0,newFreq) != OK){
			printf("Error writing LSB\n");
			return -1;
		}

		if(sys_outb(TIMER_0,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return -1;
		}

		break;

	case 1:
		controlW |= TIMER_SEL1 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return -1;
		}

		if(sys_outb(TIMER_1,newFreq) != OK){
			printf("Error writing LSB\n");
			return -1;
		}

		if(sys_outb(TIMER_1,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return -1;
		}

		break;

	case 2:
		controlW |= TIMER_SEL2 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return -1;
		}

		if(sys_outb(TIMER_2,newFreq) != OK){
			printf("Error writing LSB\n");
			return -1;
		}

		if(sys_outb(TIMER_2,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return -1;
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

int timer_get_conf(unsigned char timer, unsigned char *st)

{

        unsigned long controlWord = 0;

        unsigned long config = 0;

        if (timer >= 0 && timer <= 2)

        {

                controlWord |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

                sys_outb(TIMER_CTRL, controlWord);

                sys_inb(TIMER_0 + timer, &config);

                *st = (unsigned char) config;

                return 0;

        }

        return -1;

}

int timer_display_conf(unsigned char conf) {


	printf("\nTimer Status byte: 0x%x \n", conf);


	//check output value
    if (BIT(7) & conf)

            printf("Output Value: 1\n");

    else

            printf("Output Value: 0\n");

    //check null count
    if (BIT(6) & conf)

            printf("Null count: 1\n");

    else

            printf("Null count: 0\n");


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

int timer_test_time_base(unsigned long freq) {
	unsigned int timer = 0;

	if(timer_set_frequency(timer,freq) != 0)
		return -1;
    else
    	printf("Frequency changed successfully\n");

	return 0;
}

int timer_test_int(unsigned long time) {

	int ipc_status, r, irq_set;
	message msg;

	if((irq_set = timer_subscribe_int()) == -1)
		return -1;

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
						printf("One second elapsed\n");
				}
				break;
			}
		}

	}

	if(timer_unsubscribe_int() != 0)
		return -1;
	
	return 0;
}

int timer_test_config(unsigned char timer) {
	
	unsigned char config;

	if(timer_get_conf(timer,&config) != 0)
		return -1;

	if(timer_display_conf(config) != 0){
		printf("Error displaying timer configuration\n\n");
		return -1;
	}

	return 0;
}
