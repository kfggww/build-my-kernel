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
    mov [bx+80*32], dl
    mov byte [bx+1+80*32], 0x84
    add bx, 2
    add si, 1
    loop show

    ;; 注意, 如果hlt指令放在db后面, 那么循环结束之后, CPU会从db处取指令, 会错误
    ;; 的把数据当成指令去执行
    hlt

msg:
    db "Hello, MBR!"

msg_len equ $ - msg

