#ifndef __TEST4_H
#define __TEST4_H

/** @defgroup test4 test4
 * @{
 *
 * Functions for testing the kbd code
 */

/** 
 * @brief To test packet reception via interrupts 
 * 
 * Displays the packets received from the mouse
 * Exits after receiving the number of packets specified in argument
 * 
 * @param cnt Number of packets to receive and display before exiting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */

int mouse_test_packet(unsigned short cnt);


#endif /* __TEST_4.H */
