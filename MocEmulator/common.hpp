#ifndef MocEmulator_Common
#define MocEmulator_Common
#include <iostream>
#include <string.h>
#include <stdlib.h> 	// exit(), EXIT_FAILURE

// General types
using R8 = char;
using UNS8 = unsigned char;
using S8 = signed char;
using UNS16 = unsigned short;
using S16 = signed short;
using UNS32 = unsigned int;
using S32 = signed int;

// Used to get sizes
using SIZE = unsigned long;
using LSIZE = unsigned long long;

// Make my life easier :)
#define moc_error(line, msg, ...)						\
	fprintf(stderr, "Error, Line: %d\n\t", line); 		\
	fprintf(stderr, msg, ##__VA_ARGS__); 				\
	exit(EXIT_FAILURE);
#define moc_assert(line, cond, msg, ...) 				\
	if(!(cond)) 	{ 									\
		moc_error(line, msg, ##__VA_ARGS__) 			\
	}

#endif
