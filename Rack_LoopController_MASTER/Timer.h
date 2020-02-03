
#ifndef TIMER_H_
#define TIMER_H_

#define TIMER0_PERIOD_US	1997/*us*/

void Timer0_Init();
uint32_t Timer0_ms2cnt(uint32_t ms);

#endif /* TIMER_H_ */