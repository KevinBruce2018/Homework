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
    ;2行3列开始显示
    mov ah,02H
    mov dh,2
    mov dl,3
    int 10h
    
    ;显示4个红色的*
    MOV AH,09H
    MOV BH,0
    MOV AL,'*'
    MOV BL,04H
    MOV CX,4
    INT 10H
    
    ;3行5列开始显示
    mov ah,02H
    mov dh,3
    mov dl,5
    int 10h
    ;显示1个红色的*
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


