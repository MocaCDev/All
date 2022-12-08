#include "util.h"
#include "GUI/gui.h"
#include "ata_pio.h"
#include "mstdio.h"

__attribute__((section("kernel_entry"))) void _start(void) {

    init_GUI();

    while(1) {}
}
