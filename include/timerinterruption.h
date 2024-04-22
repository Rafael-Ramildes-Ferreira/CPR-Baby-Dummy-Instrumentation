#ifndef _TIMERINTERRUPTION_H
#define _TIMERINTERRUPTION_H

#include "ESP8266TimerInterrupt.h"


// Select a Timer Clock
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default

#define TIMER_INTERVAL_MS       1000


/**
 * @brief Wrapper class of the ESP8266 class
 * It helps to add debug functionality and keep the code consistent
*/
class TimerInterruption{
	public:
		static int set_timer_interrupt(void func(void));
	private:
		static ESP8266Timer ITimer;
};

#endif