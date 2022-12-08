#ifndef PARSER
#define PARSER
#include "../common.h"
#include "../mcommon/asm_common.h"
#include "assembler/bytecode.h"
#include "lexer.h"
#include "state.h"

/*
 *  The parser itself is not used, rather, the functionality of
 *  the parser will be invoked in the assembler. The parser is
 *  just a template of all the code we need for specific instructions.
 * */

// Mode of assembler(16bit, 32bit)
enum AssemblerMode {
    BIT16,
    BIT32
};

typedef struct Parser {
    _Token              token;
    _Lexer              *lexer;
    struct Parser*      (*func_call)(struct Parser *parser);
    struct Parser       *next;
    UNS8                *output_file;

    // Mode of assembler. This helps us decipher whether we are parsing
    // 32bit assembly or 16bit assembly
    enum AssemblerMode  mode;

    // 'locked' state of registers
    _Lock               **lock_state;
    UNS32               lock_amnt;

    // Function to call from assembler to get next token and parse it.
    void               (*get_next)(struct Parser *p);
} _Parser;

/*
 *  Some macros to help us throughout the parser.
 *  NOT_END checks if the current token is EOf. If it is, it errors. 
 */
#define NOT_END(p)                                  \
    COM_ASSERT(                                     \
        (p->token.id != EOf),                       \
        "Line: %d\n\t\tUnexpected end of file.\n", p->lexer->current_line)
#define LEXER(p)        (p->lexer)

// Initialize the parser. Set the assembler mode. And call according function
_Parser *init_parser(_Lexer *lex, bool is16, UNS8 *output_file);
void next_token(_Parser *p);

// All 16bit parser functions
_Parser *parse_mov(_Parser *p);
_Parser *parse_int(_Parser *p);
_Parser *parse_org(_Parser *p);
_Parser *parse_times(_Parser *p);
_Parser *parse_db(_Parser *p);
_Parser *parse_dw(_Parser *p);
_Parser *parse_dd(_Parser *p);  // TODO: Add this.
_Parser *parse_macro(_Parser *p);
_Parser *parse_val(_Parser *p);

/*
 *  External functions used in the parser/extra structures needed
 *  throughout parsing.
 */
extern void is_locked(enum Tokens reg, _Parser *p);
extern void asmwrite();
extern _AssemblerResp update_bin(_AssemblerInfo a_info);
extern _AssemblerResp write_reg_value(_AssemblerInfo a_info);
extern enum Tokens infer_type(_AssemblerInfo asm_info);
extern UNS32 get_size();
extern void close_device();

// Macro specifically used for update_bin
#define test_op(op, against)    (op & against) == op ? true : false

#define updbin(tid, l)                 \
    assembler_info.token_id = tid;     \
    assembler_info.cline     = l;       \
    assembler_resp = update_bin(assembler_info);

#define assign_val(tid, val, l, t)         \
    assembler_info.token_id = tid;         \
    assembler_info.data = val;             \
    assembler_info.cline = l;              \
    assembler_info.optype = t;             \
    assembler_resp = write_reg_value(assembler_info);

/*
 *  test_all: Function to test all the expectations of the next operand.
 *  GET_AND_SET: Update the binary, get next token.
 *  GET_AND_TEST: Update the binary, test the next token according to what
 *                the assembler says we should be getting next.
 * */
static bool test_all(UNS32 tid, enum AsmExpect exp[2])
{
    bool looks_good = false;
    for(UNS32 i = 0; i < 2; i++) {
        if(test_op(tid, exp[i]) == true) looks_good = true;

        // If the token id matches to at least one of the two expected operands
        // then break and return.
        if(looks_good) break;
    }

    return looks_good;
}

#define get_and_set(p)                                              \
    updbin(((_Parser *)p)->token.id, p->lexer->current_line);       \
    next_token(((_Parser *)p));
    //NOT_END(p)

#define get_and_test(p)                                             \
    updbin(((_Parser *)p)->token.id, p->lexer->current_line);       \
    next_token(((_Parser *)p));  \
    if(p->token.id == COMMA) next_token((_Parser *)p);\
    com_assert(                                                     \
        ( test_all(p->token.id, assembler_resp.expect) == true ),   \
        "Line: %d\n\t\tValue `%s` was not expected by the assembler", p->lexer->current_line, p->token.value                                                   \
    );
    //NOT_END(p)

// Extra structures.
typedef struct WriteInfo {
    // TODO: Update this
    // TODO: Create some sort of global file that has information
    //       about opcode sizes/register information.
    enum OpTypes    pad_type;

    // Bytecode value to fill the padding with.
    UNS32           pad_bytecode;

    // Amount of bytes to pad.
    S32             pad_amnt;
} _WriteInfo;

static _WriteInfo wi;

/*
 *  write_freely:
 *      This macro fills out the _WriteInfo structure data, then
 *      calls asmwrite function that will then use this data to
 *      write to the device.
 * */
#define write_freely(pt, bc)     \
    wi.pad_amnt = 1;                \
    wi.pad_type = pt;               \
    wi.pad_bytecode = bc;  \
    asmwrite(wi);

#endif