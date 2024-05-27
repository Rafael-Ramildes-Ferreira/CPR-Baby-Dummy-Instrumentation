// #include <ESP8266WiFi.h>
#include "main.h"
#include "wireless.h"


/* Starts static attributes */
ChestCompression *WiFiCommunicator::chest = nullptr;
TimerInterruption WiFiCommunicator::timer_it(TIMER_0);
WiFiServer WiFiCommunicator::server(80);
const char*	WiFiCommunicator::ssid = "Boneco Resusci";

/**
 * @brief Starts the wifi communicator
*/
int WiFiCommunicator::begin(ChestCompression *chest){
	// // Informs the communicator which chest is been monitored
	WiFiCommunicator::chest = chest;
	
	// Configurar a ESP32 como ponto de acesso
	WiFi.softAP(WiFiCommunicator::ssid);
  WiFiCommunicator::server.begin();

	// Configures the periodically routine in which messages are sent
	WiFiCommunicator::timer_it.set_timer_interrupt(&WiFiCommunicator::send_wifi);

  return 0;
}

/**
 * @brief Routine to send Wi-Fi messages of the operation status
*/
void WiFiCommunicator::send_wifi(){
  assert(WiFiCommunicator::chest != nullptr);
  
  // Verificar se há clientes
  WiFiClient client = WiFiCommunicator::server.accept();
  if (client) {

    // Responde à solicitação do cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(WiFiCommunicator::chest->get_distance()) + ";" + WiFiCommunicator::chest->get_frequency());
    client.println();
    
    // Fechar a conexão com o cliente
    client.stop();
  }
}