.186
.model tiny
.code

;----------------------------------------------------------------------------
;константы
;----------------------------------------------------------------------------
videosec  equ 0b800h
two_strings_size equ 2 * 2 * 80



org 100h

start:
        mov cx, 30d
        mov ah, 02h
        mov dl, 0ah
print_n:
        int 21h
loop    print_n

        call es_videosec
        call background
        mov si, offset test_str
        mov cx, 40
        mov di, two_strings_size + 2 * 6 * 80 + 2 * 20
        mov bl, 10
        call frame_open

        mov ah, 4ch
        int 21h

;----------------------------------------------------------------------------
; вспомогательная процедура передачи в es videosec
; утраченные регистры:
; bx, es
;----------------------------------------------------------------------------
es_videosec proc
        mov bx, videosec
        mov es, bx

        ret
es_videosec endp
 

;----------------------------------------------------------------------------
; процедура вывода фона на весь экран
; утраченные регистры:
; ax, cx, di
;----------------------------------------------------------------------------
background proc
        mov al, 0b1h
        mov ah, 00001111b
        mov cx, 2 * 28 * 80
        mov di, two_strings_size
loop_back:
        mov word ptr es:[di], ax
        inc di
        inc di
loop loop_back

        ret
        
background endp


;----------------------------------------------------------------------------
; процедура dx := 80 - dx
; утраченные регистры:
; ax, dx
;----------------------------------------------------------------------------
rev_dx proc
        mov ax, dx
        mov dx, 80
        sub dx, ax
        ret 
rev_dx endp


;----------------------------------------------------------------------------
; процедура вывода 1 символа в видеопамять
; ax - символ, который хотим вывести
; di - номер ячейки, с которой начать печать
; утраченные регистры:
; ax, di
;----------------------------------------------------------------------------
one_symbol proc
        mov word ptr es:[di], ax
        inc di
        inc di
        ret
one_symbol endp


;----------------------------------------------------------------------------
; процедура вывода символа в видеопамять n раз
; использованные сегменты:
; ax - символ, который хотим напечатать
; сx - количество печатуемых символов
; di - номер ячейки, с которой начать печать
; утраченные регистры:
; ax, cx, di
;----------------------------------------------------------------------------
print_n_chars proc
        rep stosw
        
        ret

print_n_chars endp



;----------------------------------------------------------------------------
; процедура вывода строки с особыми символами по краям
; si - адрес стоки из трех слов, среднее будет повторено n раз
; cx - длина всей строки
; di - номер ячейки, с которой печатать
; утраченные регистры:
; ax, cx, di, si
;----------------------------------------------------------------------------
line_with_boards proc
        mov word ptr ax, [si] ; кладем в ax первый символ
        call one_symbol ; печатаем левый край

        add si, 2 ; переходим ко второму символу
        mov word ptr ax, [si]
        sub cx, 2
        call print_n_chars
        add cx, 2

        add si, 2 ; переходим к правому краю
        mov word ptr ax, [si]
        call one_symbol
        add si, 2
       
        ret
line_with_boards endp



;----------------------------------------------------------------------------
; процедура, рисующая тень для рамки
; cx - длина рамки
; bl - высота рамки
; di - нижний левый угол рамки
; утраченные регистры:
; dl, cx, di
;----------------------------------------------------------------------------
print_shadow proc
        mov dl, 11110111b
        inc di
        inc di
        dec cx
lower_shadow:
        and byte ptr es:[di + 1], dl
        inc di
        inc di
loop    lower_shadow

        mov cl, bl

side_shadow:
        and byte ptr es:[di + 1], dl
        int 3
        sub di, 2 * 80
loop side_shadow

        ret

print_shadow endp


;----------------------------------------------------------------------------
; вывод строки в рамку
; di - левый верхний угол рамки
; утраченные регистры:
; si
;----------------------------------------------------------------------------
print_str_in_frame proc
        mov si, offset uganda
        
print_str_in_frame endp


;----------------------------------------------------------------------------
; вывод рамки
; si - адрес строки, задающей символы рамки
; cx - длина рамки
; bl - высота рамки
; di - номер ячейки, с которой начать печать
; утраченные регистры:
; ax, bx, cx, dx, si, di
;----------------------------------------------------------------------------
print_frame proc
        mov dx, cx ; прячем cx
        push bx ; сохраняем bl
        call line_with_boards
        call rev_dx 
        add di, dx
        add di, dx

        dec bl
        dec bl
middle:
        call rev_dx
        mov cx, dx
        call line_with_boards
        call rev_dx
        add di, dx
        add di, dx


        dec bl
        sub si, 6

        cmp bl, 0
jne     middle
        add si, 6

        call rev_dx
        mov cx, dx
        call line_with_boards
        pop bx ; вернули bl
        mov cx, dx
        call rev_dx
        add di, dx
        add di, dx
        call print_shadow

        ret
print_frame endp


;----------------------------------------------------------------------------
; процедура, разворачивающая рамку с задержкой
; si - адрес строки, задающей символы рамки
; cx - длина рамки
; bl - высота рамки
; di - верхний левый угол рамки
; утраченные регистры:
; ax, bx, cx, dx, si, di
;----------------------------------------------------------------------------
frame_open proc
        mov ax, cx
        push cx ; сохранили cx
        mov bh, 2
        div bh ; al := cx / 2
        xor ch, ch
        mov cl, ah
        inc cx
        inc cx ; cx := cx % 2 + 2
        xor ah, ah ; ax := cx / 2
        dec ax
        add di, ax
        add di, ax

frame_loop:

        push di
        push bx
        push cx
        call print_frame
        sub si, 18

        xor cx, cx ; вызов задержки
        mov dx, 11111
        mov ax, 8600h
        int 15h

        pop cx
        pop bx
        pop di
        dec di
        dec di
        inc cx
        inc cx

        mov bp, sp
        cmp word ptr cx, [bp]
jbe     frame_loop
        inc sp
        inc sp
        ret
frame_open endp



uganda db 'Help poor children in Uganda!', '$'
uganda_size db $ - uganda

test_str dw 4ac9h, 4acdh, 4abbh, 4abah, 0ffffh, 4abah, 4ac8h, 4acdh, 4abch

end start
