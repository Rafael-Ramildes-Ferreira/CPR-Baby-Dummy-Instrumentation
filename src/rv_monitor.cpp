#include "rv_monitor.h"
#include <stdio.h>


extern unsigned char specs_bin[];

r2u2_monitor_t r2u2_monitor_memory = MONITOR_MEM_ALLOC;

r2u2_status_t print_verdict(r2u2_instruction_t instr, r2u2_verdict* res);

// /**
//  * @brief Monitor class constructor
//  * 		I want it to be a const expresion
//  */
// Runtime_Monitor::Runtime_Monitor(){
// 	r2u2_monitor_memory = MONITOR_MEM_ALLOC;
// }


/**
 * @brief Initiates the monitor by maping the specifications and output function
 * @return Status: 	0 - No fault
 * 					1 - Fault
 */
r2u2_status_t Runtime_Monitor::begin(void){
	r2u2_monitor_memory.instruction_mem = &specs_bin;
	if(r2u2_monitor_memory.instruction_mem == NULL) {
		perror("Cannot access specification file");
		return R2U2_ERR_OTHER;
	}

	r2u2_monitor_memory.out_func = print_verdict;
	if(r2u2_monitor_memory.out_func == NULL) {
		perror("Cannot assign output function");
		return R2U2_ERR_OTHER;
	}

	return r2u2_init(&r2u2_monitor_memory);
}

void Runtime_Monitor::printInstr(void){
	printf("Test of monitor memory allocation\n");
	printf("* Instr mem: %p\n",r2u2_monitor_memory.instruction_mem);
	printf("* Data: %s\n",r2u2_monitor_memory.instruction_mem);
}

r2u2_status_t print_verdict(r2u2_instruction_t instr, r2u2_verdict* res){
	printf("%d:%u,%s\n", instr.engine_tag, res->time, res->truth ? "T" : "F");

	return R2U2_OK;
}
