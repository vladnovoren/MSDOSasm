.model tiny
.code
org 100h

start:
	mov ax, 1
	mov bx, 4 * ax

	mov ah, 4ch
	int 21h


end start