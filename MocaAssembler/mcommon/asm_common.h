#ifndef assembler_common
#define assembler_common
#include "../MocaAsm/parser.h"
#include "../MocaAsm/lexer.h"
#include "../common.h"

/*
 *  TODO:
 *  
 *      Update file to store strictly structures/enums/variables that are
 *      viable for usage during the assembling.
 *      This file provides mainly structures/variables that tell us
 *      a thing or two about registers, what types there are etc
 */

/*
 *  out_dev:
 *      output device of which we write the image/binary to.
 *  mem_dev:
 *      output device for writing memory addresses etc.
 * */
static FILE*    out_dev         = NULL;
static FILE*    mem_dev         = NULL;

/*
 *  Some common data for registers.
 * */
#define PARENT_REG_MAX      (S32) 0x7FFF
#define PARENT_REG_MIN      (S32) 0xFFFFFFFFFFFF8001+1
#define CHILD_REG_MAX       (S32) 0x7F
#define CHILD_REG_MIN       (S32) 0xFFFFFFFFFFFFFF80+1

/*
 *  Macros used to refer to all of the parent registers/child
 *  registers, as well as other things.
 * */

// TODO:
//      Move `asm_common.h` into the 'global' header file of which contains all
//      the given information being held in the file.

/*#define PARENT_REGS     \
    (ax_reg | bx_reg |  \
     cx_reg | dx_reg |  \
     ss_reg | sp_reg |  \
     bp_reg | es_reg |  \
     ip_reg | fs_reg | gs_reg)
#define CHILD_REGS      \
    (ah_reg | al_reg |  \
     bh_reg | bl_reg |  \
     ch_reg | cl_reg |  \
     dh_reg | dl_reg )*/
#define MACROS      \
    (keep | dump)
#define OPCODE_SIZES    \
    (db | dw | dd |     \
     byte | word | dword)

/*
 *  Lock:
 *      Structure holding all the information for the current 'locked'
 *      register.
 *      A 'locked' register is any register assign to a value under
 *      the '@keep' macro.
 * */
typedef struct Lock {
    enum Tokens     reg;
    UNS32           data;
} _Lock;

/*
 *  AssemblerInfo:
 *      Struct containing information about the current bytecode.
 *      The struct is strictly for the assembler. The parser just fills
 *      out the data.
 * */
typedef struct AssemblerInfo {
    UNS8                    _bytecode;
    S32                     data;
    enum Tokens             token_id;
    enum OpTypes            optype;
    UNS32                   cline;
} _AssemblerInfo;

/*
 *  AsmExpect:
 *      Enum containing what the parsers next instruction should be.
 *      Example: `mov`, the parser should now expect a register.
 *  AssemblerResp:
 *      Response from assembler. Based on the data from AssemblerInfo,
 *      the assembler will give some data back to the parser to 'help'
 *      it with the following values it will obtain.
 * */
enum AsmExpect {
    // Expect a register.
    _register = (ax_reg | ah_reg | al_reg |
                 bx_reg | bh_reg | bl_reg |
                 cx_reg | ch_reg | cl_reg |
                 dx_reg | dh_reg | dl_reg |
                 sp_reg | bp_reg | sp_reg |
                 es_reg | ds_reg | ip_reg |
                 fs_reg | gs_reg),
    // Sized operand of the instruction. `byte`, `word or `dword`.
    _size_op = (byte | word | dword),
    // Types(hexadecimal or decimal).
    number = (hex_value | num_value)
};

typedef struct AssemblerResp {
    enum AsmExpect      expect[2];
    // TODO: Possibly have other values to help?
} _AssemblerResp;

// Assembler response information
static _AssemblerResp   assembler_resp;

// Assembler information
static _AssemblerInfo assembler_info;
static _AssemblerInfo prev_assembler_info;

/*
 *  Functions used during execution of bytecode.
 * */
static UNS16 LE_16(UNS16 val)
{
    UNS16 ret = 0;
    
    ret |= (val >> 8) & 0x00FF;
    ret |= (val << 8) & 0xFF00;
    
    return ret;
}

static UNS8 LE_get_byte_val(UNS16 val, bool endianess)
{
    // If endianess = true, swap the 16bit value. Otherwise, don't.
    if(endianess == true) {
        val = LE_16(val);
        return (UNS8) (val << 8) & 0x00FF;
    }

    return (UNS8) (val << 8) & 0x00FF;
}

static UNS32 LE_32(UNS32 val) {
    UNS32 ret = 0;
    
    ret |= (val >> 24)    & 0x000000FF;
    ret |= (val >> 8)     & 0x0000FF00;
    ret |= (val << 8)     & 0x00FF0000;
    ret |= (val << 24)    & 0xFF000000;

    return ret;
}

static void write_8(UNS8 data, UNS32 amnt)
{
    for(; amnt > 0; amnt--)
        fwrite(&data, 1, sizeof(UNS8), out_dev);
}

static void write_16(UNS16 data, UNS32 amnt)
{
    UNS8 d[2];

    UNS16 *loc = (UNS16 *) &d[0];
    *loc = LE_16(data);

    for(; amnt > 0; amnt--)
        fwrite(d, 2, sizeof(UNS8), out_dev);
}

static void write_32(UNS32 data, UNS32 amnt)
{
    UNS8 d[4];

    UNS32 *loc = (UNS32 *) &d[0];
    *loc = LE_32(data);

    for(; amnt > 0; amnt--)
        fwrite(d, 4, sizeof(UNS8), out_dev);
}

static UNS32 convert_hex(UNS8 *val)
{
    UNS32 number = 0;

    while(*val) {
        UNS8 b = *val++;

        if(b >= '0' && b <= '9') b = b - '0';
        else if (b >= 'a' && b <='f') b = b - 'a' + 10;
        else if (b >= 'A' && b <='F') b = b - 'A' + 10;
        number = (number << 4) | (b & 0xF);
    }

    return number;
}

#endif