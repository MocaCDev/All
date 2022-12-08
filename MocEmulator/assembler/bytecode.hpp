
#ifndef MocEmulator_Bytecode
#define MocEmulator_Bytecode
#include "../common.hpp"

// Different types of arguments for instructions
enum args
{
	reg8 = 1 << 0,
	reg16 = 1 << 1,
	mem8 = 1 << 2,
	mem16 = 1 << 3,
	d8 = 1 << 4,
	d16 = 1 << 5
};

// 8bit register being assigned a byte value from memory
// Example: mov ah, ref 0x100
#define RM8	(reg8 & mem8)

// 16bit register being assigned a 2 byte value from memory
// Example: mov ax, ref 0x100
#define RM16	(reg16 & mem16)

// 8bit register being assigned a byte value
// Example: mov ah, 0x0
#define RV8	(reg8 & d8)

// 16bit register being assigned a 2 byte value
// Example: mov ax, 0x0
#define RV16	(reg16 & d16)

// Max "args" per instruction. Types do not count, so, for example: mov ax, word 0x10 counts as 2 args, not 3
#define MAX_MOV_ARGS	2	// register, value(direct or from memory)
#define MAX_SH_ARGS		2	// register, amount to shift(right or left)
#define MAX_JMP_ARGS	1	// Address(or label) to jump to. jmp short addr(label)/jmp long addr(label) still counts as 1 argument.
#define MAX_CMP_ARGS	2	// register, compared value
#define MAX_INC_ARGS	1	// register

namespace MocBytecode
{
	// Bytecodes for mov instructions, based on the register in the instruction.
	enum class Bytecodes: UNS32
	{
		// 			mov reg 											mov ref reg
		mov_ax = 0xA0, mov_ah = 0xA4, mov_al = 0xA8, mov_ref_ax = 0xA1, mov_ref_ah = 0xA3, mov_ref_al = 0xA5,
		mov_bx = 0xAC, mov_bh = 0xB1, mov_bl = 0xB5, mov_ref_bx = 0xAD, mov_ref_bh = 0xB2, mov_ref_bl = 0xB6,
		mov_cx = 0xB9, mov_ch = 0xBD, mov_cl = 0xC2, mov_ref_cx = 0xBA, mov_ref_ch = 0xBE, mov_ref_cl = 0xC3,
		mov_dx = 0xC6, mov_dh = 0xCA, mov_dl = 0xCE, mov_ref_dx = 0xC7, mov_ref_dh = 0xCB, mov_ref_dl = 0xCF,
	
		// Special bytecodes for registers: SP, BP, SS, FS, GS, DS and ES
		spec_1 = 0xA2, // fs
		spec_2 = 0xA6, // ds
		spec_3 = 0xAE, // gs
		spec_4 = 0xB3, // es
		spec_5 = 0xB7, // ss
		spec_6 = 0xBC, // sp
		spec_7 = 0xC0  // bp
	};
	
	// All of the registers supported in 16bit real mode.
	enum Registers
	{
		reg_ax, reg_ah, reg_al,
		reg_bx, reg_bh, reg_bl,
		reg_cx, reg_ch, reg_cl,
		reg_dx, reg_dh, reg_dl,
		red_ss, reg_sp, reg_bp,
		reg_es, reg_ds, reg_fs,
		reg_gs, reg_ip
	};

	// All instructions to expect
	enum instructions
	{
		iMOV_AX, iMOV_AH, iMOV_AL,
		iMOV_BX, iMOV_BH, iMOV_BL,
		iMOV_CX, iMOV_CH, iMOV_CL,
		iMOV_DX, iMOV_DH, iMOV_DL,
		iMOV_ES, iMOV_SS, iMOV_SP,
		iMOV_BP, iMOV_GS, iMOV_FS, iMOV_DS,
		iREF_MOX_AX, iREF_MOV_AH, iREF_MOV_AL,
		iREF_MOV_BX, iREF_MOV_BH, iREF_MOV_BL,
		iREF_MOV_CX, iREF_MOV_CH, iREF_MOV_CL,
		iREF_MOV_DX, iREF_MOV_DH, iREF_MOV_DL,
		iJMP, iCMP, iSHFT
	};

	// What type of value can the current register hold?
	enum class TypeOfHold: UNS32
	{
		tbyte,	// 8 bit value(either from memory, or direct)
		tshort,	// 16 bit value(either from memory, or direct)
		tword,	// 32 bit value(either from memory, or direct)
		tdword,	// 64 bit value(either from memory, or direct)
		// Strictly mov ref reg, value
		// NOTE: If you have `mov ref reg`, whatever value that register has is treated as a memory address,
		// and whatever value is assigned to it is the dereferenced value of the address.
		// Example: mov ax, 0x1020; mov ref ax, 0x10. The memory address 0x1020 now has the value 0x10
		// when dereferenced.
		ref_byte,	// 8 bit value from memory, strictly
		ref_short,	// 16 bit value from memory, strictly
		ref_word,	// 32 bit value from memory, strictly
		ref_dword,	// 64 bit value from memory, strictly
	};
	
	// In relation to AssemblerResp, this is the Bytecode class response. This will be given to the assembler response that the parser will then use
	// to make sure it is parsing the correct ideals.
	typedef struct Response
	{
		// TODO: More data? If any?
		// What value type to expect depending on the register.
		enum TypeOfHold value_type[2]; // if it is a "parent" register, it can have a byte value, or a word value.
		
		// Depending on the instruction from the assembler data, we will decipher what the actual bytecode is.
		Bytecodes bytecode;
	} _Response;
	
	class Bytecode
	{
	public:
		Bytecode();
	};
}

#endif
