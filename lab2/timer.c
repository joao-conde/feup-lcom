#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"


unsigned int counter = 0;
int hookID;


int timer_set_frequency(unsigned char timer, unsigned long freq) {


	if(timer < 0 || timer > 2){
		printf("Timer %lu does not exist. Select a timer from 0 to 2\n\n",timer);
		return INVALID_ARGS;
	}


	if(freq > MAX_TIMER_FREQ){
		printf("Invalid frequency - HIGHER THAN MAX FREQUENCY\n");
		return INVALID_ARGS;
	}


	unsigned long controlW, newFreq;
	unsigned char status;

	newFreq = MAX_TIMER_FREQ / freq;

	//reading timer configuration to maintain last 4 bits
	timer_get_conf(timer, &status);
	controlW = (unsigned long)status;

	controlW &= CLEAR_MSB;

	switch(timer){

	case 0:
		controlW |= TIMER_SEL0 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		if(sys_outb(TIMER_0,newFreq) != OK){
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		if(sys_outb(TIMER_0,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	case 1:
		controlW |= TIMER_SEL1 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		if(sys_outb(TIMER_1,newFreq) != OK){
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		if(sys_outb(TIMER_1,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	case 2:
		controlW |= TIMER_SEL2 | TIMER_LSB_MSB;

		if(sys_outb(TIMER_CTRL, controlW) != OK){
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		if(sys_outb(TIMER_2,newFreq) != OK){
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		if(sys_outb(TIMER_2,(newFreq >> 8)) != OK){
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	}

	return OK;
}

int timer_subscribe_int(void) {

	hookID = TIMER0_IRQ;

	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hookID) != OK){
		printf("Failure setting policy\n");
		return FAIL_SET_POLICY;
	}

	if(sys_irqenable(&hookID) != OK){
		printf("Failure enabling IRQ line\n");
		return FAIL_ENABLE_IRQ;
	}

	return BIT(TIMER0_IRQ);
}

int timer_unsubscribe_int() {


	if(sys_irqdisable(&hookID) != OK){
		printf("Failure disabling IRQ line\n");
		return FAIL_DISABLE_IRQ;
	}

	if(sys_irqrmpolicy(&hookID)!= OK){
		printf("Failure removing policy\n");
		return FAIL_REMOVE_POLICY;
	}

	return OK;
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

                if(sys_outb(TIMER_CTRL, controlWord) != OK){
                	printf("Error writing control word\n");
                	return FAIL_WRITE_CW;
                }

                if(sys_inb(TIMER_0 + timer, &config) != OK){
                	printf("Error reading config of timer %d\n",timer);
                	return FAIL_READ_CONF;
                }

                *st = (unsigned char) config;

                return OK;

        }

        return INVALID_ARGS;

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

	return OK;
}

int timer_test_time_base(unsigned long freq) {
	unsigned int timer = 0;

	if(timer_set_frequency(timer,freq) != OK)
		return FAIL_SET_FREQ;
    else
    	printf("Frequency changed successfully\n");

	return OK;
}

int timer_test_int(unsigned long time) {

	int ipc_status, r, irq_set;
	message msg;

	if((irq_set = timer_subscribe_int()) == -1)
		return FAIL_SUB_INT;

	while(counter < time * TIMER0_DEFAULT_FREQ){

		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK){
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)){ /* received notification */

			switch (_ENDPOINT_P(msg.m_source)){

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set){
					timer_int_handler();
					if((counter % TIMER0_DEFAULT_FREQ) == OK)
						printf("One second elapsed\n");
				}
				break;
			}
		}

	}

	if(timer_unsubscribe_int() != OK)
		return FAIL_UNSUB_INT;
	
	return OK;
}

int timer_test_config(unsigned char timer) {
	
	unsigned char config;

	if(timer_get_conf(timer,&config) != OK)
		return FAIL_GET_CONF;

	if(timer_display_conf(config) != OK){
		printf("Error displaying timer configuration\n\n");
		return FAIL_DISPLAY_CONF;
	}

	return OK;
}
