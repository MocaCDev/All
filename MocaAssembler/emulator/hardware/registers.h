#ifndef REGISTERS
#define REGISTERS
#include "../BIOS/BIOS.h"
#include "../../common.h"

/*
 *  The overall emulator will be working with registers for the entirity
 *  of the program.
 * 
 *  This header file dwells in the /hardware folder due to the fact
 *  registers are located on the computer and is how humans "talk"
 *  with the hardware.
 * */

#define TOTAL_16bit_REGS    22
#define TOTAL_32bit_REGS    31 // all 16bit/8bit registers supported
                               // in 32bit assembly

/*
 *  "Global" state of all 16bit registers throughout the emulation.
 *
 *  Developer Note:
 *      16bit registers can and sometimes are used in 32bit assembly.
 *      When emulating, keep in mind that the emulator may still
 *      have to reference/assign/read 16bit registers.
 * */
typedef struct GlobalRegs16bit {
    UNS16       ax;
    UNS8        ah;
    UNS8        al;
    UNS16       bx;
    UNS8        bh;
    UNS8        bl;
    UNS16       cx;
    UNS8        ch;
    UNS8        cl;
    UNS16       dx;
    UNS8        dh;
    UNS8        dl;
    UNS16       es;
    UNS16       ds;
    UNS16       bp;
    UNS16       sp;
    UNS16       ss;
    UNS16       di;
    UNS16       si;
    UNS16       ip;
    UNS16       fs;
    UNS16       gs;
} _Global16bitRegs;

/*
 *  "Global" state of all 32bit registers throughout emulation
 * */
typedef struct GlobalRegs32bit {
    UNS32               eax;
    UNS32               ebx;
    UNS32               ecx;
    UNS32               edx;
    UNS32               edi;
    UNS32               esi;
    UNS32               ebp;
    UNS32               esp;
    UNS32               eip;  
} _Global32bitRegs;


/*
 *  Declaring static variables for register information.
 * */
static _Global16bitRegs *r_16bit_info;
static _Global32bitRegs *r_32bit_info;

/*
 *  Macros to assign/use 16bit registers without the need of
 *  accessing them through the struct.
 * */
#define _ax      ((_Global16bitRegs *) r_16bit_info)->ax
#define _ah      ((_Global16bitRegs *) r_16bit_info)->ah
#define _al      ((_Global16bitRegs *) r_16bit_info)->al
#define _bx      ((_Global16bitRegs *) r_16bit_info)->bx
#define _bh      ((_Global16bitRegs *) r_16bit_info)->bh
#define _bl      ((_Global16bitRegs *) r_16bit_info)->bl
#define _cx      ((_Global16bitRegs *) r_16bit_info)->cx
#define _ch      ((_Global16bitRegs *) r_16bit_info)->ch
#define _cl      ((_Global16bitRegs *) r_16bit_info)->cl
#define _dx      ((_Global16bitRegs *) r_16bit_info)->dx
#define _dh      ((_Global16bitRegs *) r_16bit_info)->dh
#define _dl      ((_Global16bitRegs *) r_16bit_info)->dl
#define _es      ((_Global16bitRegs *) r_16bit_info)->es
#define _ds      ((_Global16bitRegs *) r_16bit_info)->ds
#define _bp      ((_Global16bitRegs *) r_16bit_info)->bp
#define _sp      ((_Global16bitRegs *) r_16bit_info)->sp
#define _ss      ((_Global16bitRegs *) r_16bit_info)->ss
#define _di      ((_Global16bitRegs *) r_16bit_info)->di
#define _si      ((_Global16bitRegs *) r_16bit_info)->si
#define _ip      ((_Global16bitRegs *) r_16bit_info)->ip
#define _fs      ((_Global16bitRegs *) r_16bit_info)->fs
#define _gs      ((_Global16bitRegs *) r_16bit_info)->gs

/*
 *  Macros to assign/use 32bit registers without the need of
 *  accessing them through the struct.
 * */
#define _eax     ((_Global32bitRegs *) r_32bit_info)->eax
#define _ebx     ((_Global32bitRegs *) r_32bit_info)->ebx
#define _ecx     ((_Global32bitRegs *) r_32bit_info)->ecx
#define _edx     ((_Global32bitRegs *) r_32bit_info)->edx
#define _ebp     ((_Global32bitRegs *) r_32bit_info)->ebp
#define _esp     ((_Global32bitRegs *) r_32bit_info)->esp
#define _esi     ((_Global32bitRegs *) r_32bit_info)->esi
#define _eip     ((_Global32bitRegs *) r_32bit_info)->eip
#define _edi     ((_Global32bitRegs *) r_32bit_info)->edi

/*
 *  init_registers() -> allocating memory and setting all registers to
 *                      zero(there "initial" state)
 * 
 *  Developer Note:
 *      the 16bit register dl(in 16bit assembly mode) needs to be
 *      set to the drive number(0x00 = floppy, 0x80 = hard disk).
 *      This will be done by passing `-type=disk` as a CLA.
 * */
void init_registers()
{
    r_16bit_info = calloc(1, sizeof(*r_16bit_info));
    r_32bit_info = calloc(1, sizeof(*r_32bit_info));

    // 16bit regs
    _ax = _ah = _al = 0;
    _bx = _bh = _bl = 0;
    _cx = _ch = _cl = 0;
    _dx = _dh = _dl = 0;
    _es = _ds = _ip = _bp = _sp = _ss = _si = _di = _fs = _gs = 0;

    // 32bit regs
    _eax = _ebx = _ecx = _edx = 0;
    _edi = _esi = _ebp = _esp = 0;
}

/*
 *  assign_register:
 *  Input: reg(register to assign), value.
 *      assign_register will assign just one register.
 *      Any parent/child register will be assigned with this
 * */
#define assign_register(reg, value)    reg = value

/*
 *  update_parent_reg:
 *  Input: p(parent reg), c1(child reg 1), c2(child reg 2)
 *      Parent Registers: ax, bx, cx, dx.
 *      Update parent register based on the value of the
 *      higher-half/lower-half registers.
 * */
#define update_parent_reg(p, c1, c2)      p = (c1 << 8) | c2

/*
 *  assign_PCC_regs:
 *  Input: c1(child reg 1), c2(child reg 2), p(parent reg), val.
 *      PCC -> Parent, Child, Child.
 *      Parent Registers: ax, bx, cx, dx
 *      Child: ah, al, bh, bl, ch, cl, dh, dl
 *      Assign the parent register, as well as the two children
 *      registers(higher-half/lower-half of parent)
 * */
#define assign_PCC_regs(c1, c2, p, val)   \
    c = val >> 8;                         \
    c2 = val & 0xFF;                      \
    update_parent_reg(p, c1, c2)

/*
 *  test_parent_reg:
 *  Input: p(parent register), c1(child reg 1), c2(child reg 2).
 *      Test the parent regs higher-half value to the higher-half register
 *      as well as test the parents regs lower-half value to the
 *      lower-half register.
 * */     
#define test_parent_reg(p, c1, c2)      \
    (p >> 8) == c1 && (p & 0xFF) == c2 ? true : false                               

#endif