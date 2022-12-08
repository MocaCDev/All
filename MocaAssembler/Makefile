C = gcc
emulator_path = emulator/
EMU_FLAGS = -lSDL2 -lSDL2main -lSDL2_ttf

.PHONY: assembler
.PHONY: emulator
.PHONY: all
.phony: clean

all: assembler emulator

assembler:
	gcc main.c MocaAsm/lexer.c MocaAsm/parser.c MocaAsm/parser16.c MocaAsm/assembler/assembler.c -o main.o

emulator:
	gcc $(emulator_path)emu_main.c $(emulator_path)BIOS/BIOS_emulator.c $(emulator_path)/disassembler/disassemble.c $(EMU_FLAGS) -o emu_main.o

clean:
	rm -rf *.o