#ifndef Moc_Parser
#define Moc_Parser
#include "../../common.hpp"
#include "lexer.hpp"

namespace MocParser
{
	class Parser
	{
	public:
		typedef struct MParser
		{
			int a;
		} _MParser;

	public:
		Parser()
		{
			std::cout << "Hello" << std::endl;
		}
	};
}

#endif
