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
    
    mov ax,526
    mov bl,10
    
    ;第一次求商和余数并压栈
    div bl
    push ax
    mov ah,0
    ;第二次求商和余数并压栈
    div bl
    push ax
    mov ah,0
    ;第三次求商和余数并压栈
    div bl
    push ax
    mov ah,0
    ;依次打印每个数
    pop ax
    mov dl,ah
    add dl,48
    mov ah,2
    int 21h
    
    pop ax
    mov dl,ah
    add dl,48
    mov ah,2
    int 21h
    
    pop ax
    mov dl,ah
    add dl,48
    mov ah,2
    int 21h
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START

