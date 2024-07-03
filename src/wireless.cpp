#include "main.h"
#include "wireless.h"
#include <BLEUtils.h>
#include <BLE2902.h>


// volatile bool request_to_send = false;

/* Starts static attributes */
ChestCompression *WiFiCommunicator::chest = nullptr;
AirFlow *WiFiCommunicator::air_flow = nullptr;
TimerInterruption WiFiCommunicator::timer_it(TIMER_0);
WiFiServer WiFiCommunicator::server(80);
const char*	WiFiCommunicator::ssid = "Boneco Resusci";
volatile bool WiFiCommunicator::request_to_send = false;

ChestCompression *BlueToothCommunicator::chest = nullptr;
AirFlow *BlueToothCommunicator::air_flow = nullptr;
TimerInterruption BlueToothCommunicator::timer_it(TIMER_0);
const char*	BlueToothCommunicator::ssid = "Boneco Resusci";
BLEServer *BlueToothCommunicator::btServer = nullptr;
BLEService *BlueToothCommunicator::sSend = nullptr;
BLECharacteristic *BlueToothCommunicator::sSendCompress = nullptr;
#ifdef FREQUENCY_ON_ESP
BLECharacteristic *BlueToothCommunicator::sSendFrequency = nullptr;
#endif
BLECharacteristic *BlueToothCommunicator::sSendFlow = nullptr;
BLEAdvertising *BlueToothCommunicator::pAdvertising = nullptr;
volatile bool BlueToothCommunicator::request_to_send = false;

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
  
  #ifdef DEBUG
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  #endif

	// Configures the periodically routine in which messages are sent
	WiFiCommunicator::timer_it.set_timer_interrupt(&WiFiCommunicator::ISR);

  pinMode(2,OUTPUT);

  return 0;
}

/**
 * @brief Routine to send Wi-Fi messages of the operation status
*/
void WiFiCommunicator::send_wifi(){
  assert(WiFiCommunicator::chest != nullptr);
  assert(WiFiCommunicator::air_flow != nullptr);

  double distance = WiFiCommunicator::chest->get_distance();
  double flow = WiFiCommunicator::air_flow->get_flow();
  
  // Verificar se há clientes
  WiFiClient client = WiFiCommunicator::server.available();
  if (client.available()) {
    // Responde à solicitação do cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(6) + ";" + 3);
    // client.println(
    //               String(distance) 
    //               + ";" + flow
    //               );
    client.println();
    // Fechar a conexão com o cliente
    client.stop();

#ifdef DEBUG
    Serial.println(
                  String(distance) 
                  + ";" + flow
                  );
#endif
  }

  WiFiCommunicator::request_to_send = false;
}

void WiFiCommunicator::ISR(void){
  digitalWrite(2,!digitalRead(2));
  WiFiCommunicator::request_to_send = true;
}

/**
 * @brief Starts the bluetooth communicator
*/
int BlueToothCommunicator::begin(ChestCompression *chest, AirFlow *air_flow){
	// Informs the communicator which sensors are to be monitored
	BlueToothCommunicator::chest = chest;
	BlueToothCommunicator::air_flow = air_flow;
	
	// Configura a ESP32 como ponto de acesso
  BLEDevice::init(DEVICENAME);
  BlueToothCommunicator::btServer = BLEDevice::createServer();
  #ifdef DEBUG
  btServer->setCallbacks(new ConnectionServerCallbacks());
  #endif

  // Configura o Serviço
  BlueToothCommunicator::sSend = btServer->createService(SEND_UUID);
  uint32_t cnotify = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE;

  // Configura as caracteristicas
  sSendCompress = sSend->createCharacteristic(COMPRESS_UUID, cnotify);
  sSendCompress->addDescriptor(new BLE2902());
  sSendCompress->setValue("0");
  #ifdef FREQUENCY_ON_ESP
  sSendFrequency = sSend->createCharacteristic(FREQUENCY_UUID, cnotify);
  sSendFrequency->addDescriptor(new BLE2902());
  sSendFrequency->setValue("0");
  #endif
  // sSendFlow = sSend->createCharacteristic(FLOW_UUID, cnotify);
  // sSendFlow->addDescriptor(new BLE2902());
  // sSendFlow->setValue("0");

  BlueToothCommunicator::sSend->start();

  // Advertising
  BlueToothCommunicator::pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SEND_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  #ifdef DEBUG
  Serial.print("Device Name: ");
  Serial.println(DEVICENAME);
  #endif

	// Configures the periodically routine in which messages are sent
	BlueToothCommunicator::timer_it.set_timer_interrupt(&BlueToothCommunicator::ISR);

  pinMode(2,OUTPUT);
  Serial.println("Tudo configurado");

  return 0;
}

/**
 * @brief Routine to send Bluetooth messages of the operation status
*/
void BlueToothCommunicator::update(){
  assert(BlueToothCommunicator::chest != nullptr);
  // assert(BlueToothCommunicator::air_flow != nullptr);

  // Calcula os valores
  const String distance = String(BlueToothCommunicator::chest->get_distance());
  #ifdef FREQUENCY_ON_ESP
  const String frequency = String(BlueToothCommunicator::chest->get_frequency());
  #endif
  // const String flow = String(BlueToothCommunicator::air_flow->get_flow());

  char dist_char_array[5];
  distance.toCharArray(dist_char_array,5);
  #ifdef FREQUENCY_ON_ESP
  char freq_char_array[5];
  frequency.toCharArray(freq_char_array,5);
  #endif
  // char flow_char_array[5];
  // flow.toCharArray(flow_char_array,5);
  
  // Atualiza os valores
  BlueToothCommunicator::sSendCompress->setValue(dist_char_array);
  BlueToothCommunicator::sSendCompress->notify();
  
  #ifdef FREQUENCY_ON_ESP
  BlueToothCommunicator::sSendFrequency->setValue(freq_char_array);
  BlueToothCommunicator::sSendFrequency->notify();
  #endif
  
  // BlueToothCommunicator::sSendFlow->setValue(flow_char_array);
  // BlueToothCommunicator::sSendFlow->notify();

#ifdef DEBUG
    Serial.print(".");
#endif

  BlueToothCommunicator::request_to_send = false;
}

void BlueToothCommunicator::ISR(void){
  digitalWrite(2,!digitalRead(2));
  BlueToothCommunicator::update();
  BlueToothCommunicator::request_to_send = true;
}