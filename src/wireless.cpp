#include "main.h"
#include "wireless.h"


volatile bool request_to_send = false;

/* Starts static attributes */
ChestCompression *WiFiCommunicator::chest = nullptr;
AirFlow *WiFiCommunicator::air_flow = nullptr;
TimerInterruption WiFiCommunicator::timer_it(TIMER_0);
WiFiServer WiFiCommunicator::server(80);
const char*	WiFiCommunicator::ssid = "Boneco Resusci";
// volatile bool WiFiCommunicator::request_to_send = false;

/**
 * @brief Starts the wifi communicator
*/
int WiFiCommunicator::begin(ChestCompression *chest, AirFlow *air_flow){
	// Informs the communicator which sensors are to be monitored
	WiFiCommunicator::chest = chest;
	WiFiCommunicator::air_flow = air_flow;
	
	// Configurar a ESP32 como ponto de acesso
	WiFi.softAP(WiFiCommunicator::ssid);
  WiFiCommunicator::server.begin();

	// Configures the periodically routine in which messages are sent
	WiFiCommunicator::timer_it.set_timer_interrupt(&WiFiCommunicator::ISR);
  Serial.println("Left set_timer_interrupt");

  pinMode(2,OUTPUT);

  return 0;
}

/**
 * @brief Routine to send Wi-Fi messages of the operation status
*/
void WiFiCommunicator::send_wifi(){
  assert(WiFiCommunicator::chest != nullptr);
  assert(WiFiCommunicator::air_flow != nullptr);
  
  // Verificar se há clientes
  Serial.println("void WiFiCommunicator::send_wifi()");
  WiFiClient client = WiFiCommunicator::server.accept();
  Serial.println("WiFiClient client = WiFiCommunicator::server.accept();");
  if (client) {
    Serial.println("Cliente not false");
    // Responde à solicitação do cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(
                  String(WiFiCommunicator::chest->get_distance()) 
    //               + ";" + WiFiCommunicator::chest->get_frequency() 
                  + ";" + WiFiCommunicator::air_flow->get_flow()
                  );
#ifdef DEBUG
    Serial.println(
                  String(WiFiCommunicator::chest->get_distance()) 
    //               + ";" + WiFiCommunicator::chest->get_frequency() 
                  + ";" + WiFiCommunicator::air_flow->get_flow()
                  );
    Serial.println("Interrupção!!");
#endif
    client.println();
    
    // Fechar a conexão com o cliente
    client.stop();
  }

  request_to_send = false;
}

void WiFiCommunicator::ISR(void){
  digitalWrite(2,!digitalRead(2));
  request_to_send = true;
}