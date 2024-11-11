#include "rv_monitor.h"
#include "stdio.h"


extern unsigned char specs_bin[];

r2u2_monitor_t r2u2_monitor_memory = MONITOR_MEM_ALLOC;

// /**
//  * @brief Monitor class constructor
//  * 		I want it to be a const expresion
//  */
// Runtime_Monitor::Runtime_Monitor(){
// 	r2u2_monitor_memory = MONITOR_MEM_ALLOC;
// }


int Runtime_Monitor::begin(void){
	r2u2_monitor_memory.instruction_mem = &specs_bin;

	return 0;
}

void Runtime_Monitor::printInstr(void){
	printf("Test of monitor memory allocation\n");
	printf("Instr mem: %p\n",r2u2_monitor_memory.instruction_mem);
	printf("Data: %s\n",r2u2_monitor_memory.instruction_mem);
}
