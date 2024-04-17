#ifndef _CHEST_COMPRESSION_H
#define _CHEST_COMPRESSION_H

#include <Adafruit_VL6180X.h>


class ChestCompression{
	public:
		ChestCompression();
		double calc_frequency();
		double calc_distance();
		double get_distance();
		double get_frequency();

	private:
		double last_peak(double distance);
		double last_valley(double distance);

		Adafruit_VL6180X dist_sensor;
		double distance, frequency;
};

#endif