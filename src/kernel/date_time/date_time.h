#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <stdint.h>
struct date
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
};
struct time
{
	uint8_t second;
	uint8_t minute;
	uint8_t	hour;
};
#endif