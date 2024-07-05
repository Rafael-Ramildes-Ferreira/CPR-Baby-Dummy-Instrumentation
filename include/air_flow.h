#ifndef __AIR_FLOW_H
#define __AIR_FLOW_H


#include <SFM3X00.h>

#define SFM_ADDR 0x40

/**
 * @brief Deals with the air flow sensor handling
*/
class AirFlow
{
	private:
		SFM3X00 *flowSensor;
		double flow;
	public:
		int begin();
		double readFlow();
		double get_flow();
};

#endif