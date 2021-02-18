.model tiny
.code

VIDEOSEG	equ 0b800h

org 100h
Start: 		mov ah, 09h
		mov dx, offset Message
		int 21h
		
		mov bx, VIDEOSEG
		mov es, bx

		