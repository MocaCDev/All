#ifndef bytecode
#define bytecode
#include "../../common.h"
#include "../lexer.h"

/*
 *  ByteCode:
 *      Structure that contains information about the bytecode
 *      value of the current instruction.
 * */
typedef struct ByteCode {
    struct {
        UNS8        bytecode_val;
        enum Tokens token;
    };
} _ByteCode;

/*
 *  InstructionOp:
 *      Values to expect for different instructions. Helps us assemble
 *      quicker
 * */
enum InstructionOp {
    R16   = 2 << 1,
    R8    = 2 << 2,
    VAL8  = 2 << 3,
    VAL16 = 2 << 4
};

/*
 *  IMM8    -> strictly VAL8, only 8bit values.
 *  IMM16   -> VAL8/VAL16 values. Can be either. Or them.
 *  REG8    -> Strictly 8bit registers.
 *  REG16   -> 8bit/16bit registers.
 *  
 *  TODO: Support movb, movs, movd etc?
 *  TODO: Fully understand how this will work.
 * */
#define IMM8        (VAL8)
#define IMM16       (VAL8 | VAL16)
#define REG8        (R8)
#define REG16       (R8 | R16)

/*
 *  InstructionInfo:
 *      Structure containing the information for instructions.
 *      Strictly for the parser. Helps with configuring the syntax/requirements
 *      for a instruction.
 * */
typedef struct InstructionInfo {
    struct {
        enum Tokens             token;
        UNS32                   ops[3];
    };
} _InstructionInfo;

static const _InstructionInfo opcode_info[] = {
    {mov,   {REG16, IMM16}},
    {keep,  {REG16, IMM16}}
};

static const _ByteCode byte_code[] = {
    // 0xA0 - 0xA1 are all keyword bytecodes.
    {0xA0,           mov},
    {0xA1,          keep},
    {0xA2,     interrupt},
    // 0xA3 -> cli
    // 0xA4 -> sti
    {0xA5,          dump},
    // 0xA6 -> all
    {0xB0,           ref},
    {0xB1,         refer},
    {0xB2,      overload},
    // 0xB0 - 0xB2 are all size operand bytecodes.
    {0xB3,          byte},
    {0xB4,          word},
    {0xB5,         dword},
    // 0xC0 - 0xD3 are register opcodes
    {0xC0,        ax_reg},
    {0xC1,        ah_reg},
    {0xC2,        al_reg},
    {0xC3,        bx_reg},
    {0xC4,        bh_reg},
    {0xC5,        bl_reg},
    {0xC6,        cx_reg},
    {0xC7,        ch_reg},
    {0xC8,        cl_reg},
    {0xC9,        dx_reg},
    {0xCA,        dh_reg},
    {0xCB,        dl_reg},
    {0xCC,        ss_reg},
    {0xCD,        sp_reg},
    {0xCE,        bp_reg},
    {0xCF,        ds_reg},
    {0xD0,        es_reg},
    {0xD1,        ip_reg},
    {0xD2,        fs_reg},
    {0xD3,        gs_reg}
    // 0xD4 -> "Special" register to store origin of program.
};

static S16 find_bytecode(enum Tokens id)
{
    for(UNS16 i = 0; i < sizeof(byte_code)/sizeof(byte_code[0]); i++)
        if(byte_code[i].token == id) return i;
    
    return -1;
}

#endif