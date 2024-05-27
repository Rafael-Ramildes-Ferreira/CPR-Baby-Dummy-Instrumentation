#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <WiFi.h>
#include "chest_compressions.h"
#include "timerinterruption.h"

/**
 * @brief Class to deal with setup and running of the periadical WiFi transmit
 * routine
*/
class WiFiCommunicator{
	public:
		WiFiCommunicator(ChestCompression *chest);
		static ChestCompression *chest;
		
		// ISR: Interrupt Service Routine
		static void send_wifi(void);

	private:
		static const char* ssid; // Nome do seu ponto de acesso
		static TimerInterruption timer_it; // = TimerInterruption(TIMER_0);
		static WiFiServer server; // = WiFiServer(80);
};

#endif