#include "kbd.h"
#include "i8042.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);


int main(int argc, char **argv) {

	sef_startup();
	sys_enable_iop(SELF);

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"scan <IH written in assembly(1) or C(0) - ass>\"\n"
					"\t service run %s -args \"poll\"\n"
					"\t service run %s -args \"tscan <maximum waiting time in between scancodes - n>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned long assembly, time;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {

		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}

		if ((assembly = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		if (assembly != 1 && assembly != 0) {
			printf(
					"'ass' parameter must be 0 or 1, for C or assembly written code respectively (check function prototype)\n");
			return 1;
		}

		printf("kbd::kbd_test_scan(%lu)\n", assembly);

		return kbd_test_scan(assembly);
	} else if (strncmp(argv[1], "tscan", strlen("tscan")) == 0) {

		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_timed_scan()\n");
			return 1;
		}


		if ((time = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("kbd::kbd_test_timed_scan(%lu)\n", time);

		return kbd_test_timed_scan(time);
	} else if (strncmp(argv[1], "poll", strlen("poll")) == 0) {


		if (argc != 2) {
			printf("kbd: kbd_poll() has no arguments\n");
			return 1;
		}


		printf("kbd::kbd_test_poll()\n");
		return kbd_test_poll();

	} else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;
	}
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
