;基本模型机程序

#LOAD "MXJ1.IS"         ;预调入指令系统/微程序
#SET RAM 05feh = 1234h  ;数据区0260H单元预置数据5500h
#SET RAM 05fch = 4433h

data    segment         ;将程序装载到数据存储器
        assume ds:data

        org   0
start:
        ;mov r0,iol
        ;push r0
       ;push r2
 
        pop r2
 s:     pop r0
        cmp r0,iol
        jz s
data    ends
        end   start
