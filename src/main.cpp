// #include <Arduino.h>
#include <Adafruit_VL6180X.h>
#include "main.h"
#include "wireless.h"
#include "buildconfig.h"

Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("");
  #endif

  if (!dist_sensor.begin()) {
    error_handler();
  }

  wifi_start();
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