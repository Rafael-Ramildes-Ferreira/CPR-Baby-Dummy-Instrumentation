#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <WiFi.h>
#include "chest_compressions.h"
#include "timerinterruption.h"
#include "air_flow.h"

/**
 * @brief Class to deal with setup and running of the periadical WiFi transmit
 * routine
*/
class WiFiCommunicator{
	public:
		static int begin(ChestCompression *chest, AirFlow *airflow);
		
		// ISR: Interrupt Service Routine
		// static void send_wifi(void);

	// private:
		static ChestCompression *chest;
		static AirFlow *air_flow;
		static const char* ssid; // Nome do seu ponto de acesso
		static TimerInterruption timer_it;
		static WiFiServer server;
};

#endif