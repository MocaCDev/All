#include "parser.h"

static UNS32 convert(_Parser *p)
{
    if(p->token.id == hex_value) return convert_hex(p->token.value);

    return atoi(p->token.value);
}

static bool is_reg(_Parser *p)
{
    switch(p->token.id) {
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
        case dl_reg: return true;
        default: return false;
    }
}

static enum OpTypes get_register_optype(enum Registers _register)
{
    UNS32 i = 0;
    for(; i < sizeof(reg_info_16bit)/sizeof(reg_info_16bit[0]); i++)
        if(reg_info_16bit[i].reg == _register) break;

    return reg_info_16bit[i].type;
}

static void check_reg_info(_InstructionState istate, _Parser *p)
{
    //next_token(p);

    // Check register against value.
    if(istate.rreg == NoReg) {
        // Get register information.
        UNS32 index = 0;
        for(; index < sizeof(reg_info_16bit)/sizeof(reg_info_16bit[0]); index++) {
            if(reg_info_16bit[index].reg == istate.lreg) break;
        }

        assembler_info.data = convert(p);

        if(infer_type(assembler_info) == word && reg_info_16bit[index].type == byte ||
           infer_type(assembler_info) == dword && reg_info_16bit[index].type == word)
            com_error("Line: %d\n\t\tRegister size overflow.\n", p->lexer->current_line);

        return;
    }

    // Check register against register.
    // Get information about both corresponding registers.
    for(UNS8 i = 0; i < 2; i++) {

    }

    exit(0);
}

// Same as "peek" in lexer, just allows the parsers instance of the lexer
// to check the next value.
static bool ppeek(_Parser *p, UNS8 val)
{
    p->lexer->index++;
    p->lexer->value = p->lexer->data[p->lexer->index];

    if(p->lexer->value == val) {
        p->lexer->index--;
        p->lexer->value = p->lexer->data[p->lexer->index];
        
        return true;
    }

    // If it's not true, reset lexers state and continue on.
    p->lexer->index--;
    p->lexer->value = p->lexer->data[p->lexer->index];

    return false;
}

// Strictly for `mov` instructions. `parser_move`.
static void assembler_update_bin(bool isreg, bool isval, _Parser *p)
{
    if(isreg) {
        get_and_test(p)
        
        return;
    }

    if(isval) {
        UNS32 val = convert(p);

        assign_val(p->token.id, val, p->lexer->current_line, get_register_optype(ins_state.lreg))
        return;
    }

    get_and_test(p)
}

_Parser *parse_mov(_Parser *p)
{
    reset_state

    get_and_test(p)

redo:
    switch(p->token.id) {
        case hex_value:
        case num_value: check_reg_info(ins_state, p);assembler_update_bin(false, true, p);goto end;
        case byte: {
            assign_sizeop(p, byte)
        
            get_and_test(p)

            goto redo;
        }
        case word: {
            assign_sizeop(p, word);

            // TODO: What are we doing here?
            get_and_test(p)

            goto redo;
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
        case dl_reg:
        case es_reg: {
            if(ins_state.position == LVAL) {
                // If there is a lock state, check and make sure none of the registers
                // are locked.
                if(p->lock_state)
                    for(UNS32 i = 0; i < p->lock_amnt; i++)
                        if(p->lock_state[i]->reg == p->token.id)
                            com_error("Line: %d\n\t\tAttempting to perform `mov` intruction on locked register `%s`.\n",
                                p->lexer->current_line, p->token.value
                            )
                
                ins_state.lreg = p->token.id;
                assembler_update_bin(true, false, p);
                ins_state.position = RVAL;

                goto redo;
            }
            if(ins_state.position == RVAL) {
                ins_state.rreg = p->token.id;
                check_reg_info(ins_state, p);

                write_freely(byte, byte_code[find_bytecode(p->token.id)].bytecode_val)
                
                // Reset the state, and return.
                reset_state
                goto end;
            }
        }
    }
    
end:
    return p;
}

void parse_keep_macro(_Parser *p)
{
    if(p->lock_state == NULL) {
        p->lock_state = calloc(1, sizeof(*p->lock_state));
        p->lock_amnt = 0;
        p->lock_state[p->lock_amnt] = calloc(1, sizeof(*p->lock_state[p->lock_amnt]));
    }

    get_and_test(p)

    switch(p->token.id) {
        case mov: parse_mov(p);break;
        default: {
            //if(p->token.id == EOf) com_error("Unexpected End Of File")

            // If it's not a register, error
            com_assert(
                ( test_op(p->token.id, PARENT_REGS) == true ) ||
                ( test_op(p->token.id, CHILD_REGS) == true ),
                "Macro `keep` expects a register"
            )

            // TODO: Get the `is_locked` function to work
            // TODO: Get the `n_lock_state`/`lock_state` variables working
            // Check if the current register is 'locked'
            is_locked(p->token.id, p);

            p->lock_state[p->lock_amnt]->reg = p->token.id;
            assembler_update_bin(true, false, p);

            p->lock_state[p->lock_amnt]->data = convert(p);
            assembler_update_bin(false, true, p);

            p->lock_amnt++;

            p->lock_state = realloc(p->lock_state, (p->lock_amnt + 1) * sizeof(*p->lock_state));
            p->lock_state[p->lock_amnt] = calloc(1, sizeof(*p->lock_state[p->lock_amnt]));

            break;
        }
    }
}

// "Helper" function to find index of 'locked' register
// TODO: Move this into a file of its own for functions over locked registers?
static S32 index_of_locked_reg(_Parser *p)
{
    // If function returns -1, the register is not locked.
    S32 index = -1;

    for(UNS32 i = 0; i < p->lock_amnt; i++)
    {
        if(p->lock_state[i]->reg == p->token.id) index = i;
    }
    
    return index;
}

// "Helper" function to attempt to release locked register, if found.
static void attempt_release(_Parser *p)
{
    S32 index = index_of_locked_reg(p);

    com_assert(index >= 0, "Line: %d\n\t\tRegister was not locked by assembler.\n", p->lexer->current_line)

    // Free the memory at the index, if it's valid memory.
    free(p->lock_state[index]);

    // TODO: We don't want to error. Manually write a mov instruction?
}

// TODO: Figure out why tf this shit won't work.
void parse_dump_macro(_Parser *p)
{
    // If there is no lock state, error.
    com_assert(
        ( p->lock_state != NULL ),
        "Line: %d\n\t\t`@dump` macro expects a register to be locked via `@keep` macro.\n",
        p->lexer->current_line
    )

    get_and_test(p)

    /*
     *  TODO: Create a macro of which holds information/"arguments" that are accepted
     *        by the given macro.
     * */

    // Make sure we have a register or we have `all`.
    com_assert(
        ( is_reg(p) == true ) ||
        ( p->token.id == all ),
        "Line: %d\n\t\t`@dump` macro expects a `reg` or `all`.\n",
        p->lexer->current_line
    )

    if(is_reg(p) == true) {
        // Freely write the register being released to the device.
        write_freely(byte, byte_code[find_bytecode(p->token.id)].bytecode_val)
        attempt_release(p);

        if(p->lexer->value == ',') {
            next_token(p);

            while(p->token.id == COMMA) {
                next_token(p);
                
                // Make sure it's a register. Error if it's not.
                com_assert(
                    ( is_reg(p) == true ),
                    "Line: %d\n\t\tExpected `reg` as argument for macro `@dump`.\n",
                    p->lexer->current_line
                )
                
                // Do as we did above: Write register to device, release register.
                write_freely(byte, byte_code[find_bytecode(p->token.id)].bytecode_val)
                attempt_release(p);
            }
        }

        return;
    }

    // Freely write the bytecode for `all`, then release all registers.
    write_freely(byte, 0xA6)
    for(; p->lock_amnt > 0; p->lock_amnt--)
        free(p->lock_state[p->lock_amnt]);
    
    free(p->lock_state);
}

_Parser *parse_macro(_Parser *p)
{
    UNS32 last_line = p->lexer->current_line;
    next_token(p);

    com_assert(
        ( test_op(p->token.id, MACROS) == true ),
        "Unknown macro %s", p->token.value
    )

    switch(p->token.id) {
        case keep: parse_keep_macro(p);break;
        case dump: parse_dump_macro(p);break;
        default: {
            //next_token(p);

            com_assert(
                ( p->lexer->current_line == last_line ),
                "Line: %d\n\t\t@ expects a value after it.\n",
                last_line
            )
            
            switch(p->token.id) {
                case mov: {
                    write_freely(byte, 0xA3)

                    // TODO:
                    //     Add a global state that holds information about registers
                    //     such as a register requiring interrupts to be disabled.
                    //     The struct will just tell the assembler not to write the mov bytecode
                    //     because we just freely wrote a manual bytecode to represent a
                    //     special mov instruction requiring interrupts to be disabled
                }
            }
            exit(0);
        }
    }
    
    return p;
}

_Parser *parse_times(_Parser *p)
{
    next_token(p);

    if(!(p->token.id == hex_value) && !(p->token.id == num_value))
        com_error("Line: %d\n\t\tExpected a hex/decimal value for amount of times to pad.\n", p->lexer->current_line)

    wi.pad_amnt = convert(p);
    next_token(p);

    if(p->token.id == MINUS) {
        next_token(p);

        switch(p->token.id) {
            case DSIGN: wi.pad_amnt -= get_size();break;
            case hex_value:
            case num_value: wi.pad_amnt -= convert(p);break;
        }
        next_token(p);

        com_assert(
            ( wi.pad_amnt > 0 ),
            "Line: %d\n\t\tNegative pad value. Must be > 0.\n", p->lexer->current_line
        )
    }

    com_assert(
        ( test_op(p->token.id, OPCODE_SIZES) == true ),
        "Line: %d\n\t\tExpected size operand(`db`, `dw` or `dd`).\n\t\tInstead got `%s`.\n", p->lexer->current_line, p->token.value
    )

    /*
     *  TODO: 
     *      Make it to where the functions `write_8`, `write_16` and
     *      `write_32` accept a argument for amount of times to
     *      write the byte to the output device.
     *      This way, we don't have to have separate functions
     *      that just call the functions `write_8`, `write_16` and `write_32`.
     */
    switch(p->token.id) {
        case db: wi.pad_type = byte;break;
        case dw: wi.pad_type = word;break;
        case dd: wi.pad_type = dword;break;
    }
    next_token(p);

    wi.pad_bytecode = convert(p);
    asmwrite(wi);
    
    return p;
}

_Parser *parse_db(_Parser *p)
{
    next_token(p);

    write_freely(byte, convert(p))

    return p;
}

_Parser *parse_dw(_Parser *p)
{
    next_token(p);

    // TODO: Update assembler to allow us to write whatever
    //       bytecode we want without so much restriction/precision.
    write_freely(word, LE_16(convert(p)))

    return p;
}

_Parser *parse_org(_Parser *p)
{
    // MocaAssembler requires the org instruction be on the first line.
    com_assert(
        ( get_size() == 0 ) && ( p->lexer->current_line == 1 ),
        "Line: %d\n\t\tCannot set origin of program after first line.\n",
        p->lexer->current_line
    )
    next_token(p);

    // Configure the type
    assembler_info.data = convert(p);
    enum Tokens t = infer_type(assembler_info);

    // TODO: Do we want this?
    // 0xD4 lets the disassembler/emulator know that the following value
    // is to be the origin of the program.
    write_freely(byte, 0xD4)

    // TODO: Make it to where we don't have to do this switch statement every time
    //       we want to decipher the type to write to the device.
    switch(t) {
        case byte: write_freely(byte, assembler_info.data);break;
        case word: write_freely(word, assembler_info.data);break;
        case dword: write_freely(dword, assembler_info.data);break;
    }

    return p;
}

_Parser *parse_val(_Parser *p)
{
    /* 
     *  TODO: Make it where the assembler assigns a memory address to each
     *        user defined variable.
     *        So, if we have `name db 'Bob'`, `name` will be stored in a
     *        memory address given by the assembler.
     * */

    // Freely write the name of the variable to the device.
    for(UNS32 i = 0; i < strlen(p->token.value); i++) {
        write_freely(byte, p->token.value[i])
    }

    // "Pad". Helps the disassembler decipher when to expect the variable type etc.
    write_freely(byte, 0x0)

    next_token(p);

    switch(p->token.id) {
        case db: printf("DB");break;
        case COLON: printf("Function!");break;
    }

    close_device();
    exit(0);
}

_Parser *parse_int(_Parser *p)
{
    get_and_test(p)

    if(!(p->token.id == hex_value) && !(p->token.id == num_value))
        com_error("Line: %d\n\t\tExpected hex/decimal value.\n", p->lexer->current_line)
    
    write_freely(byte, convert(p))

    return p;
}
