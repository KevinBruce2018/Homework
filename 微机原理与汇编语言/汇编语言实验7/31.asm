DATAS SEGMENT
DATAS ENDS
STACKS SEGMENT
STACKS ENDS
CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    MOV AH,00H
    MOV AL,02H
    INT 10H
    ;设置显示的起始行和起始列为2,3
    mov ah,02H
    mov dh,2
    mov dl,3
    int 10h
    ;设置显示页数为0，显示一次*颜色为红色
    MOV AH,09H
    MOV BH,0
    MOV AL,'*'
    MOV BL,04H
    MOV CX,1
    INT 10H
    
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START


