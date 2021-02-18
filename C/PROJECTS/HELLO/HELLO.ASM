.model tiny
.code
org 100h

start:	mov ah, 09h
	mov dx, offset Msg
	int 21h

	mov ah, 4ch
	int 21h

Msg	db "Hello, Ded!", 0dh, 0ah, '$'

end	start
