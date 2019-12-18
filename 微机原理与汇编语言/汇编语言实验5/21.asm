DATAS SEGMENT
    a1 dw 5
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
    mov ax,ds:[bx]
    call fun
    
    
    
    MOV AH,4CH
    INT 21H
fun proc
	push bx
	push cx
	push dx
	;函数入口是ax 不需要出口参数
    mov cx,ax
p:  sub cx,1
    cmp cx,0
    je s
    mul cx
    jmp p
    
    
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
	pop dx
	pop cx
	pop bx
	ret
fun endp
CODES ENDS
    END START


