#ifndef _MAIN_H
#define _MAIN_H

#include "buildconfig.h"

#define MY_DEBUG_PRINT(...) do{ printf(__VA_ARGS__ ); } while( false )

/* Exported functions */
void error_handler();

#endif