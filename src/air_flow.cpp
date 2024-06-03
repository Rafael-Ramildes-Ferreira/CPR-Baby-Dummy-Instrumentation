#include "air_flow.h"

int AirFlow::begin()
{
	flowSensor = new SFM3X00(SFM_ADDR);
	flowSensor->begin();

	return 0;
}

double AirFlow::get_flow()
{
	return flowSensor->readFlow();
	// return 0.0;
}