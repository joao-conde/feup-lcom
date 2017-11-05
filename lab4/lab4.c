#include "test4.h"

#include <stdlib.h>
#include <stdio.h>
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
					"\t service run %s -args \"test_packet <brief explanation - arg>\"\n"
					"\t service run %s -args \"test_async <brief explanation - arg>\"\n"
					"\t service run %s -args \"test_remote <brief explanation - arg>\"\n"
					"\t service run %s -args \"test_gesture <brief explanation - arg>\"\n",
			argv[0],argv[0],argv[0],argv[0]);
}

static int proc_args(int argc, char **argv) {

	unsigned int npackets, idle_time, period;
	int length;

	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {

		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}

		if ((npackets = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("mouse::mouse_test_packet(%lu)\n", npackets);

		return mouse_test_packet(npackets);
	}

	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {

		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_async()\n");
			return 1;
		}


		if ((idle_time = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;

		printf("mouse::mouse_test_async(%lu)\n", idle_time);

		return mouse_test_async(idle_time);
	}

	else if (strncmp(argv[1], "test_remote", strlen("test_remote")) == 0) {

		if (argc != 4) {
			printf("mouse: wrong no. of arguments for mouse_test_remote()\n");
			return 1;
		}

		if ((period = parse_ulong(argv[2], 10)) == ULONG_MAX ||	(npackets = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;

		printf("mouse::mouse_test_remote(%lu,%lu)\n", period, npackets);

		return mouse_test_remote(period,npackets);
	}

	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {

			if (argc != 3) {
				printf("mouse: wrong no. of arguments for mouse_test_gesture()\n");
				return 1;
			}

			length = strtol(argv[2], NULL, 10);

			printf("mouse::mouse_test_gesture(%d)\n", length);

			return mouse_test_gesture(length);
		}

	else {
		printf("mouse: %s - no valid function!\n", argv[1]);
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
