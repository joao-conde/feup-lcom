#ifndef RTC_H
#define RTC_H

#define BIT(n) (0x01<<(n))


/* Access to RTC */

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


/* RTC’s Internal Address Space */

/* REGISTER */
#define REGA 10
#define REGB 11

/* INPUTS */
#define DAY   7
#define MONTH 8
#define YEAR  9

#define HOURS   4
#define MINUTES 2
#define SECONDS 0

/* FLAGS */
#define REGA_UIP BIT(7)
#define REGB_BIN BIT(2)


/* CURENT MILLENIUM  */
#define CURRENT_MILLENIUM 2000


/** @defgroup rtc rtc
 * @{
 *
 * Functions interacting with the RTC (Real-Time Clock).
 */


/**
 * @brief Checks if RTC is updating
 *
 * @return 1 if RTC updating, 0 otherwise
 */
int isRTCUpdating();


/**
 * @brief Checks if the data read from RTC is in BCD
 *
 * @return 1 if BCD, 0 if binary
 */
int isBCD();


/**
 * @brief Converts a byte in BCD to binary
 *
 * @param bcd a pointer to the byte in BCD form
 *
 * @return the binary form of the given BCD byte
 */
unsigned long BCDtoBin(unsigned long* bcd);


/**
 * @brief Reads date from RTC
 *
 * @param day a pointer to a byte to store the day
 * @param month a pointer to a byte to store the month
 * @param year a pointer to a byte to store the year
 */
void getDate(unsigned long *day, unsigned long *month, unsigned long *year);


/**
 * @brief Reads hour from RTC
 *
 * @param hour a pointer to a byte to store the hour
 * @param minutes a pointer to a byte to store the minutes
 * @param seconds a pointer to a byte to store the seconds
 */
void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds);



#endif /* RTC_H */
