#include <Arduino.h>
#include <Adafruit_VL6180X.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <SPI.h>
#include "main.h"
#include "chest_compressions.h"

Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();

int i = 0;

void setup() {
  ESP.wdtDisable();//Desabilita o SW WDT. 

  Serial.begin(115200);
  delay(1);
  Serial.println("Começando");

  if (!dist_sensor.begin()) {
    error_handler();
  }
  ESP.wdtFeed();
}

void loop() {
  // double distance = filtered_read_sensor();
  double distance = 3.1416;
  calc_frequency(distance);

  Serial.print("Distância: ");
  Serial.println(distance);
  ESP.wdtFeed();
}

double filtered_read_sensor(){
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
    Serial.println("ERROR!!!");
  }
  
}