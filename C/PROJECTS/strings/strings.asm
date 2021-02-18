.model tiny
.code

org 100h


;вывод строк с помощью строковых команд


str "This is string.", 0dh, 0ah, '$'
str_len db $ - str


end start
