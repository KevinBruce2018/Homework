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
    
    mov ax,666
    call print_ax
    
    ;如果对函数需要二次调用的话，一些寄存器受到了影响
    ;可能影响到其它寄存器的值 最好进行还原现场
    ;此处的没涉及其它操作 应该不受影响 保险起见还是还原下
    mov ax,253
    call print_ax
    
    MOV AH,4CH
    INT 21H
    
    ;该函数的入口参数为ax，无出口参数
print_ax proc
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
	
	;还原现场
	pop dx
	pop cx
	pop bx
	ret
print_ax endp
CODES ENDS
    END START


