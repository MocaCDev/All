#ifndef BIOS_INTERRUPTS
#define BIOS_INTERRUPTS
#include "../../common.h"

#define MAX_REGS_PER_INTERRUPT     8 

/*
 *  enum interrupts -> All interrupts BIOS has.
 * */
enum interrupts {
    NONE         = -1,
    SET_VID_MODE = 0x10
};

/*
 *  enum regs -> All regs the BIOS can use for each interrupt.
 * */
enum regs {
    ax = 1 << 0,
    ah = 1 << 1,
    al = 1 << 2,
    bx = 1 << 3,
    bh = 1 << 4,
    bl = 1 << 5,
    cx = 1 << 6,
    ch = 1 << 7,
    cl = 1 << 8,
    dx = 1 << 9,
    dh = 1 << 10,
    dl = 1 << 11,
    es = 1 << 12,
    ds = 1 << 13,
    fs = 1 << 14,
    gs = 1 << 15,
    ip = 1 << 16,
    ss = 1 << 17,
    sp = 1 << 18,
    bp = 1 << 19
};

/*
 *  function to depict whether a register is 8bit or 16bit
 * */
static UNS8 TYPEOF(UNS16 reg) {
    switch(reg) {
        case ah:
        case al:
        case bh:
        case bl:
        case dh:
        case dl:
        case ch:
        case cl:
            return 8;
        case ax:
        case bx:
        case cx:
        case dx:
            return 16;
        default: com_error("Register didn't match to 8bit or 16bit.")
    }
}

/*
 *  struct BiosInterruptInfo -> 
 *      Information of what registers are needed for each interrupt.
 * */
typedef struct BiosInterruptInfo {
    struct {
        UNS16       interrupt;
        enum regs   regs_needed[3];
    };
} _BiosInterruptInfo;

/*
 *  Register data.
 * */
typedef struct RegData {
    // higher-half/lower-half 16bit register data.
    struct {
        enum regs   register_8bit;
        UNS8        data8bit;
    };
    // 16bit register data
    struct {
        enum regs   register_16bit;
        UNS16       data16bit;
    };
} _RegData;

/*
 *  struct BiosInterrupt -> Information over current interrupt
 *
 *  In the assembly file, it will look like this(for example, setting video mode/font):
 *      @interrupt 0x10
 *      mov ah, 0x00
 *      mov al, 0x00
 *      mov bx, 0x010F
 *      int 0x10
 * */
typedef struct BiosInterrupt {
    enum interrupts         interrupt_id;
    enum regs               regs_used[MAX_REGS_PER_INTERRUPT];
    _RegData                register_data[MAX_REGS_PER_INTERRUPT];
    struct BiosInterrupt    *next;
} _BiosInterrupt;

/*
 *  General information of registers for BIOS.
 *  Probably needed for the emulator to know about values in
 *  each register, when the regs aren't being used for an
 *  interrupt.
 * */
typedef struct GeneralRegInfo {
    enum regs               reg_used;
    _RegData                register_data;
    struct GeneralRegInfo   *next;
} _GeneralRegInfo;

// This will be an "array" of all interrupts
static _BiosInterrupt *interrupts;
static _BiosInterrupt *current_interrupt;
static UNS32           total_interrupts = 0;

// This will be an "array" of all the register data(non-interrupts)
static _GeneralRegInfo *reg_info;
static _GeneralRegInfo *current_reg_info;
static UNS32            total_reg_info = 0;

/*
 *  Macros to assign `next`.
 * */
#define ASSIGN_NEXT_INTR(intr)                                      \
    interrupts->next = calloc(1, sizeof(*interrupts->next));        \
    current_interrupt = interrupts->next;
#define ASSIGN_NEXT_REGI(regi)                                      \
    reg_info->next = calloc(1, sizeof(*reg_info->next));            \
    current_reg_info = reg_info->next;

// All BIOS interrupt information.
// Which includes the interrupt id as well as the registers
// needed for the interrupt.
static const _BiosInterruptInfo interrupt_info[] = {
    {
        (UNS16)SET_VID_MODE,
        .regs_needed = {ah, al, bx}
    }
};

#endif