#include "disassemble.h"

void init_device(const char *_device){
    device = fopen(_device, "rb");

    if(!device)
        com_error("Error opening device: %s", _device);

    // Initialization
    interrupts  = calloc(1, sizeof(*interrupts));
    reg_info    = calloc(1, sizeof(*reg_info));

    check_boot_sig(device);
}