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

double read_sensor(){
  static double yn1 = 0, yn2 = 0, xn0 = 0, xn1 = 0;

  uint8_t range = dist_sensor.readRange();

  if (dist_sensor.readRangeStatus() == VL6180X_ERROR_NONE) {
    xn0 = range;
    
  }else{
    xn0 = UINT8_MAX; // Used to be 200 (is that the maximum distance? [todo])
  }

  // Compute the filtered signal
  double yn = 1.656*yn1 - 0.6859*yn2 + 0.01568*xn0 + 0.01383*xn1;

  // Updates values
  // delay(1); // Why delay?
  xn1 = xn0;
  yn2 = yn1;
  yn1 = yn;

  return yn/10.0;
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
  }
  
}