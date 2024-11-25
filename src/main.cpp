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
#include "timerinterruption.h"
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

TimerInterruption trace_export(TIMER_3,30);

void request_trace(void);


bool trace_requested = false;
long int monitor_timeStamp = -1;
bool data_sent = false;
double value = 0.0;


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
  /* Sensors read */
  #ifdef DISTANCE_SENSOR
  // Distance
  rtc_wdt_feed();
  chest->calc_distance();
  #endif  // DISTANCE_SENSOR

  #ifdef FREQUENCY_ON_ESP
  // Frequency
  rtc_wdt_feed();
  chest->calc_frequency();  
  #endif  // FREQUENCY_ON_ESP
  
  // Air Flow
  #ifdef AIR_FLOW_SENSOR
  rtc_wdt_feed();
  air_flow.readFlow();
  #endif  // AIR_FLOW_SENSOR


  /* Trace data export */
  if(trace_requested){
    monitor_timeStamp++;

    trace_requested = false;

    monitor.update_float_signal(0,value);
    // value = air_flow.get_flow();
    value = chest->get_distance();
    monitor.update_float_signal(1,value);
    monitor.update_bool_signal(2,data_sent);
    data_sent = false;
    monitor.tic();
    printf("----------");
    
    #ifdef DEBUG
    #ifdef DISTANCE_SENSOR
    printf("\n[%d] Distance: %lf\n",monitor_timeStamp,chest->get_distance());
    #endif  // DISTANCE_SENSOR
    
    #ifdef FREQUENCY_ON_ESP
    printf("[%d] Frequency: %lf\n",monitor_timeStamp,chest->get_frequency());
    #endif  // FREQUENCY_ON_ESP

    #ifdef AIR_FLOW_SENSOR
    printf("[%d] Air_flow: %lf\n",monitor_timeStamp, air_flow.get_flow());
    #endif  // AIR_FLOW_SENSOR
    #endif  // DEBUG
  }


  /* Wireless comunication */
  if(communicator->request_to_send)
  {
    data_sent = true;
    rtc_wdt_feed();
    communicator->update();
    #ifdef DEBUG
    printf("[%d] Request_To_Send\n",monitor_timeStamp);
    #endif  // DEBUG
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