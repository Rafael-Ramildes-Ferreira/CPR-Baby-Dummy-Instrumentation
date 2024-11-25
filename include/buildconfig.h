#ifndef _BUILDCONFIG_H
#define _BUILDCONFIG_H

#define DEBUG
#ifdef DEBUG
#include <Arduino.h>
#endif

// #define DISTANCE_SENSOR
// #define FREQUENCY_ON_ESP
#define AIR_FLOW_SENSOR


#ifndef DISTANCE_SENSOR
#undef FREQUENCY_ON_ESP
#endif

#if defined(DISTANCE_SENSOR) && defined(AIR_FLOW_SENSOR)
#define AVAILABLE_SENSORS	chest,&air_flow
#elif defined(DISTANCE_SENSOR) && !defined(AIR_FLOW_SENSOR)
#define AVAILABLE_SENSORS	chest,nullptr
#elif !defined(DISTANCE_SENSOR) && defined(AIR_FLOW_SENSOR)
#define AVAILABLE_SENSORS	nullptr,&air_flow
#else
#define AVAILABLE_SENSORS	nullptr,nullptr
#endif

#endif