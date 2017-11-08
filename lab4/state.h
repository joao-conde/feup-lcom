#include "i8042.h"

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

	//from a negative number in 2's complement to regular decimal
	if (BIT(7) & *(packet + 1)) {
		*(packet + 1) = *(packet + 1) ^ 255;
		*(packet + 1) = -*(packet + 1);
	}

	if (BIT(7) & *(packet + 2)) {
		*(packet + 2) = *(packet + 2) ^ 255;
		*(packet + 2) = -*(packet + 2);
	}

	if((*(packet+1)) >= length || (*(packet+1)) <= -length)
		if((*(packet+2)) > 0)
			return VERT_LINE;

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

