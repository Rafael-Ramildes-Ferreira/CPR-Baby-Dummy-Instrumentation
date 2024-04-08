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
  // Filter values
  static double yn = 0, yn1 = 0, yn2 = 0, xn = 0, xn1 = 0;

  // Reads the sensor
  uint8_t range = dist_sensor.readRange();
  if (dist_sensor.readRangeStatus() == VL6180X_ERROR_NONE) {
    xn = range;
    
  }else{
    // xn = UINT8_MAX; // Used to be 200 (is that the maximum distance? [todo])
    return yn; // Ignores failed reading
  }

  // Compute the filtered signal
  yn = 1.656*yn1 - 0.6859*yn2 + 0.01568*xn + 0.01383*xn1;

  // Updates values
  // delay(1); // Why delay?
  xn1 = xn;
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