#ifndef assembler
#define assembler
#include "../parser.h"

// Position of output device/function to return the size from assembler.
static UNS32        pos = 0;
UNS32   get_size();

#define ASSIGN_RESP(exp)        assembler_resp.expect = exp;

// Function that allows us to freely write values to the device without restriction.
void asmwrite(_WriteInfo wi);

// Start assembling(invokes lexer/parser functionality).
void begin_assemble(_Parser *p);

// Checks if a register is 'locked'.
void is_locked(enum Tokens reg, _Parser *p);

// Updates the binary with the according bytecode.
_AssemblerResp update_bin(_AssemblerInfo a_info);

// Decipher the type of value we are working with(byte, word or dword).
enum Tokens infer_type(_AssemblerInfo asm_info);

// Close the ouput device. We are done.
void close_device();

#endif