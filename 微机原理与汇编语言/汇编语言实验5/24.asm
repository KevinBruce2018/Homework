DATAS SEGMENT
    a1 db 'H','W','C','$'
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    call fun 
    
    
    MOV AH,4CH
    INT 21H
fun proc
	;出口入口都没有
	push ax
	push bx
	mov bx,offset a1
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
	pop bx
	pop ax
	ret
fun endp
CODES ENDS
    END START


