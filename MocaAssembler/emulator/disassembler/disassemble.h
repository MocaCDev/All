#ifndef DISASSEMBLE
#define DISASSEMBLE
#include "../../common.h"

/*
 *  Assuming since user is assembling to 16bit binary,
 *  some BIOS interrupts will be used. We want to go ahead
 *  and store the interrupt information for the emulator.
 * */
#include "../BIOS/BIOS.h"
#include "../BIOS/BIOS_emulator.h"

/*
 *  Data for the BIOS interrupts will be filled out by reading
 *  from a device(in our case, possibly the boot sector program.)
 * */
static FILE* device;

#define CLOSE_DEVICE(dev)       \
    fclose(dev);
void init_device(const char *_device);

#endif