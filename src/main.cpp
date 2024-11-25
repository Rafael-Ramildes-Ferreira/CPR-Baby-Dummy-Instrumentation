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
#include "buildconfig.h"
#include "main.h"
#include "wireless.h"
#include "air_flow.h"
#include "chest_compressions.h"
#include "timerinterruption.h"
#include "soc/rtc_wdt.h"


// Adafruit_VL6180X dist_sensor = Adafruit_VL6180X();
BlueToothCommunicator *communicator;
#ifdef DISTANCE_SENSOR
ChestCompression *chest;
#endif  // DISTANCE_SENSOR
#ifdef AIR_FLOW_SENSOR
AirFlow air_flow;
#endif  // AIR_FLOW_SENSOR

TimerInterruption trace_export(TIMER_3,30);

void request_trace(void);


bool trace_requested = false;
unsigned int monitor_timeStamp = 0;


void setup() {
  rtc_wdt_feed();

#ifdef DEBUG
  Serial.begin(115200);
#endif

  delay(1);

#ifdef DEBUG
  Serial.println("Começando");
#endif
  
  #ifdef DISTANCE_SENSOR
  chest = new ChestCompression();
  chest->begin();
  // if(!(chest->begin())){
  //   error_handler();
  // }
#ifdef DEBUG
  Serial.println("chest.begin()");
#endif
  #endif

  #ifdef AIR_FLOW_SENSOR
  air_flow.begin();
#ifdef DEBUG
  Serial.println("air_flow.begin();");
#endif
  #endif

  communicator = new BlueToothCommunicator();
#ifdef DEBUG
  Serial.println("new BlueToothCommunicator();");
#endif

  trace_export.set_timer_interrupt(request_trace);
#ifdef DEBUG
  Serial.println("trace_export.set_timer_interrupt(request_trace);");
#endif
  
  communicator->begin(AVAILABLE_SENSORS);
#ifdef DEBUG
  Serial.println("communicator->begin(AVAILABLE_SENSORS);");
#endif
}

void loop() {
  #ifdef DISTANCE_SENSOR

  // Distance
  rtc_wdt_feed();
  chest->calc_distance();

  #ifdef FREQUENCY_ON_ESP
  // Frequency
  rtc_wdt_feed();
  chest->calc_frequency();  
  #endif  // FREQUENCY_ON_ESP

  #endif  // DISTANCE_SENSOR
  
  // Air Flow
  #ifdef AIR_FLOW_SENSOR
  rtc_wdt_feed();
  air_flow.readFlow();
  #endif  // AIR_FLOW_SENSOR

  #ifdef DEBUG
  if(trace_requested){
    trace_requested = false;
    
  #ifdef DISTANCE_SENSOR
    printf("\n[%d] Distance: %lf\n",monitor_timeStamp,chest->get_distance());
  #endif  // DISTANCE_SENSOR
    
  #ifdef FREQUENCY_ON_ESP
    printf("[%d] Frequency: %lf\n",monitor_timeStamp,chest->get_frequency());
  #endif  // FREQUENCY_ON_ESP

  #ifdef AIR_FLOW_SENSOR
    printf("[%d] Air_flow: %lf\n",monitor_timeStamp, air_flow.get_flow());
  #endif  // AIR_FLOW_SENSOR
  }
  #endif  // DEBUG

  if(communicator->request_to_send)
  {
    rtc_wdt_feed();
    communicator->update();
    printf("[%d] Request_To_Send\n",monitor_timeStamp);
  }
}


void IRAM_ATTR request_trace(void){
  trace_requested = true;
}


void error_handler(){
  while (1)
  {
    /* Does nothing */
    /* It could blink a led or something, but the watch dog will probably restart it*/
    Serial.println("ERROR!!!");
  }
  
}