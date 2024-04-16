#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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

// Configuração do hotspot
const char* ssid = "Boneco Resusci"; // Nome do seu ponto de acesso

// Declaração do servidor na porta 80
WiFiServer server(80);

// For debugging
volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;

/**
 * @returns Status
 * 			0: No Error
 * 			-1: Error
*/
int wifi_start(){
	if(!ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, send_wifi)){
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
	
	// Configurar a ESP8266 como ponto de acesso
	WiFi.softAP(ssid);

	return 0;
}

void send_wifi(){
  // Verificar se há clientes
  WiFiClient client = server.available();
  if (client) {

    // Responde à solicitação do cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(distance) + ";" + frequency);	// Estranho: porque ele converte a distância e a frequencia não?
    client.println();
    
    // Fechar a conexão com o cliente
    client.stop();
  }
}