#ifndef _CHEST_COMPRESSION_H
#define _CHEST_COMPRESSION_H

#include <Adafruit_VL6180X.h>
#include "buildconfig.h"

// Peak and valley finder algorithm configuration
#define BUFFER_SIZE 		10
#define DEVIATION_THRESHOLD	0.05


class ChestCompression{
	public:
    	int begin();
		double calc_distance();
		double get_distance();
	#ifdef FREQUENCY_ON_ESP
		double calc_frequency();
		double get_frequency();
	#endif

	private:
	#ifdef FREQUENCY_ON_ESP
		double last_peak();
		double last_valley();
		double update_frequency();

		double readings[BUFFER_SIZE];
    	unsigned int buffer_index;

    	double mean,deviation;
    	double last_valley_time,last_peak_time;
		double frequency;
	#endif

		Adafruit_VL6180X dist_sensor;
		double distance;
};

#endif