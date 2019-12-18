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
    ;数字算出来了 不过这题没让打印 就不打印了
    call get_num
    
    MOV AH,4CH
    INT 21H
get_num proc
	push bx
	push cx
	;一开始先让bx存储当前乘得的结果
    mov bx,0
    
    ;输入字符同时计算得数
f:	mov ah,1
    int 21h
    cmp al,'a'
    je next
    sub al,48
    mov ah,0
    ;计算bx*10+ax 结果在存到bx中
    mov cx,ax
    mov ax,bx
    mov bx,10
    mul bx
    add ax,cx
    mov bx,ax
    
    jmp f
next:    
    ;算出的数字传到ax中
    mov ax,bx    
    pop cx
    pop bx
	ret
get_num endp
CODES ENDS
    END START


