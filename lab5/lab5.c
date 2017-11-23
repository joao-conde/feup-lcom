#include "test5.h"
#include "vbe.h"
#include "pixmap.h"

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
					"\t service run %s -args \"test_line <first endpoint x - xi> <first endpoint y - yi> <last endpoint x - xf> <last endpoint y - yf> <line color - color>\"\n"
					"\t service run %s -args \"test_xpm <array with xpm - xpm> <horizontal coordinate of upper-left corner - xi> <vertical coordinate of upper-left corner - yi>\"\n"
					"\t service run %s -args \"test_move <array with xpm - xpm> <initial horizontal coordinate of upper-left corner - xi> <initial vertical coordinate of upper-left corner - yi> <final horizontal coordinate of upper-left corner - xf> <final vertical coordinate of upper-left corner - yf> <speed of movement - s> <frame-rate in frames/second - f>\"\n"
					"\t service run %s -args \"test_controller\"\n", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {

	unsigned short mode, delay, x, y, size, x1, y1, frame_rate;
	short speed;
	unsigned long color;


	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {

		if (argc != 4) {
			printf("vbe: wrong no. of arguments for video_test_init()\n");
			return 1;
		}

		mode = parse_ulong(argv[2], 16);
		delay = parse_ulong(argv[3], 10);

		printf("vbe::video_test_init(0x%x,%lu)\n", mode, delay);
		return video_test_init(mode, delay);
	}

	else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {

		if (argc != 6) {
			printf("vbe: wrong no. of arguments for video_test_square()\n");
			return 1;
		}

		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		size = parse_ulong(argv[4], 10);
		color = parse_ulong(argv[5], 10);

		printf("vbe::video_test_square(%lu,%lu,%lu,%lu)\n", x, y, size, color);

		return video_test_square(x, y, size, color);
	}

	else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {

		if (argc != 7) {
			printf("vbe: wrong no. of arguments for video_test_line()\n");
			return 1;
		}

		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		x1 = parse_ulong(argv[4], 10);
		y1 = parse_ulong(argv[5], 10);
		color = parse_ulong(argv[6], 10);

		printf("vbe::video_test_line(%lu,%lu,%lu,%lu,%lu)\n", x, y, x1, y1,
				color);

		return video_test_line(x, y, x1, y1, color);
	}

	else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {

		if (argc != 5) {
			printf("vbe: wrong no. of arguments for test_xpm()\n");
			return 1;
		}

		x = parse_ulong(argv[3], 10);
		y = parse_ulong(argv[4], 10);

		if ((strncmp(argv[2], "pic1", strlen("pic1")) == 0))
			test_xpm(pic1, x, y);

		else if ((strncmp(argv[2], "pic2", strlen("pic2")) == 0))
			test_xpm(pic2, x, y);

		else if ((strncmp(argv[2], "pic3", strlen("pic3")) == 0))
			test_xpm(pic3, x, y);

		else if ((strncmp(argv[2], "cross", strlen("cross")) == 0))
			test_xpm(cross, x, y);

		else if ((strncmp(argv[2], "penguin", strlen("penguin")) == 0))
			test_xpm(penguin, x, y);

		else {
			printf("%s is not available\n", argv[4]);
			return 1;
		}

		printf("vbe::test_xpm(%s, %lu, %lu)\n", argv[2], x, y);
	}

	else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {

			if (argc != 9) {
				printf("vbe: wrong no. of arguments for test_move()\n");
				return 1;
			}

			x = parse_ulong(argv[3], 10);
			y = parse_ulong(argv[4], 10);
			x1 = parse_ulong(argv[5], 10);
			y1 = parse_ulong(argv[6], 10);

			speed = strtol(argv[7], NULL, 10);
			frame_rate = parse_ulong(argv[8], 10);

			if ((strncmp(argv[2], "pic1", strlen("pic1")) == 0))
				test_move(pic1, x, y, x1, y1, speed, frame_rate);

			else if ((strncmp(argv[2], "pic2", strlen("pic2")) == 0))
				test_move(pic2, x, y, x1, y1, speed, frame_rate);

			else if ((strncmp(argv[2], "pic3", strlen("pic3")) == 0))
				test_move(pic3, x, y, x1, y1, speed, frame_rate);

			else if ((strncmp(argv[2], "cross", strlen("cross")) == 0))
				test_move(cross, x, y, x1, y1, speed, frame_rate);

			else if ((strncmp(argv[2], "penguin", strlen("penguin")) == 0))
				test_move(penguin, x, y, x1, y1, speed, frame_rate);

			else {
				printf("%s is not available\n", argv[4]);
				return 1;
			}

			printf("vbe::test_move(%s, %lu, %lu, %lu, %lu, %d, %lu)\n", argv[2], x, y, x1, y1, speed, frame_rate);
		}

	else if (strncmp(argv[1], "test_controller", strlen("test_controller")) == 0) {

		if (argc != 2) {
			printf("vbe: wrong no. of arguments for test_controller()\n");
			return 1;
		}

		printf("vbe::test_controller()\n");

		return test_controller();
	}

	else {
		printf("vbe: %s - no valid function!\n", argv[1]);
		return 1;
	}

	return 1;
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
