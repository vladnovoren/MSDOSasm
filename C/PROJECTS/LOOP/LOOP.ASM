.model tiny
.code
org 100h

start:	mov bx, offset string


cycle:	mov dl, [bx]
	mov ah, 2
	int 21h

	mov al, [bx]
	inc bx
	cmp al, 0ah
	jne cycle

	
	ret

string 	db "Help poor children in Uganda!", 0dh, 0ah, 'S'

end 	start