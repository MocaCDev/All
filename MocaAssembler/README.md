<h1 style="font-size: 40px">MOCA ASSEMBLER</h1>

<p style="font-size: 18px">The assembler that simply changes your life and becomes your new love</p>

## What is Moca Assembler?
<p style="font-size: 14px">Moca assembler is a new assembler written <i>and owned</i> by MocaCDeveloper. The assembler supports all general ideals an assembler/the assembly language requires, such as bitwise operations, mov instructions, labels etc etc.</br>However, this assembler is not like the others. I decided to throw in a few new keywords to possibly cut down on the total amount of code needed to be written</p></br>

# New keywords?

<h3 style="font-size:22px">@keep</h3>
<p style="font-size: 14px">@keep is a <i>macro</i> when using Moca Assembler. What @keep does is it "locks" a register to a specific value. When would this be helpful? Lets say we need to do multiple syscalls/interrupts one after the other. Syscalls/interrupts <i>assign different values</i> to registers, making the programmer <i>reassign</i> the values back to the register that were <i>already</i> assigned. With @keep, fear no more. Regardless what value is stored in the registers during an interrupt, the value you told the assembler to "lock" stays with the register. Obviously, if a interrupt/syscall errors the assembler <i>won't</i> override the value assigned via the interrupt/syscall.</br>Slight disclaimer: Lets say you lock the register ax to a value and a syscall/interrupt decides to store the error code in that register. The assembler automatically "unlocks" the register, or, "dumps" the register. Which leads us to the next keyword(or macro?)</p>

<h3 style="font-size: 22px">@dump register, @dump all</h3>
<p style="font-size: 14px">@dump is another <i>macro</i> when using Moca Assembler. This macro "unlocks" or "dumps" the previous "state" of the specified register or <i>all of them</i>. If you "lock" only one register, then there is no point of doing @dump all. @dump all "unlocks" <i>all registers that are locked</i>. Calling @dump register/@dump all when there are <i>none</i> locked registers will just act like a mov instruction, assign a specified register to zero, or all of the registers. This functionality is more than likely to change in upcoming versions of Moca Assembler. Do not plan on being able to use @dump all to reassign all registers to zero for too long.</p>

<h3 style="font-size: 22px">ref</h3>
<p style="font-size: 14px">ref is a keyword using Moca Assembler that tells the assembler a few things. First, it tells the assembler that the <i>type</i> of the variable is to always be a dword. The reason? When declaring a variable with ref, there is no type required to go with it. You can have:</p>

```x86asm
ref db Myvar
```
<p style="font-size: 14px">and the type of 'MyVar' will still be a dword.</br>Why does this happen though? ref tells the assembler that, no matter what, whatever value is assigned to the variable is to be approached as a memory address, and thus the assemblers job is to "access" whatever value is at this memory address. Although it may not be anything valid, the assembler will still do it. Think of ref as making a pointer in assembly. It simply just tells the assembler to keep the type of the variable as a dword so it can be assigned to appropriate memory addresses, and it tells the assembler that whenever a value is assigned to the given variable, the variable instead gets the value <i>at the address</i>.</p>

<h3 style="font-size: 22px">refer</h3>
<p style="font-size: 14px">refer is a keyword using Moca Assembler that basically tells the assembler that the given instruction is to only be noticed during runtime. refer is like a preprocessor directive in C. You can have:</p>

```x86asm
refer mov ah, 0x20
```

<p style="font-size: 14px">and the assembler will generate no bytecode. Instead, the assembler <i>does</i> move the value 0x20 into the register ah, just during the assembling. refer is a keyword that allows users to access data or assign data <i>only</i> during the runtime of the assembler. Here is an example, since this is kinda hard to explain:</p>

```x86asm
; The variable 'SomeValue' is able to be used anywhere throughout the code. The assembler won't
; error when it reaches something like `mov SomeValue, 0x40`. However,
; after the assembler is done running, there is no bytecode generated for `SomeValue`. This variable
; was only accessible during the runtime.
refer SomeValue dw      0x200
```

<h3 style="font-size: 22px">overload</h3>
<p style="font-size: 14px">overload is a keyword using Moca Assembler that, well, <i>overloads a register</i>. As of now, I do not know the costs of using overload. However, I do not believe the costs of using it will result in a big "loss" of memory or runtime. Basically, overload just assigns a value to a register that is too large for the register to actually hold. Instead, it "caches" and stores the remaining value after the initial assignment elsewhere, and when bitwise operations are made on the register, the remaining value stored elsewhere gets "pushed", or assigned, to the register. If a mov instruction is made on the register that was overloaded, the "cached" value is then forgotten and the program moves on.</p>