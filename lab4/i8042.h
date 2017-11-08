#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_



/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC
 */


#define BIT(n) (0x01<<(n))


/* USEFUL MACROS */

#define ESC_BREAK 		0x81
#define SET2_SCANCODE	0xE0
#define OUTBUF_FULL_ASS 1
#define BREAKCODE		BIT(7)
#define KBD_IRQ			1
#define MOUSE_IRQ		12
#define PACKET_SIZE     3
#define RB_DOWN         BIT(1)

/* MOUSE COMMANDS */

#define WRITE_BYTE  0xD4
#define ENABLE_DATAREPORT 0xF4
#define ENABLE_MOUSE 0xA8
#define DISABLE_DATAREPORT 0xF5
#define ENABLE_REMOTE	0xF0
#define ENABLE_STREAM	0xEA

/* I/O port addresses */

#define STAT_REG 0x64
#define OUT_BUF  0x60
#define INP_BUF 0x60


/* ERROR CONTROL  */


#define FAIL_READ_STATUS		-15  /**< @brief Error reading status register */
#define FAIL_READ_OUTBUF		-16  /**< @brief Error reading output buffer */
#define TRIES_EXCEED			-17 /**< @brief Maximum number of tries exceeded */
#define ERROR_STATUS			-18 /**< @brief Status byte read with errors */


/* KBC CONTROL*/

#define KBD_BIT_ORDER 25
#define MOUSE_BIT_ORDER 13

/* BIT MASKS for status control word */

#define OBF BIT(0)
#define IBF BIT(1)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)


#define FIRSTBYTE    BIT(3)
#define TRUE 1
#define FALSE 0


#define DELAY_US   20000
#define MS_TO_MICRO 1000


/**@}*/

#endif /* _LCOM_I8042_H */
