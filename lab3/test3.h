#ifndef __TEST3_H
#define __TEST3_H

/** @defgroup test3 test3
 * @{
 *
 * Functions for testing the i8042 KBC
 */


/**
 * @brief Tests printing the scancodes (both make and break codes) from the KBC
 *
 * Each key pressed from keyboad will have it's makecode and breakcode printed upon press and release.
 * Function can be executed in C or assembly code
 *
 * @param assembly Execute assembly or C code
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_scan(unsigned short assembly);

/**
 * @brief Tests turning LED's on and off
 *
 * Process each element of the toggle array per second
 *
 * @param size Length of toggle array
 * @param *toggle Base of toggle array
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_leds(unsigned short size, unsigned short *toggle);

/**
 * @brief Test whether the program is able to handle interrupts from more than one device
 *
 * Similar to kbd_test_scan() in that it should print in the console the scancodes
 * Program should exit not only when the user releases the ESC key
 * but also if it does not receive a scancode for a number seconds equal to its argument, n.
 *
 * @param n Maximum number of seconds to wait if no code received
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_timed_scan(unsigned short n);

#endif /* __TEST3_H */
