#include "common.h"
#include "emulator/BIOS/BIOS.h"
#include "MocaAsm/lexer.h"
#include "MocaAsm/parser.h"
#include "MocaAsm/assembler/assembler.h"
#include "emulator/hardware/registers.h"

/*typedef struct Test {
    UNS8        *value;
    void        (*func_call)(void *parser);
} _Test;

typedef struct Parser {
    _Test           *t;
    struct Parser   *next;
} _Parser;

static _Parser *parser;
static _Parser *current_parser;
UNS32   amnt = 0;

void test(void *testit)
{
    printf("%s", ((_Parser *)testit)->t->value);
}*/

UNS8 *read_file(const UNS8 *filename)
{
    FILE* f = fopen(filename, "r");

    com_assert(f, "The file %s does not exist", filename)

    UNS8 *data = NULL;
    
    // Get size of file
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    com_assert(size > 0, "The file %s is empty", filename)

    data = calloc(size, sizeof(*data));
    fread(data, size, sizeof(UNS8), f);
    fclose(f);

    return data;
}

int main(int args, char **argv)
{
    com_assert(args > 1, "No input file.\n\t\tRerun with: `moca assembly_file`\n")
    //COM_ASSERT(TEST_OP(a, D) == true, "FAILED")

    bool is16 = false;
    UNS8 *filename;
    UNS8 *output = "out.bin";

    for(UNS32 i = 0; i < args; i++)
    {
        if(strcmp(argv[i], "-f") == 0) {
            i++;
            if(argv[i]) filename = argv[i];
            else com_error("-f flag expects a file.\n")
            
            continue;
        }
        if(strcmp(argv[i], "-m16") == 0) {
            is16 = true;
            continue;
        }
        if(strcmp(argv[i], "-o") == 0) {
            i++;
            if(argv[i]) output = argv[i];
            else com_error("-o flag expects a file.\n")

            continue;
        }
    }

    _Parser *parser = init_parser(
        init_lexer(read_file(filename)), is16, output
    );
    begin_assemble(parser);

    /*parser = parser_assign(
        init_lexer(
            read_file(argv[1])
        )
    );
    begin_assemble(parser);*/
    /*parser = calloc(1, sizeof(*parser));
    //current_parser = parser;
    parser->t = calloc(1, sizeof(*current_parser->t));

    parser->t->func_call = test;
    parser->t->value = "HEY";

    parser->next = calloc(1, sizeof(*parser->next));
    current_parser = parser->next;
    current_parser->t = calloc(1, sizeof(*current_parser->t));
    amnt++;

    current_parser->t->func_call = test;
    current_parser->t->value = "BYE";
    amnt++;

    current_parser->next = calloc(1, sizeof(*current_parser->next));

    current_parser = parser;

    for(UNS32 i = 0; i < amnt; i++)
    {
        current_parser->t->func_call((void *)current_parser);
        current_parser = current_parser->next;
    }*/
    
    /*interrupts = (_BiosInterrupt *) calloc(1, sizeof(*interrupts));
    interrupts->interrupt_id = SET_VID_MODE;
    interrupts->regs_used[0] = ah;
    interrupts->regs_used[1] = al;

    
    for(UNS8 i = 0; i < 2; i++)
    {
        if(TYPEOF(interrupts->regs_used[i]) == 8)
        {
            interrupts->register_data[i].register_8bit = interrupts->regs_used[i];
            interrupts->register_data[i].data8bit = 0x01;
        }
    }
    for(UNS8 i = 0; i < 2; i++)
    {
        if(TYPEOF(interrupts->regs_used[i]) == 8)
        {
            printf("Type: %X\tData: %X\n", 
                interrupts->register_data[i].register_8bit,
                interrupts->register_data[i].data8bit);
        }
    }*/
    //printf("%d\t%d", interrupts->interrupt_id, interrupts->next->interrupt_id);

    //printf("%d", interrupts->next->interrupt_id);
    //COM_ASSERT(1 < 1, "Error! %d is not < %d", 1, 1);
}