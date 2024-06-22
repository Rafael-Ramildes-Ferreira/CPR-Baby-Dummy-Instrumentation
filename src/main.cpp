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
ChestCompression *chest = nullptr; // Temporary
AirFlow air_flow;

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
  chest = new ChestCompression(); // Temporary
#ifdef DEBUG
  Serial.println("new ChestCompression();");
#endif

  communicator = new BlueToothCommunicator();
#ifdef DEBUG
  Serial.println("new BlueToothCommunicator();");
#endif

  air_flow.begin();
#ifdef DEBUG
  Serial.println("air_flow.begin();");
#endif
  
  // communicator->begin(nullptr,nullptr);
  communicator->begin(chest,&air_flow);
#ifdef DEBUG
  Serial.println("communicator->begin(chest,&air_flow);");
#endif
}

void loop() {
  rtc_wdt_feed();
  
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