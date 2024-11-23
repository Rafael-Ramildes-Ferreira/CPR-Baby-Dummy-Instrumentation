#ifndef __RV_MONITOR_H
#define __RV_MONITOR_H


#include <r2u2.h>

#include "user_bounds.h"


#define MONITOR_MEM_ALLOC \
{ \
    0, 0, R2U2_MONITOR_PROGRESS_FIRST_LOOP, \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [MAX_INST_LENGTH]){0})), \
    (reinterpret_cast<r2u2_instruction_t (*)[]>(&(r2u2_instruction_t [NUM_OF_INTRUCTIONS]){0})), \
    NULL, NULL, \
    (reinterpret_cast<void* (*)[]>(&(void*[NUM_OF_SIGNALS]){0})), \
    (reinterpret_cast<r2u2_value_t (*)[]>(&(r2u2_value_t [NUM_OF_BZ_INSTRUCTION]){0})), \
    {(reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [NUM_OF_AT_INSTRUCTION]){0})), (reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [NUM_OF_AT_INSTRUCTION]){0}))}, \
    {(reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [NUM_OF_INTRUCTIONS]){0})), (reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [NUM_OF_INTRUCTIONS]){0}))}, \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [NUN_OF_BOXQ_BYTES]){0})), \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [NUN_OF_SCQ_BYTES]){0})), \
    (reinterpret_cast<r2u2_at_filter_aux_data_t (*)[]>(&(r2u2_at_filter_aux_data_t [NUM_OF_AT_INSTRUCTION]){0})), \
  }

class Runtime_Monitor{
  public:
    // Runtime_Monitor(void);
    r2u2_status_t begin(void);
    r2u2_status_t tic(void);
    void printInstr(void);
};
#endif	//  __RV_MONITOR_H