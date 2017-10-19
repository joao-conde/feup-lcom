#ifndef KBD_H
#define KBD_H

#include "i8042.h"

/** @defgroup kbd kbd
 * @{
 *
 * Functions interacting with the i8042 KBC
 */


/**
 * @brief Subscribes and enables KBC interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe_int(void);


/**
 * @brief Unsubscribes KBC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int(void);


/**
 * @brief Reads the KBC output buffer
 *
 * Tests whether or not there is something to read and if the read was sucessful or not
 *
 * @return Return the byte read or negative value on failure
 */
int kbc_read();


/**
 * @brief Writes a control word to one of the KBC different port's checking status of KBC first
 *
 * @param port the port where the word is written
 * @param word word written to the said port
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_write(unsigned long port, unsigned long word);


/**
 * @brief Writes a command to the KBC followed by the said word to KBC input buffer
 *
 * @param cmd command written to the kbc
 * @param word word written to the input buffer of KBC
 *
 * @return Return 0 on success and non-zero otherwise
 */
int kbc_write_cmd(unsigned long cmd, unsigned long word);

int kbc_get_sysinbcalls();

/**
 * @brief Prints a key scancode in an human friendly way
 *
 * @param scancode the scancode to be printed
 */
void print_scancode(unsigned long scancode);

void print_set1code(unsigned long scancode);

void print_set2code(unsigned long scancode);



#endif /* KBD_H */
