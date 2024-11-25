/**
 * ATTENTION: YOU NEED TO CHEAT!!!
 * You may need to coment out the lines:
 * 
 * 69:  if (read8(VL6180X_REG_IDENTIFICATION_MODEL_ID) != 0xB4) {
 * 70:    return false;
 * 71:  }
 * 
 * from .pio\libdeps\nodemcu-32s\Adafruit_VL6180X\Adafruit_VL6180X.cpp
*/
#include "buildconfig.h"
#include "main.h"
#include "wireless.h"
#include "air_flow.h"
#include "chest_compressions.h"
#include "soc/rtc_wdt.h"


// Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
BlueToothCommunicator *communicator;
#ifdef DISTANCE_SENSOR
ChestCompression *chest;
#endif  // DISTANCE_SENSOR
#ifdef AIR_FLOW_SENSOR
AirFlow air_flow;
#endif  // AIR_FLOW_SENSOR

#ifdef DEBUG
// Debug
int i = 0;
#endif


void setup() {
  rtc_wdt_feed();

#ifdef DEBUG
  Serial.begin(115200);
#endif

  delay(1);

#ifdef DEBUG
  Serial.println("Começando");
#endif
  
  #ifdef DISTANCE_SENSOR
  chest = new ChestCompression();
  chest->begin();
  // if(!(chest->begin())){
  //   error_handler();
  // }
#ifdef DEBUG
  Serial.println("chest.begin()");
#endif
  #endif

  #ifdef AIR_FLOW_SENSOR
  air_flow.begin();
#ifdef DEBUG
  Serial.println("air_flow.begin();");
#endif
  #endif

  communicator = new BlueToothCommunicator();
#ifdef DEBUG
  Serial.println("new BlueToothCommunicator();");
#endif
  
  communicator->begin(AVAILABLE_SENSORS);
#ifdef DEBUG
  Serial.println("communicator->begin(AVAILABLE_SENSORS);");
#endif
}

void loop() {
  #ifdef DISTANCE_SENSOR

  // Distance
  rtc_wdt_feed();
  chest->calc_distance();

  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("\nDistance: ");
    Serial.println(chest->get_distance());
  }
  #endif  // DEBUG

  #ifdef FREQUENCY_ON_ESP
  // Frequency
  rtc_wdt_feed();
  chest->calc_frequency();  
  
  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("Frequency: ");
    Serial.println(chest->get_frequency());
  }
  #endif  // DEBUG 
  #endif  // FREQUENCY_ON_ESP

  #endif  // DISTANCE_SENSOR
  
  // Air Flow
  #ifdef AIR_FLOW_SENSOR
  rtc_wdt_feed();
  air_flow.readFlow();

  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("Air flow: ");
    Serial.println(air_flow.get_flow());
  }
  #endif  // DEBUG
  #endif  // AIR_FLOW_SENSOR

  if(communicator->request_to_send)
  {
    rtc_wdt_feed();
    communicator->update();
  }

  i++;
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
    Serial.println("ERROR!!!");
  }
  
}