use16
org 0x7E00

mov ax, 0x0800
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

jmp $

print:
	mov ah, 0x0E
.loop:
	mov 

failed:
	mov ah, 0x0E
	mov al, 'E'
	int 0x10

	cli
	hlt

times 512 - ($ - $$) db 0x0
