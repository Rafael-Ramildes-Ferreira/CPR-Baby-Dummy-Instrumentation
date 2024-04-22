#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <SPI.h>
#include "main.h"
#include "wireless.h"
#include "buildconfig.h"
#include "chest_compressions.h"


Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
ChestCompression chest; // Temporary
WiFiCommunicator communicator(&chest);

// Debug
int i = 0;


void setup() {
  ESP.wdtDisable();//Desabilita o SW WDT. 

  Serial.begin(115200);
  delay(1);
  Serial.println("Começando");

  ESP.wdtFeed();
}

void loop() {
  double distance = chest.calc_distance();
  double frequency = chest.calc_frequency();

  if(i%100 == 0){
  Serial.print("Distância: ");
  Serial.println(distance);
  Serial.print("Freqüência: ");
  Serial.println(frequency);
  }
  ESP.wdtFeed();

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