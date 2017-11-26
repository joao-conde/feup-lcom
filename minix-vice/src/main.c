#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
	printf("Start of MINIX-VICE!\n");

	srand(time(NULL));
	sef_startup();

	vg_init(0x105);
	drawSquare(0,0,10,10);
	sleep(5);
	vg_exit();
	return 0;
}
