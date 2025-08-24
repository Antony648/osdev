#ifndef TIME_H
#define	TIME_H
#include <stdint.h>

void sleep(uint32_t time_sec);
void sleep_ms(uint32_t milli_sec);
void sleep_ticks(uint64_t tick_count);
#endif
