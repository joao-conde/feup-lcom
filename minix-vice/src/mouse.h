#ifndef MOUSE_H
#define MOUSE_H

#include "bitmap.h"

typedef struct {
    int x, y;
    int deltaX, deltaY;

    unsigned long packet[3];
    unsigned int packet_index;
    int mouse_packets_synched;

    int LBtnDown;
    int MBtnDown;
    int RBtnDown;

    int draw;

    Bitmap* bitmap;

} Mouse;


Mouse* newMouse();
Mouse* getMouse();

void deleteMouse();

void drawMouse();

void updateMouse();


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

int kbc_write(unsigned long port, unsigned long word);

int mouse_write_cmd(unsigned long cmd, unsigned long word);

void synch_packet(long byte);

long mouse_readOBF();

void mouseIntHandler();

void display_packet(unsigned long *packet);

int enable_DataReporting();

int enable_mouse();

int setRemoteMode();

int setStreamMode();

int cleanOBF();

int disable_DataReporting();



#endif
