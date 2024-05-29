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
#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <SPI.h>
#include "main.h"
#include "wireless.h"
#include "air_flow.h"
#include "buildconfig.h"
#include "chest_compressions.h"
#include "soc/rtc_wdt.h"


// Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
WiFiCommunicator *communicator;
ChestCompression *chest = nullptr; // Temporary
AirFlow air_flow;

// Debug
int i = 0;


void setup() {
  rtc_wdt_feed();
  Serial.begin(115200);
  delay(1);
  Serial.println("Começando");

  chest = new ChestCompression(); // Temporary

  communicator = new WiFiCommunicator();
  air_flow.begin();
  communicator->begin(chest,&air_flow);
}

void loop() {
  rtc_wdt_feed();
  double distance = chest->calc_distance();
  double frequency = chest->calc_frequency();
  // double air = air_flow.get_flow();

  if(i%100 == 0){
  Serial.print("Distância: ");
  Serial.println(distance);
  Serial.print("Freqüência: ");
  Serial.println(frequency);
  // Serial.print("Fluxo de ar: ");
  // Serial.println(air);
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