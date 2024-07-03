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
ChestCompression *chest;
// AirFlow air_flow;

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
  
  chest = new ChestCompression();
  if(!(chest->begin())){
    error_handler();
  }
#ifdef DEBUG
  Serial.println("chest.begin()");
#endif

  // air_flow.begin();
#ifdef DEBUG
  // Serial.println("air_flow.begin();");
#endif

  communicator = new BlueToothCommunicator();
#ifdef DEBUG
  Serial.println("new BlueToothCommunicator();");
#endif
  
  // communicator->begin(nullptr,nullptr);
  communicator->begin(chest,nullptr);//,&air_flow);
#ifdef DEBUG
  Serial.println("communicator->begin(chest,nullptr);//,&air_flow);");
#endif
}

void loop() {
  rtc_wdt_feed();

  chest->calc_distance();
  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("Distance: ");
    Serial.println(chest->get_distance());
  }
  #endif

  chest->calc_frequency();  
  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("Frequency: ");
    Serial.println(chest->get_frequency());
  }
  #endif
  
  if(i++%100 == 0){
    Serial.print("distance: ");
    Serial.println(chest->get_distance());
    // Serial.print("flow: ");
    // Serial.println(air_flow.get_flow());
  }
  if(communicator->request_to_send)
  {
    communicator->update();
  }
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
    Serial.println("ERROR!!!");
  }
  
}