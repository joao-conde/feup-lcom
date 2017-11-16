#include "test5.h"
#include "vbe.h"

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
					"\t service run %s -args \"test_init <16-bit VBE mode to set - mode> <seconds after which returns to text mode - delay>\"\n"
					"\t service run %s -args \"test_square <square center x - x> <square center y - y> <size of each side in pixels - size> <pixel color - color>\"\n"
					"\t service run %s -args \"test_line <first endpoint x - xi> <first endpoint y - yi> <last endpoint x - xf> <last endpoint y - yf> <line color - color>\"\n",
			argv[0],argv[0],argv[0]);
}

static int proc_args(int argc, char **argv) {

	unsigned short mode, delay, x, y, size, x1, y1;
	unsigned long color;

	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {

		if (argc != 4) {
			printf("vbe: wrong no. of arguments for video_test_init()\n");
			return 1;
		}

		mode = parse_ulong(argv[2],16);
		delay = parse_ulong(argv[3],10);

		printf("vbe::video_test_init(0x%x,%lu)\n", mode, delay);
		return video_test_init(mode, delay);
	}

	else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {

		if (argc != 6) {
			printf("vbe: wrong no. of arguments for video_test_square()\n");
			return 1;
		}

		x = *argv[2];
		y = *argv[3];
		size = *argv[4];
		color = parse_ulong(argv[5],10);

		printf("vbe::video_test_square(%hu,%hu,%hu,%lu)\n", x, y, size, color);

		return video_test_square(x, y, size, color);
	}

	else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {

		if (argc != 7) {
			printf("vbe: wrong no. of arguments for video_test_line()\n");
			return 1;
		}

		x = *argv[2];
		y = *argv[3];
		x1 = *argv[4];
		y1 = *argv[5];
		color = parse_ulong(argv[6],10);


		printf("vbe::video_test_line(%hu,%hu,%hu,%hu,%lu)\n", x, y, x1, y1, color);

		return video_test_line(x, y, x1, y1, color);
	}

	else {
		printf("vbe: %s - no valid function!\n", argv[1]);
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
