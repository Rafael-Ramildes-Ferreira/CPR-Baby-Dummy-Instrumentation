#include "buildconfig.h"
#include "wireless.h"
#include "ESP8266TimerInterrupt.h"

// Select a Timer Clock
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default

#define TIMER_INTERVAL_MS       1000


// Init ESP8266 timer 1
ESP8266Timer ITimer;

// For debugging
volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;

/**
 * @returns Status
 * 			0: No Error
 * 			-1: Error
*/
int wifi_start(){
	if(!ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, send_wifi)){
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

	return 0;
}

void send_wifi(){
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(BUILTIN_LED, OUTPUT);
  }

  digitalWrite(BUILTIN_LED, statusLed);  //Toggle LED Pin
  statusLed = !statusLed;
}