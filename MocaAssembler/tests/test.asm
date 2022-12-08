; Normal assembly mov instruction(16bit, 32bit)
mov ax, 0x100
mov ah, 0x101

; New type of way to reference memory
ref dd Addr     ; ref is a keyword in MocaAsm that tells the asembler
                ; that the given `variable` is always going to
                ; reference a value at the address assigned to it

; Example, the following code will store whatever value was
; at the address 0x1234 in Addr
mov Addr, 0x1234

; If you're still kinda on edge with syntax, go ahead and invoke
; some nasm syntax and surround `Addr` with square brackets.
; To the assembler, without the square brackets, the same thing happens.
; The `ref` before the declaration of the variable `Addr` told the
; assembler that this variable is always assigned with [Addr].
mov [Addr], 0x1234 ; this does the same as mov Addr, 0x1234

; If we're wanting to assign a value stored at a memory address to a
; register, we can also use the ref keyword:
mov ah, ref 0x9090      ; this tells the assembler to store whatever
                        ; value 0x9090 has stored in it to `ah`

; Instead of the traditional cli - turn off interrupts sti - turn them on
; MocaAsm uses @
; example:
@mov bp, 0x7C00     ; this instruction disables interrupts(cli)
                    ; during the instruction.
                    ; @mov just tells the assembler to clear
                    ; interrupts during this instruction


; In MocaAsm, we can use @keep to keep the value of a register
; until we use @dump.
@keep ah, 0x0E      ; @keep is a interrupt-specific keyword.
                        ; when @keep is invoked, the assembler will
                        ; expect the user to call an interrupt

; To explain furthermore:
; @keep makes sure that we cannot give the register a new value/reassign
; the register. So, if we have:
; @keep ah, 0x0e -> ah will keep the value 0x0e
; if we tried doing mov ah, 0x0F, we would get:
;   Attempting to assign a value to a `locked` register. Make sure to
;   `dump` the register with `@dump register` to reassign.
; @dump tells the assembler: "Hey, we want to modify this register again"

; example of using this: reading from the disk. We keep `keep` the register
; ah as the function call between interrupts. As long as we don't
; invoke @dump on the register, the value will be kept and the assembler
; will continue to look for interrupts.

; Syntax: @keep register, (optional)value
@keep ah, 0x02

mov al, 0x01    ; number
mov ch, 0x00
mov cl, 0x00
mov dh, 0x00
mov dl, 0x80
int 0x13        ; after this, the assembler will expect either:
                ; @dump reg ah, @dump all if there is more than one
                ; register being 'kept' or another interrupt after some code.
                ; If the assembler doesn't reach either, well then it assumes
                ; you wanted the value kept for that given register
                ; for the entirety of the program.


; refer is a keyword that allows users to still store data/values
; in a register, but not generate any bytecode for the instruction.
; This is dangerous, however. You will be forced to do many checking
; throughout the program and you'll have to make sure the value being
; assigned to the given register won't be needed later while it's running.
; We are 'referring' the register `ah` to the value `0x00`. We are not
; 'giving' the register ah the value 0x00.
refer mov ah, 0x00

; overload is a keyword that simply tells the assembler you are going
; to 'overload' the register. Another words, you're letting the assembler
; know that you are putting a value larger than what the register can hold..
; in the register. The assembler will then store this value on the 'stack',
; and if there are bitwise operations on the register, it will 'push' the values
; into the register while 'popping' them from the stack.
overload mov ah, 0x20305