#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC
 */


#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1    /**< @brief keyboard IRQ line */

/* USEFUL MACROS */

#define ESC_BREAK 0x81

#define ACK   	0xFA
#define RESEND	0xFE
#define ERROR	0xFC


/* I/O port addresses */

#define STAT_REG 0x64
#define OUT_BUF  0x60
#define INP_BUF  0x60
#define KBC_CMD_REG 0x64



/* KBC CONTROL*/

#define KBD_BIT_ORDER 25


/* BIT MASKS for status control word */

#define OBF BIT(0)
#define IBF BIT(1)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)


/* COMMANDS */

#define LED_TOGGLE_CMD	0xED



#define DELAY_US   20000


/**@}*/

#endif /* _LCOM_I8042_H */
