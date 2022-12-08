.PHONY: run
.PHONY: clean
.PHONY: build

FLAGS = -masm=intel -O1 -Wno-error -c -nostdinc -nostdlib -fno-builtin -fno-stack-protector -ffreestanding -m32
run: build
	@qemu-system-i386 -drive format=raw,file=b.bin

build:
	@gcc -std=gnu2x ${FLAGS} -o bootloader.o bootloader.c
	@gcc -std=gnu2x ${FLAGS} -o second_stage.o second_stage_boot.c
	@ld -m elf_i386 -Tlinker2.ld -nostdlib --nmagic -o second_stage.out second_stage.o
	@ld -m elf_i386 -Tlinker.ld -nostdlib --nmagic -o boot.out bootloader.o
# Literally taking the file and turning it into a binary file
	@ { head -c 512 > boot.bin; } < boot.out
	@objcopy -O binary boot.out boot.bin
	@objcopy -O binary second_stage.out second_stage.bin
#@cat boot.bin second_stage.bin > b.bin
	@cat boot.bin second_stage.bin > b.bin

clean:
	@rm -rf *.out
	@rm -rf *.o
	@rm -rf *.bin