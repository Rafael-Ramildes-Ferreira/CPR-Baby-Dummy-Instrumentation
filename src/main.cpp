#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <SPI.h>
#include "main.h"
#include "chest_compressions.h"


ChestCompression chest;


void setup() {
  ESP.wdtDisable();//Desabilita o SW WDT. 

  Serial.begin(115200);
  delay(1);
  Serial.println("Começando");

  ESP.wdtFeed();
}

void loop() {
  // double distance = chest.filtered_read_sensor();
  double distance = 3.1416;
  double frequency = chest.calc_frequency();

  Serial.print("Distância: ");
  Serial.println(distance);
  Serial.print("Freqüência: ");
  Serial.println(frequency);
  ESP.wdtFeed();
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
    Serial.println("ERROR!!!");
  }
  
}