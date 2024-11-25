#include "rv_monitor.h"

#include "engines/mltl/mltl.h"

#include "utils.h"

#include <stdio.h>
#include <string.h>


extern unsigned char specs_bin[];

r2u2_monitor_t r2u2_monitor_struct = MONITOR_MEM_ALLOC;

r2u2_status_t print_verdict(r2u2_instruction_t instr, r2u2_verdict* res);

// /**
//  * @brief Monitor class constructor
//  * 		I want it to be a const expresion
//  */
// Runtime_Monitor::Runtime_Monitor(){
// 	r2u2_monitor_struct = MONITOR_MEM_ALLOC;
// }


/**
 * @brief Initiates the monitor by maping the specifications and output function
 * @return Status: 	0 - No fault
 * 					1 - Fault
 */
r2u2_status_t Runtime_Monitor::begin(void){
	r2u2_monitor_struct.instruction_mem = &specs_bin;
	if(r2u2_monitor_struct.instruction_mem == NULL) {
		perror("Cannot access specification file");
		return R2U2_ERR_OTHER;
	}

	r2u2_monitor_struct.out_func = print_verdict;
	if(r2u2_monitor_struct.out_func == NULL) {
		perror("Cannot assign output function");
		return R2U2_ERR_OTHER;
	}

	(*(r2u2_monitor_struct.signal_vector))[0] = malloc(sizeof(char)*8);
	(*(r2u2_monitor_struct.signal_vector))[1] = malloc(sizeof(char)*8);
	(*(r2u2_monitor_struct.signal_vector))[2] = malloc(sizeof(char)*8);
	(*(r2u2_monitor_struct.signal_vector))[3] = malloc(sizeof(char)*8);
	(*(r2u2_monitor_struct.signal_vector))[4] = malloc(sizeof(char)*1);

	return r2u2_init(&r2u2_monitor_struct);
	// return R2U2_OK;
}

void Runtime_Monitor::update_float_signal(uint8_t index,float value){
	sprintf((char*)((*(r2u2_monitor_struct.signal_vector))[index]),"%8.6f,\n",value);
}

void Runtime_Monitor::update_bool_signal(uint8_t index,bool value){
	sprintf((char*)((*(r2u2_monitor_struct.signal_vector))[index]),"%d,\n",value);
}

r2u2_status_t Runtime_Monitor::tic(void){
	return r2u2_tic(&r2u2_monitor_struct);
}

void Runtime_Monitor::printInstr(void){
	printf("Test of monitor memory allocation\n");
	printf("* Instr mem: %p\n",r2u2_monitor_struct.instruction_mem);
	printf("* Data: %s\n",r2u2_monitor_struct.instruction_mem);
}

r2u2_status_t print_verdict(r2u2_instruction_t instr, r2u2_verdict* res){
	printf("%d:%u,%s\n", ((r2u2_mltl_instruction_t*)instr.instruction_data)->op2.value, res->time, res->truth ? "T" : "F");

	return R2U2_OK;
}
