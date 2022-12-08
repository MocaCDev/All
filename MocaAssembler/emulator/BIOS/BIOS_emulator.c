#include "BIOS_emulator.h"

void check_boot_sig(FILE *device)
{
    fseek(device, 510, SEEK_SET);

    UNS8 buff[2];
    fread(buff, 2, sizeof(UNS8), device);

    // Check signature.
    if(buff[0] == BOOT_SECT_SIG >> 8) {
        if(buff[1] == (UNS8)BOOT_SECT_SIG & 0xFF) {
            fseek(device, 0, SEEK_SET);

            interrupts = calloc(1, sizeof(*interrupts));
            return;
        }
    }

    com_error("Boot signature of device is not valid.\n")
}