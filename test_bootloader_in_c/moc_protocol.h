#ifndef MocProto
#define MocProto

/* Make sure we are 16bit. */
asm(".code16gcc\n");
#include "bios.h"

void try(S8 status)
{
    // If the status is a success, just return
    if(status == success) return;

    // Otherwise, error
    asm("mov al, 'E'\nmov ah, 0x0E\nint 0x10\n");
    asm("cli;hlt");
}

#endif