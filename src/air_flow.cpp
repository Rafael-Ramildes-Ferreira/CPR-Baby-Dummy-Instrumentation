#include "buildconfig.h"

#ifdef AIR_FLOW_SENSOR
#include "air_flow.h"

/**
 * @brief Starts the air flow measuring class, 
 * 		creating the lbrary's object an starting the communication
 * @return Error code (not yet implemented)
 */
int AirFlow::begin()
{
	flowSensor = new SFM3X00(SFM_ADDR);
	flowSensor->begin();

	return 0;
}

/**
 * @brief Gets the flow value from the sensor, which is a more expensive operation than
 * 		grabing it from memory
 * @return The new found flow value
 */
double AirFlow::readFlow()
{
	this->flow = flowSensor->readFlow();
	return this->flow;
}

/**
 * @brief Gets the flow value already stored in memory 
 * 		(this should reduce the time to access this value, since it doesn't have to communicate with I2C)
 * @return The stored flow value
 */
double AirFlow::get_flow()
{
	return this->flow;
}

#endif	// AIR_FLOW_SENSOR