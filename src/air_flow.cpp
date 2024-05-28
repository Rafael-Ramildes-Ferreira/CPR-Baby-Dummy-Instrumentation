#include "air_flow.h"

int AirFlow::begin()
{
	flowSensor.begin();

	return 0;
}

double AirFlow::get_flow()
{
	return flowSensor.readFlow();
}