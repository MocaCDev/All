#ifndef COMMON
#define COMMON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger/assembler_logger/asm_logger.h"

/*
 * 16bit/32bit assembler. No support for 64bit integers.
 * By default the assembler will assemble to 32bit binary.
 */

typedef unsigned char       UNS8;
typedef signed char         S8;
typedef unsigned short      UNS16;
typedef signed short        S16;
typedef unsigned int        UNS32;
typedef signed int          S32;

typedef UNS8                bool;
#define true                1
#define false               0

#define com_assert(condition, err_msg, ...)         \
    if(!(condition)) {                              \
        LOG_Error(err_msg, ##__VA_ARGS__)           \
        exit(1);                                    \
    }
#define com_error(err_msg, ...)   {                 \
    LOG_Error(err_msg, ##__VA_ARGS__)               \
    exit(1);                                        \
}



#endif