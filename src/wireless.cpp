#include "main.h"
#include "wireless.h"
#include "utils.h"
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string.h>


// volatile bool request_to_send = false;

/* Starts static attributes */
#ifdef WIFI_COMMINICATION
ChestCompression *WiFiCommunicator::chest = nullptr;
AirFlow *WiFiCommunicator::air_flow = nullptr;
TimerInterruption WiFiCommunicator::timer_it(TIMER_0);
WiFiServer WiFiCommunicator::server(80);
const char*	WiFiCommunicator::ssid = "Boneco Resusci";
volatile bool WiFiCommunicator::request_to_send = false;
#endif	// WIFI_COMMINICATION

ChestCompression *BlueToothCommunicator::chest = nullptr;
AirFlow *BlueToothCommunicator::air_flow = nullptr;
TimerInterruption BlueToothCommunicator::timer_it(TIMER_0);
const char*	BlueToothCommunicator::ssid = "Boneco Resusci";
BLEServer *BlueToothCommunicator::btServer = nullptr;
BLEService *BlueToothCommunicator::sSend = nullptr;
#ifdef DISTANCE_SENSOR
BLECharacteristic *BlueToothCommunicator::sSendCompress = nullptr;
#ifdef FREQUENCY_ON_ESP
BLECharacteristic *BlueToothCommunicator::sSendFrequency = nullptr;
#endif  // FREQUENCY_ON_ESP
#endif	// DISTANCE_SENSOR
#ifdef AIR_FLOW_SENSOR
BLECharacteristic *BlueToothCommunicator::sSendFlow = nullptr;
#endif	// AIR_FLOW_SENSOR
BLEAdvertising *BlueToothCommunicator::pAdvertising = nullptr;
volatile bool BlueToothCommunicator::request_to_send = false;
volatile bool BlueToothCommunicator::conected = false;


#ifdef WIFI_COMMINICATION
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
#endif  // WIFI_COMMINICATION

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
  #ifdef DISTANCE_SENSOR
  sSendCompress = sSend->createCharacteristic(COMPRESS_UUID, cnotify);
  sSendCompress->addDescriptor(new BLE2902());
  sSendCompress->setValue("0");

  #ifdef FREQUENCY_ON_ESP
  sSendFrequency = sSend->createCharacteristic(FREQUENCY_UUID, cnotify);
  sSendFrequency->addDescriptor(new BLE2902());
  sSendFrequency->setValue("0");
  #endif  // FREQUENCY_ON_ESP
  #endif  // DISTANCE_SENSOR
  
  #ifdef AIR_FLOW_SENSOR
  sSendFlow = sSend->createCharacteristic(FLOW_UUID, cnotify);
  sSendFlow->addDescriptor(new BLE2902());
  sSendFlow->setValue("0");
  #endif  // AIR_FLOW_SENSOR

  BlueToothCommunicator::sSend->start();

  // Advertising
  BlueToothCommunicator::pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SEND_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x04);  // (0x06) functions that help with iPhone connections issue
  // pAdvertising->setMinPreferred(0x12);
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
  #ifdef DISTANCE_SENSOR
  assert(BlueToothCommunicator::chest != nullptr);
  #endif  // DISTANCE_SENSOR
  #ifdef AIR_FLOW_SENSOR
  assert(BlueToothCommunicator::air_flow != nullptr);
  #endif  // AIR_FLOW_SENSOR

  // Calcula os valores
  #ifdef DISTANCE_SENSOR
  float distance = BlueToothCommunicator::chest->get_distance();
  #ifdef FREQUENCY_ON_ESP
  float frequency = BlueToothCommunicator::chest->get_frequency();
  #endif  // FREQUENCY_ON_ESP
  #endif  // DISTANCE_SENSOR
  #ifdef AIR_FLOW_SENSOR
  float flow = BlueToothCommunicator::air_flow->get_flow();
  #endif  // AIR_FLOW_SENSOR

  #ifdef DISTANCE_SENSOR
  uint8_t dist_char_array[4];
  memcpy(dist_char_array, &distance, 4);
  #ifdef FREQUENCY_ON_ESP
  uint8_t freq_char_array[4];
  memcpy(freq_char_array, &frequency, 4);
  #endif  // FREQUENCY_ON_ESP
  #endif  // DISTANCE_SENSOR
  #ifdef AIR_FLOW_SENSOR
  char flow_char_array[4];
  memcpy(flow_char_array, &flow, 4);
  #endif  // AIR_FLOW_SENSOR
  
  // Atualiza os valores
  #ifdef DISTANCE_SENSOR
  BlueToothCommunicator::sSendCompress->setValue(dist_char_array,4);
  BlueToothCommunicator::sSendCompress->notify();
  
  #ifdef FREQUENCY_ON_ESP
  BlueToothCommunicator::sSendFrequency->setValue(freq_char_array,4);
  BlueToothCommunicator::sSendFrequency->notify();
  #endif  // FREQUENCY_ON_ESP
  #endif  // DISTANCE_SENSOR
  
  # ifdef AIR_FLOW_SENSOR
  BlueToothCommunicator::sSendFlow->setValue(flow_char_array);
  BlueToothCommunicator::sSendFlow->notify();
  #endif  // AIR_FLOW_SENSOR

#ifdef DEBUG
    Serial.print(".");
#endif

  BlueToothCommunicator::request_to_send = false;
}

void BlueToothCommunicator::ISR(void){
  if(BlueToothCommunicator::conected == true){
    digitalWrite(2,!digitalRead(2));
    BlueToothCommunicator::request_to_send = true;
  }
}