#include "parser.h"

void next_token(_Parser *p)
{
    p->token = get_next_token(p->lexer);
}

static void parse_16(_Parser *p)
{
    next_token(p);

    switch(p->token.id) {
        case mov: p->func_call = parse_mov;break;
        case interrupt: p->func_call = parse_int;break;
        case org: p->func_call = parse_org;break;
        case times: p->func_call = parse_times;break;
        case db: p->func_call = parse_db;break;
        case dw: p->func_call = parse_dw;break;
        case val: p->func_call = parse_val;break;
        case hex_value:
        case num_value: com_error("Unexpected hexadecimal/decimal value on line %d.\n\tValue: %s\n", p->lexer->current_line, p->token.value)
        case AT_SYMB: p->func_call = parse_macro;break;
        case EOf: {
            free(p->lexer);
            free(p);
            close_device();
            exit(0);
        }
    }
}

static void parse_32(_Parser *p)
{

    // TODO: Do this
    printf("Assembling 32bit");
    exit(0);
}

_Parser *init_parser(_Lexer *lex, bool is16, UNS8 *output_file)
{
    _Parser *p = calloc(1, sizeof(*p));
    p->output_file = output_file;

    // Set the mode.
    if(is16) p->mode = BIT16;
    else p->mode = BIT32;

    // Set the lexer.
    p->lexer = lex;

    // Depending on the mode, do accordingly.
    switch(p->mode) {
        case BIT16: p->get_next = parse_16;goto end;
        case BIT32: p->get_next = parse_32;goto end;
    }

end:
    return p;
}