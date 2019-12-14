DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS



STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    ;输入
    mov ah,1
    int 21h
    push ax
    mov ah,1
    int 21h
    push ax
    mov ah,1
    int 21h
    push ax
    
    mov bx,0
    ;处理个位
    pop cx
    sub cx,48
	mov ch,0
    add bx,cx
    ;处理十位
    pop cx
    sub cx,48
    mov ch,0
    mov ax,cx
    mov dx,10
    mul dl
    add bx,ax

    ;处理百位
	pop cx
	sub cx,48
    mov ch,0
    mov ax,cx
    mov dx,100
    mul dl
    add bx,ax
    
    mov ax,bx
    
    ;打印要输出的数值
    mov cx,3
    mov bl,10
s:  div bl
	push ax
	mov ah,0  
    loop s
    mov cx,3
h:  pop ax
	mov dl,ah
	add dl,48
	mov ah,2
	int 21h
	loop h
    MOV AH,4CH
    INT 21H
    
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START



