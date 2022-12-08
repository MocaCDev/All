#ifndef ecbp_file_format_H
#define ecbp_file_format_H
#include "common.h"

/* ECBP file format:
 *
 * 	p pkg_name(given 10 bytes) package_size(4 bytes)
 * */

/* Data to fill ECBP format. */
typedef struct ECBP_format
{
	// 'p'
	uint8		start;

	// Padding
	uint8		pad1;

	// Package name
	uint8		pkg_name[14];

	// Package size(in bytes)
	uint32		pkg_size;
} __attribute__((packed)) _ECBP_format;

#endif
