#include "moc_protocol.h"

void __attribute__((section("__start"))) main()
{
    // Make sure no data is stored in regs.
    //reset_reg_state();

    //bios_vid_mem_write('a')
    //bios_vid_mem_write('i')
    //bios_vid_mem_write('d')

    //refresh();

    //bios_vid_mem_write('a')

    __asm__ __volatile__("mov ax, 0x07E0\nmov es, ax\nxor bx, bx\nmov ah, 0x02\nmov al, 0x6\nmov ch, 0x00\nmov cl, 0x02\nmov dh, 0x00\nmov dl, 0x80\nint 0x13\njmp 0x0:0x7E00\n");
    bios_vid_mem_write('a')
}