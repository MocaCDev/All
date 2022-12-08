#ifndef Moc_Lexer
#define Moc_Lexer
#include "../../common.hpp"

namespace MocLexer
{
	class Lexer
	{
	private:
		enum Tokens
		{
			mov = 0x20,
			v_byte, v_short, v_word, v_dword,
			comma, colon,
			left_sqrb, right_sqrb, // [ ]
			v_org, v_cmp, v_inc,
			label, // any value that does not register as a instruction
			/*
			 * je = Jump Equal, jz = Jump Zero, jne = Jump Not Equal, jnz = Jump Not Zero,
			 * jg = Jump Greater, jnle = Jump Not Less than Equal to, jge = Jump Greater Than Equal To,
			 * jnl = Jump Not Less Than, jl = Jump Less Than, jnge = Jump Not Greater Than Equal To,
			 * jle = Jump Less Than Equal To, jng = Jump Not Greather Than
			 * */
			v_je, v_jz, v_jne, v_jnz, v_jg, v_jnle, v_jge, v_jnl, v_jl, v_jnge, v_jle, v_jng,
			EOf, // File is at its end.
		};

		typedef struct TokenAndValue
		{
			enum Tokens	t;
			UNS8		*v;
		} _TAV;

		// All tokens, and there corresponding values.
		_TAV tmov   =	(_TAV) {.t = mov, .v = (UNS8 *)"mov"};
		_TAV tbyte  =	(_TAV) {.t = v_byte, .v = (UNS8 *)"byte"};
		_TAV tshort =	(_TAV) {.t = v_short, .v = (UNS8 *)"short"};
		_TAV tword  = 	(_TAV) {.t = v_word, .v = (UNS8 *)"word"};
		_TAV tdword =	(_TAV) {.t = v_dword, .v = (UNS8 *)"dword"};
		_TAV torg   =	(_TAV) {.t = v_org, .v = (UNS8 *)"org"};
		_TAV tcmp   = 	(_TAV) {.t = v_cmp, .v = (UNS8 *)"cmp"};
		_TAV tinc   =	(_TAV) {.t = v_inc, .v = (UNS8 *)"inc"};
		_TAV tje    =	(_TAV) {.t = v_je, .v = (UNS8 *)"je"};
		_TAV tjz    = 	(_TAV) {.t = v_jz, .v = (UNS8 *)"jz"};
		_TAV tjne   =	(_TAV) {.t = v_jne, .v = (UNS8 *)"jne"};
		_TAV tjnz   =	(_TAV) {.t = v_jnz, .v = (UNS8 *)"jnz"};
		_TAV tjg    =	(_TAV) {.t = v_jg, .v = (UNS8 *)"jg"};
		_TAV tjnle  =	(_TAV) {.t = v_jnle, .v = (UNS8 *)"jnle"};
		_TAV tjge   =	(_TAV) {.t = v_jge, .v = (UNS8 *)"jge"};
		_TAV tjnl   =	(_TAV) {.t = v_jnl, .v = (UNS8 *)"jnl"};
		_TAV tjl    =	(_TAV) {.t = v_jl, .v = (UNS8 *)"jl"};
		_TAV tjnge  =	(_TAV) {.t = v_jnge, .v = (UNS8 *)"jnge"};
		_TAV tjle   =	(_TAV) {.t = v_jle, .v = (UNS8 *)"jle"};
		_TAV tjng   =	(_TAV) {.t = v_jng, .v = (UNS8 *)"jng"};
	public:
		typedef struct MLexer
		{
			UNS8	*data;
			UNS8	current;
			UNS32	line;
			SIZE	file_size;
		} _MLexer;

		typedef struct MToken
		{
			enum Tokens	token;
			UNS8		*t_value;
		} _MToken;
	public:
		template<UNS8 *data, SIZE size>
		Lexer() {
			_MLexer *lexer = new _MLexer;

			lexer->data = data;
			lexer->file_size = size;
		}
	};
}

#endif
