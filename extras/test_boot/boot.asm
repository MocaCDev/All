use16
org 0x7C00

xor ax, ax
mov es, ax
mov ds, ax

cli
mov bp, 0x7C00
mov sp, bp
mov ss, ax
sti

mov ax, 0x07E0
mov es, ax
xor bx, bx

mov ah, 0x02
mov al, 0x01
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
mov dl, 0x80
int 0x13
jc failed

jmp 0x0:0x7E00

jmp $

failed:
	mov ah, 0x0E
	mov al, 'A'
	int 0x10
	
	cli
	hlt

times 510 - ($ - $$) db 0x0
dw 0xAA55
