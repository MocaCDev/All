#include "assembler.h"

void begin_assemble(_Parser *p)
{
    out_dev = fopen(p->output_file, "wb");
    //bin_data = calloc(1, sizeof(*bin_data));

    p->get_next(p);

    while(p->token.id != EOf) {
        p = (_Parser *) p->func_call(p);
        p->get_next(p);
    }
}

// This is not usefull to get the actual type the value needs to be.
// Example, if we have `mov ax, 0x10`, it will say the value is a byte, when in fact,
// we write a 16bit integer to the device.
enum Tokens infer_type(_AssemblerInfo asm_info)
{
    if(asm_info.data >= CHILD_REG_MIN && asm_info.data <= CHILD_REG_MAX) return byte;
    if(asm_info.data >= PARENT_REG_MIN && asm_info.data <= PARENT_REG_MAX) return word;
    return dword;
}

UNS32 get_size() { return pos; }

void asmwrite(_WriteInfo wi)
{
    switch(wi.pad_type) {
        case byte: write_8((UNS8) wi.pad_bytecode, wi.pad_amnt);pos++;break;
        case word: write_16((UNS16) wi.pad_bytecode, wi.pad_amnt);pos+=2;break;
        case dword: write_32((UNS32) wi.pad_bytecode, wi.pad_amnt);pos+=4;break;
        default: break;
    }
}

void is_locked(enum Tokens reg, _Parser *p)
{
    for(UNS32 i = 0; i < p->lock_amnt; i++)
        com_assert(
            ( p->lock_state[i]->reg != reg ),
            "Line: %d\n\t\tAttempting to lock an already locked register, `%s`.\n\t\tIf you want to reassign or relock the register with a different value,\n\t\tdump it first using `@dump register`, or `@dump all`.\n", p->lexer->current_line, get_TV(reg)
        )
}

_AssemblerResp update_bin(_AssemblerInfo a_info)
{
    // Get bytecode information
    com_assert(!(find_bytecode(a_info.token_id) == -1), "Token `%d` does not have a bytecode.\n", a_info.token_id)
    a_info._bytecode = byte_code[find_bytecode(a_info.token_id)].bytecode_val;

    write_8(a_info._bytecode, 1);
    pos++;

    // Get assembler response
    switch(a_info.token_id) {
        case mov: {
            assembler_resp = (_AssemblerResp){ 
                .expect = {_register, _size_op} 
            };
            break;
        }
        case keep:
        case dump: {
            assembler_resp = (_AssemblerResp){
                .expect = {_register}
            };
            break;
        }
        case interrupt: {
            assembler_resp = (_AssemblerResp){
                .expect = {number}
            };
            break;
        }
        case byte:
        case word:
        case dword: {
            assembler_resp = (_AssemblerResp){
                .expect = {_register, _size_op}
            };
            break;
        }
        case ax_reg:
        case ah_reg:
        case al_reg:
        case bx_reg:
        case bh_reg:
        case bl_reg:
        case cx_reg:
        case ch_reg:
        case cl_reg:
        case dx_reg:
        case dh_reg:
        case dl_reg: {
            assembler_resp = (_AssemblerResp){
                .expect = {_size_op, _register}
            };
            break;
        }
        case es_reg: {
            assembler_resp = (_AssemblerResp){
                .expect = {_register}
            };
            break;
        }
        default: com_error("Line: %d\n\t\tUnexpected token received by assembler, `%s`.\n", a_info.cline, get_TV(a_info.token_id))
    }

    prev_assembler_info = a_info;

    return assembler_resp;
}

_AssemblerResp write_reg_value(_AssemblerInfo a_info)
{
    test_instruction
    
    com_assert(
        ( test_op(a_info.optype, assembler_resp.expect[0]) == true ),
        "Line: %d\n\t\tSize operands do not match.\n", a_info.cline
    )

    switch(a_info.optype) {
        case byte: write_8((UNS8) a_info.data, 1);pos++;break;
        case word: write_16((UNS16) a_info.data, 1);pos+=2;break;
        case dword: write_32((UNS32) a_info.data, 1);pos+=4;break;
        default: break;
    }

    return (_AssemblerResp){};
}

void close_device()
{
    fclose(out_dev);
}