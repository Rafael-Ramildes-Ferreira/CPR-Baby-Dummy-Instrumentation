#include <cmath>

double calculate_mean(const double array[],int size){
	double sum = 0.0;
    for(int i = 0;i < size; i++){
        sum += array[i];
    }

	return sum/size;
}

double calculate_std_deviation(const double array[],double mean,int size) {
	double sum = 0;
    for(int i = 0;i < size; i++){
		sum = sum + (array[i] - mean)*(array[i] - mean);
	}
    return std::sqrt(sum/size);
}