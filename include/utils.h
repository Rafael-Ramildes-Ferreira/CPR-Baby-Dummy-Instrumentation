#ifndef _STATISTICS_H
#define _STATISTICS_H

class utils{
	public:
		static double calculate_mean(const double array[],int size);
		static double calculate_std_deviation(const double array[],double mean,int size);
};

#endif