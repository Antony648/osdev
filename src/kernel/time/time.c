#include "./time.h"
extern volatile uint64_t tick;
void sleep_ticks(uint64_t tick_count)
{
	uint64_t end=tick+tick_count;
	while(tick < end);
}
void sleep(uint32_t time_sec)
{
	sleep_ticks((uint64_t)(time_sec * 100));
}
void sleep_ms(uint32_t milli_sec)
{
	milli_sec=milli_sec<10?10:milli_sec;
	sleep_ticks((uint64_t)(milli_sec /10));
	
}
