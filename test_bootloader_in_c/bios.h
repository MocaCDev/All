#ifndef MocBIOS
#define MocBIOS

// Types
typedef unsigned char   UNS8;
typedef signed char     S8;
typedef unsigned short  UNS16;
typedef signed short    S16;
typedef unsigned int    UNS32;
typedef signed int      S32;
typedef unsigned long   USIZE;
typedef S32             SIZE;

// All registers that are not used will be assigned to this
#define no_data        -1

// Return types
#define failed         -1
#define success         0

// Video Memory
UNS8 *bios_vid_mem = (UNS8 *)0xB8000;

// Print via video memory
#define bios_vid_mem_write(val)     \
    *bios_vid_mem = val;            \
    bios_vid_mem += 2;

void bios_vid_mem_write_str(UNS8 *str)
{
    while(*str != '\0')
    {
        if(*str == '\t')
        {
            for(UNS8 i = 0; i < 4; i++)
            {
                bios_vid_mem_write(' ')
            }
            str++;
            continue;
        }
        if(*str == '\n')
        {
            bios_vid_mem += 32;
            str++;
            continue;
        }
        bios_vid_mem_write((UNS8)*str)
        str++;
    }
}

typedef struct RegInfo
{
    struct {
        S8    al; S8    ah; S16   ax;
        S8    bl; S8    bh; S16   bx;
        S8    cl; S8    ch; S16   cx;
        S8    dl; S8    dh; S16   dx;
    } regs;

    S16   interrupt;
} _RegInfo;

enum registers {
    al = 0x00, ah, ax,
    bl = 0x03, bh, bx,
    cl = 0x06, ch, cx,
    dl = 0x09, dh, dx
};

static _RegInfo reg_info;

void reset_reg_state()
{
    reg_info.interrupt = no_data;

    reg_info.regs.ah = reg_info.regs.al = reg_info.regs.ax = no_data;
    reg_info.regs.bh = reg_info.regs.bl = reg_info.regs.bx = no_data;
    reg_info.regs.ch = reg_info.regs.cl = reg_info.regs.cx = no_data;
    reg_info.regs.dh = reg_info.regs.dl = reg_info.regs.dx = no_data;
}

S8 run_interrupt(_RegInfo ri)
{
    // Figure out what registers have data.
    switch(ri.interrupt)
    {
        case 0x10: {

            break;
        }
        default: return failed;
    }
    
    return success;
}

#endif