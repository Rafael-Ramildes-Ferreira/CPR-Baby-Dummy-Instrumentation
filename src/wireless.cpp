#include <ESP8266WiFi.h>
#include "main.h"
#include "wireless.h"
#include "timerinterruption.h"


/* Starts static attributes */
ChestCompression *WiFiCommunicator::chest = nullptr;
WiFiServer WiFiCommunicator::server(80);

/**
 * @brief Starts the wifi communicator
*/
WiFiCommunicator::WiFiCommunicator(ChestCompression *chest){
	// Configures the periodically routine in which messages are sent
	TimerInterruption::set_timer_interrupt(send_wifi);
	
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