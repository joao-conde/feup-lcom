int kbd_test_scan(unsigned short ass) {
	printf("Hi I am test_scan\n");
}

int kbd_test_leds(unsigned short size, unsigned short *toggle) {

	printf("Size parameter: %d\n", size);
	int i = 0;
	while (i < size) {
		printf("Element %d: %u\n", i + 1, *(toggle + i));
		i++;
	}
	int ipc_status, r, kbd_irq_set, timer_irq_set, counter = 0;
	message msg;


	kbd_irq_set = kbd_subscribe_int();
	timer_irq_set = timer_subscribe_int();

	if (kbd_irq_set == -1) {
		printf("kbd_subscribe_int(): Failure\n");
		return -1;
	}

	if (timer_irq_set == -1) {
		printf("timer_subscribe_int(): Failure\n");
		return -1;
	}

	while (1) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & timer_irq_set){
					counter++;
				}

				if (msg.NOTIFY_ARG & kbd_irq_set) {
					kbc_toogle_led(2);
				}
				break;

			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	if (timer_unsubscribe_int() == -1) {
		printf("timer_unsubscribe_int(): Failure\n");
		return -1;
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("kbd_unsubscribe_int(): Failure\n");
		return -1;
	}



	return 0;

}
int kbd_test_timed_scan(unsigned short n) {
	printf("Hi I am test_timed_scan\n");
}
