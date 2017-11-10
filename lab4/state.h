#include "i8042.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * STATE MACHINE TO WORK IN MOUSE_TEST_GESTURE()
 */

typedef enum {
	INITIAL, DRAWING, COMPLETE
} state_t;

typedef enum {
	RDOWN, RUP, VERT_LINE, HOR_TOLERANCE, INVALID
} state_data_t;

state_t state = INITIAL;

void processMouseInput(state_data_t input) {

	if(state == INVALID)
		return;

	switch (state) {
	case INITIAL:

		if (input == RDOWN)
			state = DRAWING;

		break;

	case DRAWING:

		if (input == VERT_LINE)
			state = COMPLETE;

		if (input == RUP)
			state = INITIAL;

		if (input == HOR_TOLERANCE)
			state = DRAWING;

		break;

	case COMPLETE:
		break;
	}
}


state_data_t validateMouseMovement(unsigned long* packet, short length){

	int x,y;

	x = *((int*)packet+1);
	y = *((int*)packet+2);

	//from a negative number in 2's complement to regular decimal
	if (BIT(4) & *packet) {
		x = (*(packet + 1) ^= BYTE_MINUS1) + 1;
		x= x - 256;
	}

	if (BIT(5) & *packet) {
		y= (*(packet + 2) ^= BYTE_MINUS1) + 1;
		y= y - 256;
	}

	//both negative or both positive = positive slope
	if((x < 0 && y < 0) || (x > 0 && y > 0)){

		x = abs(x);
		y = abs(y);

		if(x >= length)
			return VERT_LINE;
	}

	return HOR_TOLERANCE;
}

state_data_t analyzePacket(unsigned long* packet, short length) {

	switch (state) {
	case INITIAL:

		if((*packet) & RB_DOWN)
			return RDOWN;

		break;

	case DRAWING:

		if(!((*packet) & RB_DOWN))
			return RUP;

		/*CHECK MOUSE MOVEMENT TO SEE IF IT IS A VALID VERTICAL LINE OR NOT*/
		return validateMouseMovement(packet,length);

		break;

	case COMPLETE:
		break;
	}

	return INVALID;
}

