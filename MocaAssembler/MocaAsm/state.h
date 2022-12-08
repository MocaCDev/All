#ifndef state
#define state

// Default values for each enum.
#define DEFAULT_STATE       0x00

/*
 *  OperandSizes:
 *      Enum containing all the operand sizes(byte, word, dword).
 * */
// enum OperandSizes {
//     NoOp,       // NULL
//     Byte,
//     Word,
//     DWord
// };

/*
 *  Where:
 *      What position are we at in an instruction? LVAL or RVAL 
 * */
enum Where {
    DEF,        // NULL
    LVAL,
    RVAL
};

/*
 *  Special:
 *      Special properties given to an instruction. 
 * */
enum Special {
    NoSpec,     // NULL
    overloaded, // TODO: Structure to store data about overloaded register & value.
    referred,   // TODO: Structure to store data about referred register & value.
    reference
};

/*
 *  Instructions:
 *      All of the possible instruction the assembler can run into. 
 * */
enum Instructions {
    NoInstr,    // NULL
    MOV
};

/*
 *  InstructionState:
 *      The current state of the current instruction.
 *      Helps decipher where we're at and helps decipher
 *      the properties of the instruction
 * */
typedef struct InstructionState {
    // What instruction are we working with?
    enum Instructions       instruction;

    // Where are we in the instruction?
    enum Where              position;

    // Special size operand given by user?
    // BOTH operand sizes HAVE to be the same.
    enum OpTypes       lval_op_size;
    enum OpTypes       rval_op_size;

    // LVAL
    enum Registers          lreg;
    enum Special            lspec;      // referred/overloaded HAVE to be declared on lvalue.
                                        // lspec can be reference, and so can rspec.
    // RVAL
    enum Registers          rreg;
    enum Special            rspec;      // CANNOT be overloaded/referred
} _InstructionState;

// Current state of the instruction
static _InstructionState    ins_state;

// Macros to help us decipher what can and cannot be done depending on position of
// the instruction.
#define LVAL        (overload | reference | referred |      \
                     byte | dword)
#define RVAL        (reference | byte | word | dword)

// Macros to test LVAL/RVAL.
#define test_lval                                              \
    if(test_op(ins_state.lspec, LVAL) == true) true;           \
    else com_error("ERROR1")                                   \

#define test_rval                                              \
    if(test_op(ins_state.rspec, RVAL) == true) true;           \
    else com_error("ERROR2")                                   \

// Test LVAL/RVAL at once.
#define test_instruction        \
    test_lval                   \
    test_rval                   

// Assign the size operand, and perform checks accordingly.
#define assign_sizeop(p, opsize)                                        \
    if(ins_state.position == LVAL)  ins_state.lval_op_size = opsize;    \
    else {                                                              \
        if(ins_state.lval_op_size == DEF)                               \
            ins_state.lval_op_size = opsize;                            \
        ins_state.rval_op_size = opsize;                                \
        if(!(ins_state.lval_op_size == ins_state.rval_op_size))         \
            com_error("Line: %d\n\t\tSize operand mismatch.\n",         \
                p->lexer->current_line                                  \
            )                                                           \
    }

// Fill out instruction state struct to default values
#define reset_state                 \
    ins_state.position = LVAL;      \
    ins_state.lval_op_size = byte;  \
    ins_state.rval_op_size = byte;  \
    ins_state.instruction = NoInstr;\
    ins_state.lreg = NoReg;         \
    ins_state.rreg = NoReg;         \
    ins_state.lspec = NoSpec;       \
    ins_state.rspec = NoSpec;                                     

#endif