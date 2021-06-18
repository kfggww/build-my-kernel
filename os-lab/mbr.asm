;;; 加载内核的数据段
    kernel_data_section equ 0x7e0

;;; 显示欢迎信息
    section mbr_code vstart=0x7c00
    mov ax, 0xb800
    mov ds, ax

    mov cx, msg_size
    mov bx, 0
    mov di, 0

greeting:
    mov al, [cs:greeting_msg + bx]
    mov [di], al
    mov byte [di+1], 0x84
    inc bx
    add di, 2
    loop greeting

;;; 设置栈段
    mov ax, 0
    mov ss, ax
    mov sp, 0x7c00


;;; 从硬盘上的100号扇区读取内核到内存中
    mov ax, kernel_data_section
    mov ds, ax
    mov bx, 0
    call read_one_sector

    hlt


;;; ----------------------------------------------------------------------------
;;; 读取硬盘上的一个扇区
;;; 参数:
;;; bx, 起始扇区编号, 目前支持的扇区编号范围是[0,2^16-read]
read_one_sector:
    push ax
    push bx
    push cx
    push dx

    ; 设置读取扇区个数
    mov dx, 0x1f2
    mov al, 1
    out dx, al

    ; 设置起始扇区编号
    mov dx, 0x1f3
    mov al, bl
    out dx, ax

    mov al, bh
    mov dx, 0x1f4
    out dx, ax

    mov dx, 0x1f5
    out dx, ax

    mov al, 0xe0
    mov dx, 0x1f6
    out dx, al

    ; 开始读取
    mov al, 0x20
    mov dx, 0x1f7
    out dx, al

    ;等待读取完成
waiting:
    in al, dx
    mov bl, al                  ; 检查是否出错
    and bl, 0x01
    cmp bl, 0x01
    jz error
    and al, 0x88
    cmp al, 0x08                ; 设备不忙且数据已经就绪
    jnz waiting


    ; 读取数据到ds指定的段
    mov cx, 256
    mov bx, 0
    mov dx, 0x1f0
copy_data:
    in ax, dx
    mov [bx], al
    add bx, 2
    loop copy_data

error:
    pop dx
    pop cx
    pop bx
    pop ax

    ret

greeting_msg:
    db "Welcome to TinyOS!", 0x0d, 0x0a

msg_size equ $ - greeting_msg

    times 510-($-$$) db 0
    db 0x55, 0xaa
