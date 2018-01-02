#ifndef MOUSE_H
#define MOUSE_H

#include "bitmap.h"
#include "state_machines.h"

/** @defgroup mouse mouse
 * @{
 *
 * Game mouse. Singleton implementation.
 * Functions interacting with the i8042 KBC for mouse.
 */


typedef struct {
    int x, y;
    int deltaX, deltaY;

    int LBtnDown;
    int RBtnDown;

    Bitmap* cursor;
    Bitmap* target;

} Mouse;



/**
 * @brief Creates a new mouse pointer
 *
 * @return a pointer to the mouse created
 */
Mouse* newMouse();


/**
 * @brief Returns the existent mouse pointer or creates a new one if the first does not exist already.
 * Ensures singleton design pattern.
 *
 * @return a pointer to the existent mouse
 */
Mouse* getMouse();


/**
 * @brief Deletes the mouse pointer
 */
void deleteMouse();


/**
 * @brief Draws the mouse pointer
 */
void drawMouse();


/**
 * @brief Updates the mouse pointer
 */
void updateMouse();


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


/**
 * @brief Writes to the KBC port specified the word given.
 * Checks the status register and the input buffer.
 *
 * @param port the port to write to
 * @param word byte to write
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_write(unsigned long port, unsigned long word);


/**
 * @brief Sends a command to the mouse
 *
 * @param cmd the mouse command
 * @param word the mouse command argument
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_write_cmd(unsigned long cmd, unsigned long word);


/**
 * @brief Synch the mouse packets
 *
 * @param byte one mouse packet
 */
void synch_packet(long byte);

/**
 * @brief Reads from the output buffer
 *
 * @return the output buffer byte upon success and non-zero otherwise
 */
long mouse_readOBF();


/**
 * @brief Mouse IH
 */
void mouseIntHandler();


/**
 * @brief Enables data reporting
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int enable_DataReporting();


/**
 * @brief Enables the mouse
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int enable_mouse();



/**
 * @brief Set mouse stream mode
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int setStreamMode();


/**
 * @brief Cleans the output buffer
 *
 * @return the output buffer byte upon success and non-zero otherwise
 *
 */
int cleanOBF();

#endif
