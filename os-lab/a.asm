    ;; 使用显卡的文本模式在屏幕上输出信息

    mov ax, 0xb800
    mov ds, ax
    mov ax, 0x7c0
    mov es, ax

    mov cx, msg_len
    mov bx, 0
    mov si, 0

show:
    mov dl, [es:msg+si]
    mov [bx], dl
    mov byte [bx+1], 0x84
    add bx, 2
    inc si
    loop show

msg:
    db "Hello, MBR!"

msg_len equ $ - msg

    hlt
