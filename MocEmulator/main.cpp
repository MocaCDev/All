#include "common.hpp"
#include "assembler/assembler.hpp"

int main(int args, char **argv)
{	
	MocAssembler::Assembler a;

	a.run();
	return 0;
}