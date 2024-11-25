#ifndef _TIMERINTERRUPTION_H
#define _TIMERINTERRUPTION_H

#include "Arduino.h"

/**
 * @defgroup TIMER_INSTANCES
 * @brief 	Holds the variable that configures the timer used in 
 * 			timerBegin(uint8_t,uint16_t,bool)
 * @{
*/
#define TIMER_0		(0)
#define TIMER_1		(1)
#define TIMER_2		(2)
#define TIMER_3		(3)
/**
 * @}
*/


/**
 * @defgroup TIMER_COUNT_DIRECTION
 * @brief	Holds configured variable to set timer directionality in
 * 			timerBegin(uint8_t,uint16_t,bool)
 * @{
*/
#define COUNT_UP	(true)
#define COUNT_DOWN	(false)
/**
 * @}
*/

/**
 * @defgroup TIMER_INTERRUPT_EDGE
 * @brief	Holds parameters used to configure a timer interrupt
 * @{
*/
#define EDGE	(true)
#define FLAT	(false)
/**
 * @}
*/


/**
 * @defgroup TIMER_INTERRUPT_SETTINGS
 * @brief	Holds the variables that define the interruption interval
 * @{
*/
#define TIMER_FREQUENCY			(80e6)	// 80MHz
#define TIMER_INTERVAL_MS		(100)
#define TIMER_INTERVAL_S		((float) TIMER_INTERVAL_MS*1e-3)
#define PRELOAD					(1000)
// #define PRESCALOR				((uint16_t)(TIMER_INTERVAL_S*TIMER_FREQUENCY/PRELOAD))
#define PRESCALOR(period_ms)	((uint16_t)(((float)period_ms*1e-3)*TIMER_FREQUENCY/PRELOAD))
/**
 * @}
*/

/**
 * @defgroup TIMER_CONFIGS
 * @brief	Overall configuration defines
 * @{
*/
#define AUTO_RELOAD		(true)
#define MANUAL_RELOAD	(false)
/**
 * @}
*/


/**
 * @brief Wrapper class of the interrupt class
 * It helps to add debug functionality and keep the code consistent
*/
class TimerInterruption{
	public:
		TimerInterruption(uint8_t timer_instance);
		TimerInterruption(uint8_t timer_instance,uint16_t period_ms);
		int set_timer_interrupt(void func(void));
		// void ISR(void);
	private:
		hw_timer_t *timer_cfg;
};

#endif