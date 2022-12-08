#ifndef lexer16
#define lexer16
#include "global.h"
#include "../common.h"

#define is_ascii(v)     \
    (v >= 'a' && v <= 'z') || (v >= 'A' && v <= 'Z') ? true : false

/*
 *  Tokens:
 *      Enum storing all the token constants
 * */
enum Tokens {
    // Characters
    L_SQRB = 0x2B,
    R_SQRB,
    L_CRLB,
    R_CRLB,
    COMMA,
    COLON,
    MINUS,
    DSIGN,
    AT_SYMB,
    DOLLAR_SYMB,
    // EOF
    EOf
};

/*
 *  _TokenInfo stores all information about the possible tokens.
 *
 *  It will store the token value as well as the token ID. This will be
 *  useful for returning token information for the parser.
 * */
typedef struct TokenInfo {
    struct {
        UNS8        *token_value;
        UNS32        token_id;
    };
} _TokenInfo;

/*
 *  _Token is a structure returned by the lexer to the parser.
 *
 *  The structure contains information over the current token the lexer
 *  picked up.
 * */
typedef struct Token {
    UNS8            *value;
    enum Tokens      id;
} _Token;

/*
 *  _Lexer is a structure that contains the current state of the lexer.
 *
 *  This will be useful for not only being able to lex the file, but
 *  having a structure that contains the current state of lexing
 *  for the parser.
 * */
typedef struct Lexer {
    UNS8        value;
    UNS32       current_line;
    UNS32       index;
    UNS8        *data;
} _Lexer;

// All token value/token id information.
static const _TokenInfo token_info[] = {
    // Keywords
    {(UNS8 *)"mov",            mov},
    {(UNS8 *)"org",            org},
    {(UNS8 *)"times",        times},
    {(UNS8 *)"int",      interrupt},
    {(UNS8 *)"all",            all},
    {(UNS8 *)"ref",            ref},
    {(UNS8 *)"refer",        refer},
    {(UNS8 *)"overload",  overload},
    {(UNS8 *)"db",              db},
    {(UNS8 *)"dd",              dd},
    {(UNS8 *)"dw",              dw},
    {(UNS8 *)"word",          word},
    {(UNS8 *)"byte",          byte},
    {(UNS8 *)"dword",        dword},
    // Registers
    {(UNS8 *)"ax",          ax_reg},
    {(UNS8 *)"ah",          ah_reg},
    {(UNS8 *)"al",          al_reg},
    {(UNS8 *)"bx",          bx_reg},
    {(UNS8 *)"bh",          bh_reg},
    {(UNS8 *)"bl",          bl_reg},
    {(UNS8 *)"cx",          cx_reg},
    {(UNS8 *)"ch",          ch_reg},
    {(UNS8 *)"cl",          cl_reg},
    {(UNS8 *)"dx",          dx_reg},
    {(UNS8 *)"dh",          dh_reg},
    {(UNS8 *)"dl",          dl_reg},
    {(UNS8 *)"es",          es_reg},
    {(UNS8 *)"ss",          ss_reg},
    {(UNS8 *)"sp",          sp_reg},
    {(UNS8 *)"bp",          bp_reg},
    {(UNS8 *)"ip",          ip_reg},
    {(UNS8 *)"ds",          ds_reg},
    {(UNS8 *)"fs",          fs_reg},
    {(UNS8 *)"gs",          gs_reg},
    // Macros
    {(UNS8 *)"keep",        keep},
    {(UNS8 *)"dump",        dump}
};

// Initializes the lexer(assigns file data, current value etc).
_Lexer *init_lexer(UNS8 *data);

// Gets the next token from the source code.
_Token get_next_token(_Lexer *lexer);

// Return the value of a token.
S8 *get_TV(enum Tokens id);

#endif