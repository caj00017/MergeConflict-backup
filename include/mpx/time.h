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

#endif
