#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>

#include "i8254.h"
#include "timer.h"
#include "video_test.h"

unsigned int counter = 0;
int hookID;


int timer_set_frequency(unsigned char timer, unsigned long freq) {

	//checking if timer is in range 0, 1 or 2
	if (timer < 0 || timer > 2) {
		printf("Timer %lu does not exist. Select a timer from 0 to 2\n\n",
				timer);
		return INVALID_ARGS;
	}

	//checking if desired frequency to be set does not exceed max frequency
	if (freq > MAX_TIMER_FREQ) {
		printf("Invalid frequency - HIGHER THAN MAX FREQUENCY\n");
		return INVALID_ARGS;
	}

	unsigned long controlW, newFreq;
	unsigned char status;

	newFreq = MAX_TIMER_FREQ / freq; //calculation of new frequency

	//reading timer configuration to maintain last 4 bits
	timer_get_conf(timer, &status);
	controlW = (unsigned long) status;

	controlW &= CLEAR_MSB; //clear 4 most significative bits

	//depending on the timer builds the apropriate control word to be writen
	switch (timer) {

	case 0:
		controlW |= TIMER_SEL0 | TIMER_LSB_MSB;

		//writes control word to timer control
		if (sys_outb(TIMER_CTRL, controlW) != OK) {
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		//writes frequency LSB to timer
		if (sys_outb(TIMER_0, newFreq) != OK) {
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		//writes frequency MSB to timer
		if (sys_outb(TIMER_0, (newFreq >> 8)) != OK) {
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	case 1:
		controlW |= TIMER_SEL1 | TIMER_LSB_MSB;

		if (sys_outb(TIMER_CTRL, controlW) != OK) {
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		if (sys_outb(TIMER_1, newFreq) != OK) {
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		if (sys_outb(TIMER_1, (newFreq >> 8)) != OK) {
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	case 2:
		controlW |= TIMER_SEL2 | TIMER_LSB_MSB;

		if (sys_outb(TIMER_CTRL, controlW) != OK) {
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		if (sys_outb(TIMER_2, newFreq) != OK) {
			printf("Error writing LSB\n");
			return FAIL_WRITE_LSB;
		}

		if (sys_outb(TIMER_2, (newFreq >> 8)) != OK) {
			printf("Error writing MSB\n");
			return FAIL_WRITE_MSB;
		}

		break;

	}

	return OK;
}

int timer_subscribe_int(void) {

	hookID = TIMER_BIT_ORDER;

	/*
	 * Subscribes a notification on every interrupt in timer 0 irq_line
	 * hook_id argument is used both for input to the call and output from the call, used in other calls
	 */
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) != OK) {
		printf("Failure setting policy\n");
		return FAIL_SET_POLICY;
	}

	//enables interrupts on the timer 0 IRQ line
	if (sys_irqenable(&hookID) != OK) {
		printf("Failure enabling IRQ line\n");
		return FAIL_ENABLE_IRQ;
	}

	return BIT(TIMER_BIT_ORDER);
}

int timer_unsubscribe_int() {

	/* Reverse order of function calls
	 * First disable then unsub
	 * Unlike subbing and then enabling
	 */

	//disables interrupts on the timer 0 IRQ line
	if (sys_irqdisable(&hookID) != OK) {
		printf("Failure disabling IRQ line\n");
		return FAIL_DISABLE_IRQ;
	}

	//Unsubscribes notifications in timer 0 irq_line
	if (sys_irqrmpolicy(&hookID) != OK) {
		printf("Failure removing policy\n");
		return FAIL_REMOVE_POLICY;
	}

	return OK;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	unsigned long controlWord = 0, config = 0;

	//checking if timer is in range 0, 1 or 2
	if (timer >= 0 && timer <= 2) {

		//building read-back command (depending on the timer) to be written to timer
		controlWord |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

		//writing read-back command to the control register
		if (sys_outb(TIMER_CTRL, controlWord) != OK) {
			printf("Error writing control word\n");
			return FAIL_WRITE_CW;
		}

		//reading desired timer configuration from count register
		if (sys_inb(TIMER_0 + timer, &config) != OK) {
			printf("Error reading config of timer %d\n", timer);
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
	if (!(conf & BIT(5)))
		printf("Type of Access: LSB\n");
	else if (conf & BIT(4))
		printf("Type of Access: LSB followed by MSB\n");
	else
		printf("Type of Access: MSB\n");

	//check operating mode
	if ((conf & BIT(1))) {
		if (conf & BIT(2))
			printf("Operating Mode 3 - SQUARE WAVE\n");
		else if (conf & BIT(3))
			printf(
					"Operating Mode 5 - HARDWARE TRIGGERED STROBE (RETRIGGERABLE)\n");
		else
			printf("Operating Mode 1 - HARDWARE RETRIGGERABLE ONE-SHOT\n");

	} else {
		if (conf & BIT(2))
			printf("Operating Mode 2 - RATE GENERATOR\n");
		else if (conf & BIT(3))
			printf("Operating Mode 4 - SOFTWARE TRIGGERED STROBE\n");
		else
			printf("Operating Mode 0 - INTERRUPT ON TERMINAL COUNT\n");
	}

	//check counting mode
	if (conf & BIT(0))
		printf("Counting Mode: BCD\n\n");
	else
		printf("Counting Mode: Binary\n\n");

	return OK;
}

int timer_test_int(unsigned long time) {

	int ipc_status, r, irq_set, nmessages = 0;
	message msg;

	if ((irq_set = timer_subscribe_int()) == -1)
		return FAIL_SUB_INT;

	while (counter < time * TIMER0_DEFAULT_FREQ) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set) {
					timer_int_handler();
					if ((counter % TIMER0_DEFAULT_FREQ) == OK){
						printf("Hello! One more second elapsed.\n");
						nmessages++;
					}
				}
				break;
			}
		}
	}

	printf("%d messages have been written.\n",nmessages);

	if (timer_unsubscribe_int() != OK)
		return FAIL_UNSUB_INT;

	return OK;
}

