#include <Arduino.h>
#include <stdlib.h>
#include "chest_compressions.h"
#include "statistics.h"

// Peak and valley finder algorithm configuration
#define BUFFER_SIZE 		10
#define DEVIATION_THRESHOLD	5

int calc_frequency(double distance){
  double t0 = last_peak(distance);
  double t1 = last_valley(distance);
  
  double period = 2*abs(t0 - t1);

  return 1/period;
}

/**
 * Se o peito estiver parado em cima, deve sempre considerar pico
*/
double last_peak(double distance){
	static double readings[BUFFER_SIZE];
	static int index = 0;
	static double last_peak = millis();

	// Updates recently read values list
	readings[index%BUFFER_SIZE] = distance;
	index++;

	// Gets time series features
	double mean = calculate_mean(readings,BUFFER_SIZE);
	double deviation = calculate_std_deviation(readings,mean,BUFFER_SIZE);

	// Calculate a new last_peak if deviation is small
	if(deviation > DEVIATION_THRESHOLD){

		// Checks if distance is bigger than recently read values
		bool is_bigger = true;
		for(double value : readings){
			if(distance < value){
				is_bigger = false;
			}
		}

		if(is_bigger){
			last_peak = millis();
		} 
	}

	return last_peak;
}

double last_valley(double distance){
	return 0.0;

}