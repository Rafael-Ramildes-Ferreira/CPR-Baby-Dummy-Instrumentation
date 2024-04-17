#include <Arduino.h>
#include <stdlib.h>
#include "chest_compressions.h"
#include "statistics.h"
#include "main.h"

// Peak and valley finder algorithm configuration
#define BUFFER_SIZE 		10
#define DEVIATION_THRESHOLD	5


/**
 * @brief Chest compression constructor
*/
ChestCompression::ChestCompression(){
	dist_sensor = Adafruit_VL6180X();

	if (!dist_sensor.begin()) {
		error_handler();
	}
}

/**
 * @brief Calculates a new value of this->distance based on sensor readings
 * @returns The calculated distance
*/
double ChestCompression::calc_distance(){
	// Filter values
	static double yn = 0, yn1 = 0, yn2 = 0, xn = 0, xn1 = 0;

	// Reads the sensor
	uint8_t range = dist_sensor.readRange();
	if (dist_sensor.readRangeStatus() == VL6180X_ERROR_NONE){
		xn = range;
	} else {
		// xn = UINT8_MAX; // Used to be 200 (is that the maximum distance? [todo])
		return yn; // Ignores failed reading
	}

	// Compute the filtered signal
	yn = 1.656*yn1 - 0.6859*yn2 + 0.01568*xn + 0.01383*xn1;

	// Updates values
	// delay(1); // Why delay?
	xn1 = xn;
	yn2 = yn1;
	yn1 = yn;

	this->distance = yn/10.0;
	return this->distance;
}

/**
 * @brief Calculates a new value of this->frequency based on sensor readings
 * @returns The calculated frequency
*/
double ChestCompression::calc_frequency(){
	double t0 = this->last_peak(this->distance);
	double t1 = this->last_valley(this->distance);

	double period = 2*abs(t0 - t1);

	this->frequency = 1/period;
	return this->frequency;
}

/**
 * Se o peito estiver parado em cima, deve sempre considerar pico
*/
double ChestCompression::last_peak(double distance){
	static double readings[BUFFER_SIZE];
	static int index = 0;
	static double last_peak = millis();

	// Updates recently read values list
	readings[index%BUFFER_SIZE] = distance;
	index++;

	// Gets time series features
	double mean = calculate_mean(readings,BUFFER_SIZE);
	double deviation = calculate_std_deviation(readings,mean,BUFFER_SIZE);
  	Serial.print("Desvio: ");
  	Serial.println(deviation);

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
			Serial.println("Pico!!  <----");
		} 
	}

	return last_peak;
}

double ChestCompression::last_valley(double distance){
	static double readings[BUFFER_SIZE];
	static int index = 0;
	static double last_valley = millis();

	// Updates recently read values list
	readings[index%BUFFER_SIZE] = distance;
	index++;

	// Gets time series features
	double mean = calculate_mean(readings,BUFFER_SIZE);
	double deviation = calculate_std_deviation(readings,mean,BUFFER_SIZE);

	// Calculate a new last_valley if deviation is small
	if(deviation > DEVIATION_THRESHOLD){

		// Checks if distance is smaller than recently read values
		bool is_smaller = true;
		for(double value : readings){
			if(distance > value){
				is_smaller = false;
			}
		}

		if(is_smaller){
			last_valley = millis();
			Serial.println("Vale!!  <----");
		} 
	}

	return last_valley;
}