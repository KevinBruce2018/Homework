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
    
    mov ax,4e58H
    call print_hex
    
    MOV AH,4CH
    INT 21H
print_hex proc
	;入口参数是ax,无出口参数
	push bx
	push cx
	push dx
	mov cx,4
	s:
	mov bx,ax
	push ax
	and bh,11110000B
	;软件实现不够好 用不了rlr bh,4
	ror bh,1
	ror bh,1
	ror bh,1
	ror bh,1
	cmp bh,9
	ja hex
	mov dl,bh
	add dl,48
	
	jmp over
	hex:
	mov dl,bh
	add dl,87
	
over:
	mov ah,2
	int 21h
	pop ax
	rol ax,1
	rol ax,1
	rol ax,1
	rol ax,1
	loop s
	pop dx
	pop cx
	pop bx
	ret
print_hex endp
CODES ENDS
    END START
