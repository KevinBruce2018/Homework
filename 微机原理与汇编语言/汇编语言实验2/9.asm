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
    ;初始化操作
    mov ax,1526
    mov cx,4
    mov bl,10
	;求每次的商和余数
s:  div bl
	push ax
	mov ah,0  
    loop s
    mov cx,4
	;打印结果
h:  pop ax
	mov dl,ah
	add dl,48
	mov ah,2
	int 21h
	loop h
	
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START

