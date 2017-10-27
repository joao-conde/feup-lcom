#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv) {

	sef_startup();

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"test_packet <brief explanation - arg>\"\n",
			argv[0]);
}

static int proc_args(int argc, char **argv) {

	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {

		if (argc != 2) {
			printf("kbd: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}

		printf("mouse::mouse_test_packet()\n");

		return mouse_test_packet(1);
	}

	/*else if (strncmp(argv[1], "tscan", strlen("tscan")) == 0) {

		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_timed_scan()\n");
			return 1;
		}

		time = parse_ulong(argv[2], 10);

		if (time == ULONG_MAX)
			return 1;

		printf("kbd::kbd_test_timed_scan(%lu)\n", time);

		return kbd_test_timed_scan(time);
	} else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {

		array_length = parse_ulong(argv[2], 10);
		to_read = argc - 3;

		if (array_length <= 0) {
			printf(
					"kbd: Invalid array size for kbd_test_leds(). Must be positive value\n");
			return 1;
		}

		if (array_length != to_read) {
			printf(
					"kbd: Invalid no. of LED toggle's for kbd_test_leds(). Must match array size\n");
			return 1;
		}

		//reading the inputed values for LED toggle array
		unsigned short toggle[array_length];

		int i;
		for (i = 0; i < to_read; i++) {
			toggle[i] = (unsigned short) parse_ulong(argv[i + 3], 10);
		}

		//printing the size and array elements with function call
		printf("kbd:kbd_test_leds(%lu, {", array_length);

		for (i = 0; i < array_length; i++) {
			printf(" %u ", toggle[i]);
		}

		printf("})\n");

		return kbd_test_leds(array_length, toggle);
	} else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;
	}*/
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
