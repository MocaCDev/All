#ifndef REG_INFO
#define REG_INFO
#include "../common.h"

/*
 *  Keywords:
 *      Enum storing all the keywords MocaAsm features. 
 * */
enum Keywords {
    mov             = 0x00,
    keep            = 0x01,
    dump            = 0x02,
    times           = 0x03,
    interrupt       = 0x04,
    all             = 0x05,
    ref             = 0x06,
    overload        = 0x07,
    refer           = 0x08,
    org             = 0x09
};

/*
 *  Registers:
 *      Enum containing all the registers available in 16bit assembly.
 *  TODO: Update it to consist of 32bit registers.
 *  TODO: Add a "global" header file that is made up purely of structures over
 *        registers. We don't want to create new enums in every header file
 *        to represent registers.
 * */
enum Registers {
    ax_reg = 0xA, ah_reg, al_reg,
    bx_reg, bh_reg, bl_reg,
    cx_reg, ch_reg, cl_reg,
    dx_reg, dh_reg, dl_reg,
    ss_reg, sp_reg, bp_reg,
    ds_reg, es_reg, ip_reg,
    fs_reg, gs_reg, NoReg   // `NoReg` is just a fill in for other situations.
};

/*
 *  OpTypes:
 *      Variables/register types(byte, word or dword).
 * */
enum OpTypes {
    byte = 0x1F,
    word,
    dword,
    db,
    dd,
    dw,
    NoOp
};

/*
 *  ValueTypes:
 *      Types of values we can expect
 * */
enum ValueTypes {
    hex_value = 0x25,
    num_value,
    byte_value,
    word_value,
    dword_value,
    val
};

// All of the registers. Used for TEST_OP.
#define ALL_REGS                    \
    (ax_reg | ah_reg | al_reg |     \
     bx_reg | bh_reg | bl_reg |     \
     cx_reg | ch_reg | cl_reg |     \
     dx_reg | dh_reg | dl_reg |     \
     ss_reg | sp_reg | bp_reg |     \
     ds_reg | es_reg | ip_reg |     \
     fs_reg | gs_reg )

// All of the parent regs. Used for TEST_OP.
// TODO: Should we include registers: ss, sp, bp, ds, es, ip, fs and gs?
#define PARENT_REGS             \
    (ax_reg | bx_reg |          \
     cx_reg | dx_reg )
    
// All of the child regs. Used for TEST_OP.
#define CHILD_REGS              \
    (ah_reg | al_reg |          \
     bh_reg | bl_reg |          \
     ch_reg | cl_reg |          \
     dh_reg | dl_reg)

/*
 *  Gen16bitRegInfo:
 *      General register information over 16bit registers.
 * */
typedef struct Gen16bitRegInfo {
    struct {
        enum Registers      reg;
        enum OpTypes        type;

        // is_special will be true for registers like ss, sp, bp, es, fs, gs and ip.
        bool                is_special;

        // mov_direct: can the register be used directly in a mov instruction(`mov reg`).
        bool                mov_direct;
    };
} _G16bit_RI;

// Since this is my code, I created a macro to make the variable declaration
// look not-so-ugly.
#define fill(r, t, s, md)     \
    .reg=r, .type=t, .is_special=s, .mov_direct=md

// All register information.
static _G16bit_RI reg_info_16bit[] = {
    {fill(ax_reg, word, false, true)},
    {fill(ah_reg, byte, false, true)},
    {fill(al_reg, byte, false, true)},
    {fill(bx_reg, word, false, true)},
    {fill(bh_reg, byte, false, true)},
    {fill(bl_reg, byte, false, true)},
    {fill(cx_reg, word, false, true)},
    {fill(ch_reg, byte, false, true)},
    {fill(cl_reg, byte, false, true)},
    {fill(dx_reg, word, false, true)},
    {fill(dh_reg, byte, false, true)},
    {fill(dl_reg, byte, false, true)},
    {fill(sp_reg, dword, true, true)},
    {fill(bp_reg, dword, true, true)},
    {fill(es_reg, dword, true,false)},
    {fill(ss_reg, dword, true, true)},
    {fill(ds_reg, dword, true, true)},
    {fill(fs_reg, dword, true, true)}, // Is this the right type?
    {fill(gs_reg, dword, true, true)}, // Same here.
    {fill(ip_reg, dword, true, false)} // And here. Can `ip` be used in a `mov`?
};

#endif