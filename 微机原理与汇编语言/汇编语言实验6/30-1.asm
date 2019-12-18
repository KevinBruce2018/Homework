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
    mov ax,3633H
    call print
    MOV AH,4CH
    INT 21H
print proc
	push dx
	mov dl,ah
	;入口参数是ax
	push ax
	mov ah,2
	int 21h
	pop ax
	mov dl,al
	mov ah,2
	int 21h
	pop dx
	ret
print endp
CODES ENDS
    END START
