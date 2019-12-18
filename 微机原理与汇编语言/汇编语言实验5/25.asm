DATAS SEGMENT
    a1 db "Hello$"
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    mov bx,offset a1
    call fun 
    
    
    MOV AH,4CH
    INT 21H
fun proc
	;设置一个入口参数
	;这里设计字符串的偏移地址的首地址作为入口参数
	;用到的入口参数的寄存器为bx
	push ax
	push bx
	push dx

s:	
	mov al,ds:[bx]
	cmp al,'$'
	je e
	mov dl,al
	mov ah,2
	int 21h
	inc bx
	jmp s
e:	
	pop dx
	pop bx
	pop ax
	ret
fun endp
CODES ENDS
    END START



