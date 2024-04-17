#include <ESP8266WiFi.h>
#include "main.h"
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

/**
 * @brief Sets a periadical timer interrupt to send the wireless messages
 * @param func: Function that describes the routine used to send the messages
 * @returns Stauts:
 * 			0: No error
 * 			-1: Error
*/
static int set_timer_interrupt(void func(void)){
	if(!ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, func)){
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

/* Starts static attributes */
ChestCompression *WiFiCommunicator::chest = nullptr;
WiFiServer WiFiCommunicator::server(80);

/**
 * @brief Starts the wifi communicator
*/
WiFiCommunicator::WiFiCommunicator(ChestCompression *chest){
	// Configures the periodically routine in which messages are sent
	set_timer_interrupt(send_wifi);
	
	// Configurar a ESP8266 como ponto de acesso
	WiFi.softAP(ssid);

	// Informs the communicator which chest is been monitored
	WiFiCommunicator::chest = chest;
	this->ssid = "Boneco Resusci";
}

/**
 * @brief Routine to send Wi-Fi messages of the operation status
*/
void WiFiCommunicator::send_wifi(){
  // Verificar se há clientes
  WiFiClient client = WiFiCommunicator::server.accept();
  if (client) {

    // Responde à solicitação do cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(chest->get_distance()) + ";" + chest->get_frequency());	// Estranho: porque ele converte a distância e a frequencia não?
    client.println();
    
    // Fechar a conexão com o cliente
    client.stop();
  }
}