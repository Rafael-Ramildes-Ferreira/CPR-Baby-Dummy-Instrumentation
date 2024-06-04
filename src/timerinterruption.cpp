#include "timerinterruption.h"
#include "main.h"


/**
 * @brief Initiates timer interruption instance, and sets up configurations
 * @param timer_instance: From TIMER_INSTANCES defgroup
*/
TimerInterruption::TimerInterruption(uint8_t timer_instance){
    this->timer_cfg = timerBegin(TIMER_0, PRESCALOR, COUNT_UP);
    timerAlarmWrite(this->timer_cfg, PRELOAD, true);
}

/**
 * @brief Sets a periadical timer interrupt to send the wireless messages
 * @param func: Function that describes the routine used to send the messages
 * @returns Stauts:
 * 			0: No error
 * 			-1: Error
*/
int TimerInterruption::set_timer_interrupt(void func(void)){
	// if(!TimerInterruption::ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, func)){
	// 	#ifdef DEBUG
	// 		Serial.println("ERROR: Periodical Wireless transmition failed");
	// 	#endif
	// 	return -1; 
	// }
	// #ifdef DEBUG
	// else {
	// 	Serial.print("Communication timer configured to ");
	// 	Serial.print(TIMER_INTERVAL_MS);
	// 	Serial.println(" msec");
	// }
	// #endif
	timerAttachInterrupt(this->timer_cfg, func, EDGE);
	timerAlarmWrite(this->timer_cfg, PRELOAD, AUTO_RELOAD);
	timerAlarmEnable(this->timer_cfg);
	#ifdef DEBUG
	Serial.println("Interrupt configured");
	#endif
	
    // timerAttachInterrupt(this->timer_cfg, func, true);
    // timerAlarmEnable(this->timer_cfg);

	return 0; // No error
}