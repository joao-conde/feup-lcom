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

int mouse_write_cmd(unsigned long cmd, unsigned long word);

void synch_packet(long byte);

long mouse_readOBF();

void mouseIH();

void display_packet(unsigned long *packet);

void enable_DataReporting();

void enable_mouse();

long mouse_kbc_polling();

void setRemoteMode();

void setStreamMode();

unsigned long* create_remotePacket(unsigned long period);

void disable_DataReporting();


#endif
