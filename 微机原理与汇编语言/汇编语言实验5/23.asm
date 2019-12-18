DATAS SEGMENT
	;用于存放用户输入
    a1 db 0,0,0,0,0,0,0,0
    tips db "input a number:$"
    confirm db "Your input:$"
    sort_output db "After sorted:$"
    input_tip db "Input datas:$"
      
DATAS ENDS

STACKS SEGMENT
    ;此处输入堆栈段代码
STACKS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
	;程序说明
	;先输入一个8以内的数
	;输入完毕以后空格分隔，每次输入一个100以内的数
	;输入第n个数后回车即可
	;该程序按照非降序排列
    MOV AX,DATAS
    MOV DS,AX
    ;打印提示
    mov bx,offset tips
    call print_tip
    ;输入需要排序多少个数,存在cx中
    mov ah,1
    int 21h
    mov cl,al
    mov ch,0
    sub cl,48
    ;换行输入
    mov dl,10
    mov ah,2
    int 21h
    mov dl,13
    mov ah,2
    int 21h
    mov bx,offset input_tip
    call print_tip
    ;循环进行输入 以空格进行分开
    mov bx,offset a1
    ;入口参数是内存的位置bx和输入的数量cx
    call input
    
    mov bx,offset confirm
    call print_tip
    
    
    mov bx,offset a1
    call loop_print
    
    
    mov bx,offset sort_output
    call print_tip
    ;排序 cx中存在排序个数，bx存待排序数字的起始位置
    mov bx,offset a1
    call sort
    
    mov bx,offset a1
    call loop_print
    
    MOV AH,4CH
    INT 21H
print_tip proc
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
	mov dl,10
    mov ah,2
    int 21h
    mov dl,13
    mov ah,2
    int 21h
	pop dx
	pop bx
	pop ax
	ret
print_tip endp

input proc
	push ax
	push cx
	push dx
	mov dx,0
inputs:
muladd:
    mov ah,1
    int 21h
    ;遇到空格或者回车视为一个数字
    cmp al,32
    je next
    cmp al,13
    je next
    sub al,48
    ;内存中数据移动到al,al中数据暂存到dl
    mov dl,ds:[bx]
    mov dh,al
    mov al,dl
    ;al*10+dl
    mov dl,10
    mul dl
    add al,dh
    ;al数据放回内存
    mov ds:[bx],al
    ;不是空格，接着读
    jmp muladd
    ;遇到空格，读下一个数
next:
    inc bx
    loop inputs
    pop dx
    pop cx
    pop ax
	ret
input endp


print_num proc
	push ax
	push cx
	push bx
	push dx
	;先随便给ax一个值
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
	pop dx
	pop bx
	pop cx
	pop ax
	ret
print_num endp

loop_print proc
	;循环打印内存中的数据
	;入口参数为bx，即内存的偏移地址
	;无出口参数
	push ax
	push dx
	push cx
	push dx
    cal:
    mov ah,0
    mov al,ds:[bx]
    ;函数入口是ax，数据来源为内存
    call print_num
    mov dl,32
    mov ah,2
    int 21h
    inc bx
    loop cal
    mov dl,10
    mov ah,2
    int 21h
    mov dl,13
    mov ah,2
    int 21h
    
    pop dx
    pop cx
    pop bx
    pop ax
	ret
loop_print endp

sort proc
	push ax
	push bx
	push cx
	push dx
	push si
	push di
	;ax bx用于存内存中数据地址及比较
	;di存储数据首地址
	mov di,bx
	;dx存n-1，冒泡的第二个循环比较n-1-i的值
	mov dx,cx
	sub dx,1
sort_loop:
	;si作为计数器，用于存储i的值
	mov si,0
begins:
	;取出两个数并比较
	mov ah,ds:[bx]
	inc bx
	mov al,ds:[bx]
	cmp ah,al
	jns replace
	inc si
	cmp dx,si
	jna hh
	jmp begins
replace:
	;内存中的值进行互换
	mov ds:[bx],ah
	sub bx,1
	mov ds:[bx],al
	inc bx
	inc si
	cmp dx,si
	jna hh
	jmp begins
hh:	
	mov bx,di
	loop sort_loop
	
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	ret
sort endp
CODES ENDS
    END START

