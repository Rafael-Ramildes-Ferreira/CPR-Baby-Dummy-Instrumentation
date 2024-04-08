#include <Arduino.h>
#include <Adafruit_VL6180X.h>
#include "main.h"

Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();

void setup() {
  if (!dist_sensor.begin()) {
    error_handler();
  }
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