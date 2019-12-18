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
    ;打印年份
    mov al,9
    out 70h,al
    in al,71h
    call Date
    mov dl,'-'
    mov ah,2
    int 21h
    ;打印月份
    mov al,8
    out 70h,al
    in al,71h
    call Date
    mov dl,'-'
    mov ah,2
    int 21h
    ;打印日期
    mov al,7
    out 70h,al
    in al,71h
    call Date
    
    MOV AH,4CH
    INT 21H
    ;注意时间是8421BCD码
Date proc
	;打印日期，入口参数是AL
	push bx
	push cx
	;打印高四位
	mov bl,al
	and al,11110000B
	mov cl,4
	shr al,cl
	mov ah,0
	call print
	;打印低四位
	mov al,bl
	and al,00001111B
	mov ah,0
	call print
	pop cx
	pop bx
	ret
Date endp
print proc
	;输出无符号十进制数，入口参数为AX
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
print endp
CODES ENDS
    END START

