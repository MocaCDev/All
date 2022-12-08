.PHONY: run
.PHONY: clean
.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
run: build
	@qemu-system-i386 -drive format=raw,file=boot.bin

build:
	@gcc -std=gnu2x ${FLAGS} -o bootloader.o bootloader.c
	@ld -m elf_i386 -Tlinker.ld -nostdlib --nmagic -o boot.out bootloader.o
# Literally taking the file and turning it into a binary file
	@objcopy -O binary boot.out boot.bin
#@ { head -c 512 > boot.bin; } < boot.out

clean:
	@rm -rf *.out
	@rm -rf *.o
	@rm -rf *.bin