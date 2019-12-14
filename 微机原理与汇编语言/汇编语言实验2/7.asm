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
    
    mov ax,26
    mov bl,10
    div bl
    ;将余数和商存起来
    push ax
    ;打印高位
    mov dl,al
    add dl,48
    mov ah,2
    int 21h
    ;打印低位
    pop dx
    mov dl,dh
    add dl,48
    mov ah,2
    int 21h
    
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START

