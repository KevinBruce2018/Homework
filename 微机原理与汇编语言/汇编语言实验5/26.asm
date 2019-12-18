DATAS SEGMENT  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX
    
    call print
    
    MOV AH,4CH
    INT 21H
print proc
	push ax
	push dx
	;回车的ascii码
	mov dl,13
	mov ah,2
	int 21h
	
	pop dx
	pop ax
	ret
print endp
CODES ENDS
    END START
