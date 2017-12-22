#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"

int hookID;

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
