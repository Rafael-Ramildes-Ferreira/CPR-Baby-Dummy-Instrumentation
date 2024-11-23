#ifndef __USER_BOUNDS_H
#define __USER_BOUNDS_H


/**
 * @defgroup Monitor size
 * @brief Number of elements in each monitor struct member array
 * @{
 */
// Using values from example cav.mltl just as test
// what I don't know what means I consider the worst case
#define MAX_INST_LENGTH		    4000
#define NUM_OF_INTRUCTIONS 	  41 // ft + bz
#define NUM_OF_SIGNALS		    8
#define NUM_OF_BZ_INSTRUCTION	26
#define NUM_OF_AT_INSTRUCTION	1
#define NUN_OF_BOXQ_BYTES		  128*8 // 8 is the size of the structure tha this will be casted as
#define NUN_OF_SCQ_BYTES		  146
/**
 * @}
 */

#endif	//__USER_BOUNDS_H