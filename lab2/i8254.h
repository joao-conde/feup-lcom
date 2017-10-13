#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer
 */

#define MAX_TIMER_FREQ  1193182    /**< @brief max clock frequency for timer  */
#define TIMER0_DEFAULT_FREQ 60	   /**< @brief default clock frequency for timer */

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_1			0x41 /**< @brief Timer 1 count register */
#define TIMER_2			0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */


/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0		0x00    		 /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1		BIT(6)   		 /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2		BIT(7)   		 /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD	(BIT(7)|BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB		BIT(4)  				/**< @brief Initialize Counter LSB only */
#define TIMER_MSB		BIT(5) 	 				/**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB	(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */


/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_         BIT(5)
#define TIMER_RB_STATUS_        BIT(4)
#define TIMER_RB_SEL(n)         BIT((n)+1)


/* ERROR CONTROL  */

#define INVALID_ARGS  			-1  /**< @brief Invalid arguments */
#define FAIL_WRITE_CW			-2  /**< @brief Error writing control word */
#define FAIL_WRITE_LSB			-3	/**< @brief Error writing LSB */
#define FAIL_WRITE_MSB			-4	/**< @brief Error writing MSB */
#define FAIL_READ_CONF			-5  /**< @brief Error reading config of timer */
#define FAIL_SET_POLICY			-6	/**< @brief Error setting policy */
#define FAIL_ENABLE_IRQ			-7	/**< @brief Error enabling IRQ line */
#define FAIL_REMOVE_POLICY		-8	/**< @brief Error removing policy */
#define FAIL_DISABLE_IRQ		 -9	/**< @brief Error disabling IRQ line */
#define FAIL_SUB_INT			-10 /**< @brief Error subscribing interrupts */
#define FAIL_UNSUB_INT			-11 /**< @brief Error unsubscribing interrupts */
#define FAIL_SET_FREQ			-12 /**< @brief Error setting frequency  */
#define FAIL_GET_CONF			-13 /**< @brief Error getting timer configuraton */
#define FAIL_DISPLAY_CONF		-14 /**< @brief Error displaying timer configuration */

/* CLEAR MACROS */
#define CLEAR_LSB 		0xF0  /**< @brief Preserve MSB and reset LSB*/
#define CLEAR_MSB		0x0F  /**< @brief Preserve LSB and reset MSB*/


/**@}*/

#endif /* _LCOM_I8254_H */
