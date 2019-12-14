DATAS SEGMENT
    ;此处输入数据段代码  
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
	;如果结果太大的话会算不出正确的结果
	;对于该段代码，前三位不能大于255，否则会溢出
    MOV AX,DATAS
    MOV DS,AX
    
    mov cx,4
    mov bx,0
    ;输入一个4位数并输出
i:  mov ah,1
	int 21h
	;al的值存到dl中
	mov dl,al
	sub dl,48
	mov dh,0
	;bl的值存到al中*10
	mov al,bl
	mov ah,10
	mul ah
	mov bx,ax
	add bx,dx
	 
    loop i
    ;初始化  作为打印结果前的准备
    mov ax,bx
    mov bl,10
    mov cx,4
    
s:  div bl
	push ax
	mov ah,0  
    loop s
    mov cx,4
	;打印结果
h:  pop ax
	mov dl,ah
	add dl,48
	mov ah,2
	int 21h
	loop h
    
    
    MOV AH,4CH
    INT 21H
CODES ENDS
    END START
