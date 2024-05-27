#ifndef _WIRELESS_H
#define _WIRELESS_H

// #include <ESP8266WebServer.h>
#include "chest_compressions.h"
#include "timerinterruption.h"

/**
 * @brief Class to deal with setup and running of the periadical WiFi transmit
 * routine
*/
class WiFiCommunicator{
	public:
		WiFiCommunicator(ChestCompression *chest);
		static void send_wifi();
		static ChestCompression *chest;
	private:
		const char* ssid; // Nome do seu ponto de acesso
		TimerInterruption timer_it = TimerInterruption(TIMER_0);
		// static WiFiServer server;
};

#endif