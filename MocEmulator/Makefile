.PHONY: run
.PHONY: clean

FILES = assembler/assembler.cpp assembler/bytecode.cpp assembler/lang/lexer.cpp assembler/lang/parser.cpp
C = g++
FLAGS = -std=c++2a -o

run:
	${C} main.cpp ${FILES} ${FLAGS} main.o
	./main.o

clean:
	rm -rf *.o
