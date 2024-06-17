#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <WiFi.h>
#include "chest_compressions.h"
#include "timerinterruption.h"
#include "air_flow.h"


extern volatile bool request_to_send;

/**
 * @brief Class to deal with setup and running of the periadical WiFi transmit
 * routine
*/
class WiFiCommunicator{
	public:
		static int begin(ChestCompression *chest, AirFlow *airflow);
		static void send_wifi(void);
		
		// ISR: Interrupt Service Routine
		static void ISR(void);
		// static volatile bool request_to_send;

	private:
		static ChestCompression *chest;
		static AirFlow *air_flow;
		static const char* ssid; // Nome do seu ponto de acesso
		static TimerInterruption timer_it;
		static WiFiServer server;
};

#endif