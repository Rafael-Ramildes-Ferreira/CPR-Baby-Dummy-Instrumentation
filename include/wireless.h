#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <ESP8266WebServer.h>
#include "main.h" // Temporary
// #include "ChestCompression.h"

class WiFiCommunicator{
	public:
		WiFiCommunicator(ChestCompression *chest);
		static void send_wifi();
		static ChestCompression *chest;
	private:
		const char* ssid; // Nome do seu ponto de acesso
		static WiFiServer server;
};

#endif