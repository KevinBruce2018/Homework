DATAS SEGMENT
DATAS ENDS
STACKS SEGMENT
STACKS ENDS
CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    ;设定分辨率
    MOV AH,00H
    MOV AL,02H
    INT 10H
    ;设置光标初始位置为2行3列
    mov ah,02H
    mov dh,2
    mov dl,3
    int 10h
    ;设置*显示4次
    MOV AH,09H
    MOV BH,0
    
    MOV AL,'*'
    ;红色
    MOV BL,04H
    MOV CX,4
    INT 10H
    
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START


