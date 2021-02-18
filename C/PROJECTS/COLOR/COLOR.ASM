.model tiny
.code
org 100h

start:	mov ah, 00h ; set video mode
	mov si, offset string


cycle:	mov al, [si]
	mov bh, 1
	mov bl, 12
	int 10h

	mov al, [si]
	inc si
	cmp al, 0ah
	jne cycle

	mov ah, 4ch
	int 21h

string 	db "Help poor children in Uganda!", 0dh, 0ah, 'S'

end 	start