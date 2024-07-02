#ifndef _CHEST_COMPRESSION_H
#define _CHEST_COMPRESSION_H

#include <Adafruit_VL6180X.h>
#include "buildconfig.h"


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
		double last_peak(double distance);
		double last_valley(double distance);

    	double mean,deviation;
    	double last_valley_time,last_peak_time;
		double frequency;
	#endif

		Adafruit_VL6180X dist_sensor;
		double distance;
};

#endif