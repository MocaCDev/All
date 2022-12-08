#include "lexer.h"

_Lexer *init_lexer(UNS8 *data)
{
    _Lexer *lex = calloc(1, sizeof(*lex));

    lex->index = 0;
    lex->data = data;
    lex->value = lex->data[lex->index];
    lex->current_line = 1;
    
    return lex;
}

_Token register_token(UNS8 *val, enum Tokens tid) 
{
    return (_Token) {
        .id = tid,
        .value = val
    };
}

void advance_lexer(_Lexer *lex)
{
    if(!(lex->index > strlen(lex->data))) {
        lex->index++;
        lex->value = lex->data[lex->index];
    }
}

UNS8 *get_word(_Lexer *lex)
{
    UNS8 *word = calloc(1, sizeof(*word));
    UNS16 len = 0;

    while(is_ascii(lex->value) == true) {
        word[len] = lex->value;
        len++;
        word = realloc(word, (len + 1) * sizeof(*word));
        advance_lexer(lex);
    }

    return word;
}

UNS8 *get_number(_Lexer *lex, bool ishex)
{
    UNS8 *number = calloc(1, sizeof(*number));
    UNS16 len = 0;

    while(
        (lex->value >= '0' && lex->value <= '9') ||
        (lex->value >= 'A' && lex->value <= 'F')) {
        number[len] = lex->value;

        // Reallocate, advance.
        len++;
        number = realloc(number, (len + 1) * sizeof(*number));
        advance_lexer(lex);
        if(lex->value == 'h') {
            number[len] = 'h';
            goto end;
        }
    }

end:
    //advance_lexer(lex);

    return number;
}

bool peek(_Lexer *lex, UNS8 value)
{
    advance_lexer(lex);

    // True? Keep lexer in the current state and return true.
    if(lex->value == value) return true;

    // False? Backtrack the lexers state and return false.
    lex->index--;
    lex->value = lex->data[lex->index];
    return false;
}

S8 *get_TV(enum Tokens id)
{
    for(UNS16 i = 0; i < sizeof(token_info)/sizeof(token_info[0]); i++) {
        if(token_info[i].token_id == id) return token_info[i].token_value;
    }

    com_error("Token does not have a value")
}

_Token get_next_token(_Lexer *lex)
{
redo:
    if(lex->index >= strlen(lex->data))
        return register_token("\0", EOf);

    if(lex->value == '\n') {
        while(lex->value == '\n')
        {
            lex->current_line++;
            advance_lexer(lex);
        }

        if(is_ascii(lex->value))
            goto get_word;
        
        goto get_symbol;
    }

    if(lex->value == ' ' || lex->value == '\t')
        while(lex->value == ' ' || lex->value == '\t')
            advance_lexer(lex);

get_word:
    if(is_ascii(lex->value) == true) {
        UNS8 *w = get_word(lex);
        
        for(UNS16 i = 0; i < sizeof(token_info)/sizeof(token_info[0]); i++) {
            if(strcmp(w, token_info[i].token_value) == 0)
                return register_token(token_info[i].token_value, token_info[i].token_id);
        }

        return register_token(w, val);
    }

    if(lex->value >= '0' && lex->value <= '9') {
        UNS8 *value;
        UNS8 index = 0;

        if(peek(lex, 'x') == true)
        {
            advance_lexer(lex);
            //printf("%c", lex->value);
            //exit(0);

            value = get_number(lex, true);
            
            return register_token(value, hex_value);
        }

        value = get_number(lex, false);

        if(value[strlen(value) - 1] == 'h')
        {
            // Get rid of the 'h', return value as hex_value token
            value[strlen(value) - 1] = '\0';
            return register_token(value, hex_value);
        }

        return register_token(value, num_value);
    }

get_symbol:
    switch(lex->value) {
        case ';': {
            while(lex->value != '\n' && lex->value != '\0')
                advance_lexer(lex);
            goto redo;
        }
        case '@': advance_lexer(lex);return register_token("@", AT_SYMB);break;
        case '-': advance_lexer(lex);return register_token("-", MINUS);break;
        case '$': advance_lexer(lex);return register_token("$", DSIGN);break;
        case ',': advance_lexer(lex);return register_token(",", COMMA);break;
        case '[': advance_lexer(lex);return register_token("[", L_SQRB);break;
        case ']': advance_lexer(lex);return register_token("]", R_SQRB);break;
        case '{': advance_lexer(lex);return register_token("{", L_CRLB);break;
        case '}': advance_lexer(lex);return register_token("}", R_CRLB);break;
        case ':': advance_lexer(lex);return register_token(":", COLON);break;
        default: break;//COM_ERROR("Unknown token, %c", lex->value)
    }

end:
    return register_token("\0", EOf);
}