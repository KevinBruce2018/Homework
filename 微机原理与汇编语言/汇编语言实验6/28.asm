DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    ;随便给ax一个数
    mov ax,65
    mov cx,16
s:
    mov bx,ax
    push ax
    and bx,8000H
    cmp bx,0
    je f
    mov dl,49
    jmp p
f:
    mov dl,48
p:
    mov ah,2
    int 21h
    pop ax
    rol ax,1
    loop s
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START

