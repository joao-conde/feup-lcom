#ifndef MOUSE_H
#define MOUSE_H




/** @defgroup mouse mouse
 * @{
 *
 * Functions interacting with the i8042 KBC for mouse
 */

/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int(void);


/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int(void);

void read_packet();

int kbc_write(unsigned long port, unsigned long word);

int kbc_write_cmd(unsigned long cmd, unsigned long word);

void synch_packet(long byte);

long mouse_readOBF();

void mouseIH();

void display_packet(long *packet);


#endif
