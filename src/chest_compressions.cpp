#include "chest_compressions.h"
#include "utils.h"
#include "main.h"
#include "Arduino.h"


#ifdef FREQUENCY_ON_ESP
#define DEVIATION_THRESHOLD (.03)
#define MEAN_DIST_VALUE     (3.5)
#define HOLD_TIME           (1000)
#define FREQ_DECRESE_SLOPE  (5e-3)
#endif

#define VL6180X_ADDR 0x29


int ChestCompression::begin(){
	this->dist_sensor = Adafruit_VL6180X(VL6180X_ADDR);

#ifdef FREQUENCY_ON_ESP
	this->new_valley_time = millis();
	this->last_peak_time = millis();
	this->last_peak_time = millis();
#endif

  
  #ifdef DISTANCE_SENSOR
  return this->dist_sensor.begin();
  #else
  return 1;
  #endif
}

/**
 * @brief Calculates a new value of this->distance based on sensor readings
 * @returns The calculated distance
*/
double ChestCompression::calc_distance(){
  #ifdef DISTANCE_SENSOR
	// Filter values
	static double yn = 0, yn1 = 0, yn2 = 0, xn = 0, xn1 = 0;

  uint8_t range = this->dist_sensor.readRange();
  uint8_t status = this->dist_sensor.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    xn = range;
  
    // Compute the filtered signal
    yn = 1.656*yn1 - 0.6859*yn2 + 0.01568*xn + 0.01383*xn1;

    // Updates values
    xn1 = xn;
    yn2 = yn1;
    yn1 = yn;

    this->distance = yn/10.0;
    
    #ifdef FREQUENCY_ON_ESP
    this->readings[buffer_index%BUFFER_SIZE] = this->distance;
    this->buffer_index++;
    #endif  // FREQUENCY_ON_ESP
  } else {
		#ifdef DEBUG
			// Serial.print("Distance sensor Error Code: ");
			// Serial.println(dist_sensor.readRangeStatus());
		#endif
	}
  #endif  // DISTANCE_SENSOR

  return this->distance;
}

double ChestCompression::get_distance(){
	return this->distance;
}


#ifdef FREQUENCY_ON_ESP
/**
 * @brief Starts the frequency calculation routine
 * @returns The calculated frequency
*/
double ChestCompression::calc_frequency(){
	// Gets time series features
	this->mean = utils::calculate_mean(this->readings,BUFFER_SIZE);
	this->deviation = utils::calculate_std_deviation(this->readings,this->mean,BUFFER_SIZE);

  // Updates the peak and valleys time
  this->last_peak();
  this->last_valley();

  if(frequency_update_time + HOLD_TIME < millis() && this->frequency > 0){
    this->frequency -= FREQ_DECRESE_SLOPE;
  }

	return this->frequency;
}

/**
 * Se o peito estiver parado em cima, deve sempre considerar pico
*/
double ChestCompression::last_peak(){

  if(this->distance > MEAN_DIST_VALUE){
    // Calculate a new last_peak if deviation is small
    if(this->deviation < DEVIATION_THRESHOLD){
      // Checks if distance is bigger than recently read values
      bool is_bigger = true;
      for(double value : this->readings){
        if(this->distance < value){
          is_bigger = false;
        }
      }

      if(is_bigger){
          // Calculates the frequency between last peak (not the one found here) and last valley
          if(this->frequency_isOld){
            this->update_frequency();
          }
          
          // Records the time in witch it happened          
          last_peak_time = millis();
      } 
    }
  }

	return last_peak_time;
}

double ChestCompression::last_valley(){
	if(this->distance < MEAN_DIST_VALUE){
		// Calculate a new last_valley if deviation is small
		if(this->deviation < DEVIATION_THRESHOLD){
			// Checks if distance is smaller than recently read values
			bool is_smaller = true;
			for(double value : this->readings){
        if(this->distance > value){
          is_smaller = false;
        }
			}

			if(is_smaller){
        // Calculates the frequency between last peak and last valley (not the one found here)
        if(this->last_peak_time > this->new_valley_time){
          this->old_valley_time = this->new_valley_time;
          this->frequency_isOld = true;
        }

        // Records the time in witch it happened
			  this->new_valley_time = millis();
			} 
		}
	}

		return this->new_valley_time;
}
/**
 * @brief Calculates a new value of this->frequency based on sensor readings
 * @returns The calculated frequency
*/
double ChestCompression::update_frequency(){
	double period = this->new_valley_time - this->old_valley_time;

	this->frequency = 1e3/period;
  this->frequency_update_time = millis();
  this->frequency_isOld = false;
	return this->frequency;
}

double ChestCompression::get_frequency(){
	return this->frequency;
}
#endif