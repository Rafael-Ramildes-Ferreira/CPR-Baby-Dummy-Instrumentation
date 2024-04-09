#include <stdlib.h>
#include "chest_compressions.h"

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
	return 0.0;
}

double last_valley(double distance){
	return 0.0;

}