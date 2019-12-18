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
    
    mov ax,-555
    call print
    mov ax,666
    call print
    
    MOV AH,4CH
    INT 21H
print proc
	;入口参数是ax，无出口参数
	mov bx,ax
	and bh,10000000B
	cmp bh,0
	je d
	neg ax
	mov dl,'-'
	push ax
	mov ah,2
	int 21h
	pop ax
	d:
	call digit
	ret
print endp
digit proc
	;还原现场前将受影响的寄存器压栈
    push bx
    push cx
    push dx
    ;cx作为计数器
    mov cx,0
    ;do-while循环求每一位
s:  mov bl,10
    div bl
    push ax
    inc cx
    cmp al,0
    je n
    mov ah,0
    jmp s
    ;输出
n:  pop ax
	mov dl,ah
	add dl,48
	mov ah,2
	int 21h
	loop n
	mov dl,10
	mov ah,2
	int 21h
	mov dl,13
	mov ah,2
	int 21h
	;还原现场
	pop dx
	pop cx
	pop bx
	
	ret
digit endp
CODES ENDS
    END START
