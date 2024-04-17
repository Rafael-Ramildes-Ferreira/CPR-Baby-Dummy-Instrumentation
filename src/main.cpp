// #include <Arduino.h>
#include <Adafruit_VL6180X.h>
#include "main.h"
#include "wireless.h"
#include "buildconfig.h"

Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
ChestCompression chest; // Temporary
WiFiCommunicator communicator(&chest);

double ChestCompression::get_distance(){return 42.0;}  // Temporary
double ChestCompression::get_frequency(){return 0.42;} // Temporary

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("");
  #endif

  // if (!dist_sensor.begin()) {
  //   error_handler();
  // }
}

void loop() {
  // put your main code here, to run repeatedly:
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
  }
  
}