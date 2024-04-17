#ifndef _MAIN_H
#define _MAIN_H

/* Exported variable */
class ChestCompression{
	public:
		double get_distance();
		double get_frequency();
	private:
		double distance, frequency;
}; // Temporary
extern ChestCompression chest; // Temporary

/* Exported functions */
void error_handler();

#endif