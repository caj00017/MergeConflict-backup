#ifndef MPX_TIME_H
#define MPX_TIME_H

/**
 * @brief Function which utilizes the real time clock registers to display the current time. 
 * @return Status code
 * @author Tanner Forbes
 */
int get_time(void);

/**
 * @brief Function which utilizes the real time clock registers to set the current time. 
 * @return Status code
 * @param buf Command string to extract the time from
 * @author Chris Jones
 */
int set_time(char buf[]);

/**
 * @brief Function which utilizes the real time clock registers to get the current date. 
 * @return Status code
 * @author Tanner Forbes
 */
int get_date(void);

/**
 * @brief Function which utilizes the RTC registers to set the current date. 
 * @param buf Command string to extract the date from
 * @return Status code
 * @author Tanner Forbes
 */
int set_date(char buf[]);

/**
 * @brief Function which utilizes the RTC registers to get the current hour.
 * @return Current hour value
 * @author Tanner Forbes
 */
int get_hour(void);

/**
 * @brief Function which utilizes the RTC registers to get the current minute.
 * @return Current minute value
 * @author Tanner Forbes
 */
int get_minute(void);

/**
 * @brief Function which utilizes the RTC registers to get the current second.
 * @return Current second value
 * @author Tanner Forbes
 */
int get_second(void);

#endif
