#include "timerinterruption.h"
#include "main.h"


ESP8266Timer TimerInterruption::ITimer;

/**
 * @brief Sets a periadical timer interrupt to send the wireless messages
 * @param func: Function that describes the routine used to send the messages
 * @returns Stauts:
 * 			0: No error
 * 			-1: Error
*/
int TimerInterruption::set_timer_interrupt(void func(void)){
	if(!TimerInterruption::ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, func)){
		#ifdef DEBUG
			Serial.println("ERROR: Periodical Wireless transmition failed");
		#endif
		return -1; 
	}
	#ifdef DEBUG
	else {
		Serial.print("Communication timer configured to ");
		Serial.print(TIMER_INTERVAL_MS);
		Serial.println(" msec");
	}
	#endif

	return 0; // No error
}