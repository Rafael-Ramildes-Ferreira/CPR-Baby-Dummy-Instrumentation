#ifndef __RV_MONITOR_H
#define __RV_MONITOR_H


#include "r2u2.h"


// Using values from example cav.mltl just as test
// what I don't know what means I consider the worst case
#define rbMAX_INST_LENGTH		R2U2_MAX_INST_LEN
#define rbNUM_OF_INTRUCTIONS 	41 // ft + bz
#define rbNUM_OF_SIGNALS		8
#define rbNUM_OF_BZ_INSTRUCTION	26
#define rbNUM_OF_AT_INSTRUCTION	1 // It cannot be zero :p
#define rbNUN_OF_BOXQ_BYTES		1 // It cannot be zero :p
#define rbNUN_OF_SCQ_BYTES		146


#define MONITOR_MEM_ALLOC \
{ \
    0, 0, R2U2_MONITOR_PROGRESS_FIRST_LOOP, \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [rbMAX_INST_LENGTH]){0})), \
    (reinterpret_cast<r2u2_instruction_t (*)[]>(&(r2u2_instruction_t [rbNUM_OF_INTRUCTIONS]){0})), \
    NULL, NULL, \
    (reinterpret_cast<void* (*)[]>(&(void*[rbNUM_OF_SIGNALS]){0})), \
    (reinterpret_cast<r2u2_value_t (*)[]>(&(r2u2_value_t [rbNUM_OF_BZ_INSTRUCTION]){0})), \
    {(reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [rbNUM_OF_AT_INSTRUCTION]){0})), (reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [rbNUM_OF_AT_INSTRUCTION]){0}))}, \
    {(reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [rbNUM_OF_INTRUCTIONS]){0})), (reinterpret_cast<r2u2_bool (*)[]>(&(r2u2_bool [rbNUM_OF_INTRUCTIONS]){0}))}, \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [rbNUN_OF_BOXQ_BYTES]){0})), \
    (reinterpret_cast<uint8_t (*)[]>(&(uint8_t [rbNUN_OF_SCQ_BYTES]){0})), \
    (reinterpret_cast<r2u2_at_filter_aux_data_t (*)[]>(&(r2u2_at_filter_aux_data_t [rbNUM_OF_AT_INSTRUCTION]){0})), \
  }

#endif	//  __RV_MONITOR_H