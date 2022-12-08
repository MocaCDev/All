#ifndef MocEmulator_Assembler
#define MocEmulator_Assembler
#include "../common.hpp"
#include "bytecode.hpp"
#include "lang/parser.hpp"

namespace MocAssembler
{
	using namespace MocParser;
	using namespace MocLexer;
	using namespace MocBytecode;
	
	typedef struct AssemblerData
	{
		UNS8	*val;

		// What is the instruction?
		enum instructions instruction;
	} _AssemblerData, *asm_data;

	// AssemblerResp will be filled out throughout the instruction.
	// The assemblers response will just help us figure out what registers we should expect
	typedef struct AssemblerResp
	{
		// What flag is it? RM8, RM16? DV8, DV16?
		UNS8		flag;

		// Amount of arguments for the instruction?
		UNS8		amnt_args;

		// Depending on the RVALUE, what can we expect from the LVALUE?
		// Example: mov ref reg will expect a memory address.
		// 			mov reg can expect a referenced value, a direct value(depending on the size of register), or another register
		enum args 	lval_expect;
	} _AssemblerResp, *asm_resp;

	class Assembler
	{
	private:
		// Setting the assemblers instance of the parser and lexer to NULL
		Parser::_MParser *_par = NULL;
		Lexer::_MLexer *_lex = NULL;

		// Instruction pointer. Where are we in the instruction?
		UNS8	ip = 0;

	public:
		// Assign the instances of the parser and lexer
		Assembler()
		{
			_par = new Parser::_MParser;
			_lex = new Lexer::_MLexer;
		}

		void run();
	};
}

#endif
