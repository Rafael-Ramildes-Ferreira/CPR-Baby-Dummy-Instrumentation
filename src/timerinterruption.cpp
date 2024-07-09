#include "timerinterruption.h"
#include "main.h"


/**
 * @brief Initiates timer interruption instance, and sets up configurations
 * @param timer_instance: From TIMER_INSTANCES defgroup
*/
TimerInterruption::TimerInterruption(uint8_t timer_instance){
    this->timer_cfg = timerBegin(TIMER_0, PRESCALOR, COUNT_UP);
    timerAlarmWrite(this->timer_cfg, PRELOAD, AUTO_RELOAD);
}

/**
 * @brief Sets a periadical timer interrupt to send the wireless messages
 * @param func: Function that describes the routine used to send the messages
 * @returns Stauts:
 * 			0: No error
 * 			-1: Error
*/
int TimerInterruption::set_timer_interrupt(void func(void)){
	timerAttachInterrupt(this->timer_cfg, func, EDGE);
	timerAlarmEnable(this->timer_cfg);
	#ifdef DEBUG
	Serial.println("Interrupt configured");
	#endif
	

	return 0; // No error
}


// void TimerInterruption::ISR(void){
//   digitalWrite(2,!digitalRead(2));
//   request_to_send = true;
// }