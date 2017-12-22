#ifndef KBD_H
#define KBD_H

#include "i8042.h"
#include "i8254.h"


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
 * Tests whether or not there is something to read and if the read was successful or not
 *
 * @return Return the byte read or negative value on failure
 */
int kbc_read();



#endif /* KBD_H */
