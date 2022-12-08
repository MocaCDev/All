#ifndef ecbp_file_format_common_H
#define ecbp_file_format_common_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* My own types. */
typedef unsigned char		uint8;
typedef signed char		int8;
typedef unsigned short		uint16;
typedef signed short		int16;
typedef unsigned int		uint32;
typedef signed int		int32;
typedef unsigned long		usize;
typedef signed long		size;

/* Boolean. */
typedef uint8			bool;
#define true			1
#define false			0

/* Printing. */
#define perror(msg, ...)	\
	fprintf(stderr, msg, ##__VA_ARGS__);

/* Fast conditions :) */
#define ecbp_assert(cond, err_msg, ...) 	\
	if(!(cond)) {				\
		perror(err_msg, ##__VA_ARGS__);	\
	}
#define ecbp_assert_eq(t1, t2, err_msg, ...)	\
	if(t1 != t2) {				\
		perror(err_msg, ##__VA_ARGS__);	\
	}

#endif
