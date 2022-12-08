#include "bios.h"

void reset_reg_state()
{
    reg_info.interrupt = no_data;

    reg_info.regs.ah = reg_info.regs.al = reg_info.regs.ax = no_data;
    reg_info.regs.bh = reg_info.regs.bl = reg_info.regs.bx = no_data;
    reg_info.regs.ch = reg_info.regs.cl = reg_info.regs.cx = no_data;
    reg_info.regs.dh = reg_info.regs.dl = reg_info.regs.dx = no_data;
}

UNS8 run_interrupt(_RegInfo ri)
{
    // Figure out what registers have data.
    /*if(ri.interrupt == 0x10)
    {
        __asm__("mov al, 'a'\nmov ah, 0x0e\nint 0x10");
    }*/
    __asm__ volatile("mov al, 'a'\nmov ah, 0x0e\nint 0x10");

    //return 0;
}

// Here just to get rid of linker error :)
//void __attribute__((unused)) main() {}