/**
 * ATTENTION: YOU NEED TO CHEAT!!!
 * You may need to coment out the lines:
 * 
 * 69:  if (read8(VL6180X_REG_IDENTIFICATION_MODEL_ID) != 0xB4) {
 * 70:    return false;
 * 71:  }
 * 
 * from .pio\libdeps\nodemcu-32s\Adafruit_VL6180X\Adafruit_VL6180X.cpp
 * 
 * ATTENTION: You need to make a fill changes for R2U2 to work
 * Add paddings to the structs
 * You also need to change cases like the following:
 * - boxq->queue = &(elements[(R2U2_MAX_BOXQ_BYTES / sizeof(r2u2_boxq_intvl_t)) - instr->memory_reference]);
 * + boxq->queue = &(elements[(NUN_OF_BOXQ_BYTES / sizeof(r2u2_boxq_intvl_t)) - instr->memory_reference]);
 * in `past_time.c` (add the define to the library)
*/
#include "buildconfig.h"
#include "main.h"
#include "wireless.h"
#include "air_flow.h"
#include "chest_compressions.h"
#include "soc/rtc_wdt.h"
#include "r2u2.h"
#include "rv_monitor.h"
#include <stdio.h>

#define PADDING {0,0,0,0}

// Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
BlueToothCommunicator *communicator;
#ifdef DISTANCE_SENSOR
ChestCompression *chest;
#endif  // DISTANCE_SENSOR
#ifdef AIR_FLOW_SENSOR
AirFlow air_flow;
#endif  // AIR_FLOW_SENSOR
Runtime_Monitor monitor;

#ifdef DEBUG
// Debug
int i = 0;
#endif

void print_mem(void * obj, size_t size){
  int i = 0;
  while(i < size){
    printf("%02X|\t", i);
    for(int j = 0; j < 16; j++){
      if(i+j < size)
        printf("%02X ", ((uint8_t*)obj)[i+j]);
      else
        break;
    }
    printf("\n");
    i += 16;
  }
}

void setup() {
  rtc_wdt_feed();

#ifdef DEBUG
  Serial.begin(115200);
#endif

  delay(1);

#ifdef DEBUG
  Serial.println("Começando");
#endif
  
  chest = new ChestCompression();
  // if(!(chest->begin())){
  //   error_handler();
  // }
#ifdef DEBUG
  Serial.println("chest.begin()");
#endif

  // air_flow.begin();
#ifdef DEBUG
  // Serial.println("air_flow.begin();");
#endif

  communicator = new BlueToothCommunicator();
#ifdef DEBUG
  Serial.println("new BlueToothCommunicator();");
#endif
  
  // communicator->begin(nullptr,nullptr);
  communicator->begin(chest,nullptr);//,&air_flow);
#ifdef DEBUG
  Serial.println("communicator->begin(chest,nullptr);//,&air_flow);");
#endif

  // Starts the monitor
  r2u2_status_t status = monitor.begin();
  if(status != R2U2_OK){
    printf("Monitor initialization falied: &d", status);
    while (1);
  }
  Serial.println("monitor.begin()");
  monitor.printInstr();
  
}

void loop() {
  #ifdef DISTANCE_SENSOR

  // Distance
  rtc_wdt_feed();
  // chest->calc_distance();

  #ifdef DEBUG
  // if(i%100 == 0){
    // Serial.print("\nDistance: ");
    // Serial.println(chest->get_distance());
  // }
  #endif  // DEBUG

  #ifdef FREQUENCY_ON_ESP
  // Frequency
  // rtc_wdt_feed();
  // chest->calc_frequency();
  
  #ifdef DEBUG
  if(i%100 == 0){
    // printf("Frequency: \n");
    // Serial.println(chest->get_frequency());
    monitor.tic();
    // while(1);
  }
  i++;
  #endif  // DEBUG 
  #endif  // FREQUENCY_ON_ESP

  #endif  // DISTANCE_SENSOR
  
  // Air Flow
  #ifdef AIR_FLOW_SENSOR
  rtc_wdt_feed();
  air_flow.readFlow();

  #ifdef DEBUG
  if(i%100 == 0){
    Serial.print("Air flow: ");
    Serial.println(air_flow.get_flow());
  }
  #endif  // DEBUG
  #endif  // AIR_FLOW_SENSOR

  if(communicator->request_to_send)
  {
    rtc_wdt_feed();
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