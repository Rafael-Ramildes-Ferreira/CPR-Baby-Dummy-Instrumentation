#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include "chest_compressions.h"
#include "timerinterruption.h"
#include "air_flow.h"


#define DEVICENAME		"Boneco Ressuci"
#define SEND_UUID		"9a9a40aa-805e-4677-bc7c-3ce43fee3e31"
#define COMPRESS_UUID	"bc98e173-d5aa-491d-af24-b45386ec0102"
#define FLOW_UUID		"685df5a8-09ab-4198-b8b4-665e60e21611"

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
		static volatile bool request_to_send;

	private:
		static ChestCompression *chest;
		static AirFlow *air_flow;
		static const char* ssid; // Nome do seu ponto de acesso
		static TimerInterruption timer_it;
		static WiFiServer server;
};

/**
 * @brief Class to deal with setup and running of the periadical Bluetooth transmit
 * routine
*/
class BlueToothCommunicator{
	public:
		static int begin(ChestCompression *chest, AirFlow *airflow);
		static void update(void);
		
		// ISR: Interrupt Service Routine
		static void ISR(void);
		static volatile bool request_to_send;

	private:
		static ChestCompression *chest;
		static AirFlow *air_flow;
		static const char* ssid; // Nome do seu ponto de acesso
		static TimerInterruption timer_it;
		static BLEServer *btServer;
		static BLEService *sSend;
		static BLECharacteristic *sSendCompress;
		static BLECharacteristic *sSendFlow;
		static BLEAdvertising *pAdvertising;
};

#ifdef DEBUG
/**
 * @brief 	Holds callback functions for when a bluetooth conection happens
 * 			It's used only in debug mode
 */
class ConnectionServerCallbacks : public BLEServerCallbacks{
	public:
		void onConnect(BLEServer* pServer) {
			Serial.println("Connected");
		};

		void onDisconnect(BLEServer* pServer) {
			Serial.println("Disconnected");
		}
};
#endif

#endif