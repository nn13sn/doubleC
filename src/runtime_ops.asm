section .bss
global current_instruction 
current_instruction: resq 1 

section .data 

section .text 

extern PrintError

global sumInt 
global subInt  
global mulInt  
global divInt   
global modInt 
global LogicLess 
global LogicGreater
global LogicLessEQ 
global LogicGreaterEQ 
global LogicEqual
global LogicNotEqual 

sumInt: 
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16



cmp rcx, 1 ;1 = Datatype Double  
je sumDouble
cmp rcx, 4 ;4 = Datatype String 
je string_err

cmp qword [rsp + 16], 1 
je sumDouble 
cmp qword [rsp + 16], 4 
je string_err

add qword [rsp + 8], rax 
mov qword [rsp + 16], 0 ;0 = Datatype Int
ret 

sumDouble:
movq xmm0, rax 
addsd xmm0, qword [rsp + 8]
movq qword [rsp + 8], xmm0 
mov qword [rsp + 16], 1 
ret 


subInt:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je subDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je subDouble 
cmp qword [rsp + 16], 4 
je string_err

sub qword [rsp + 8], rax 
mov qword [rsp + 16], 0 ;0 = Datatype Int
ret 

subDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
subsd xmm1, xmm0
movq qword [rsp + 8], xmm1 
mov qword [rsp + 16], 1 
ret 


mulInt: 
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je mulDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je mulDouble
cmp qword [rsp + 16], 4 
je string_err

imul rax, qword [rsp + 8]
mov qword [rsp + 8], rax
mov qword [rsp + 16], 0 ;0 = Datatype Int
ret 

mulDouble:
movq xmm0, rax
mulsd xmm0, qword [rsp + 8]
movq qword [rsp + 8], xmm0 
mov qword [rsp + 16], 1 
ret 

divInt:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rax, 0
je div_zero_err

cmp rcx, 1 ;1 = Datatype Double  
je divDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je divDouble 
cmp qword [rsp + 16], 4 
je string_err

mov rcx, rax 
mov rax, qword [rsp + 8]
cqo
idiv rcx 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 0 ;0 = Datatype Int
ret 

divDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
divsd xmm1, xmm0
movq qword [rsp + 8], xmm1 
mov qword [rsp + 16 ], 1 
ret 

modInt:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rax, 0
je div_zero_err

cmp rcx, 1 ;1 = Datatype Double  
je mod_err
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je mod_err
cmp qword [rsp + 16], 4 
je string_err

mov rcx, rax 
mov rax, qword [rsp + 8]
cqo
idiv rcx 
mov qword [rsp + 8], rdx
mov qword [rsp + 16], 0 ;0 = Datatype Int
ret 

LogicLess:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je LessDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je LessDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
setl al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

LessDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
setb al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 


LogicGreater:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je GreaterDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je GreaterDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
setg al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

GreaterDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
seta al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 

LogicLessEQ:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je LessEQDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je LessEQDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
setle al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

LessEQDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
setbe al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 

LogicGreaterEQ:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je GreaterEQDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je GreaterEQDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
setge al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

GreaterEQDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
setae al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 

LogicEqual:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je EqualDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je EqualDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
sete al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

EqualDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
sete al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 

LogicNotEqual:
mov rax, qword [rsp + 8] ;right value  
mov rcx, qword [rsp + 16] ;right value datatype
mov r8, qword [rsp]
mov qword [rsp + 16], r8 ;moving the return pointer to the end 
add rsp, 16

cmp rcx, 1 ;1 = Datatype Double  
je NotEqualDouble 
cmp rcx, 4 
je string_err

cmp qword [rsp + 16], 1 
je NotEqualDouble
cmp qword [rsp + 16], 4 
je string_err

cmp qword [rsp + 8], rax 
setne al
movzx eax, al 
mov qword [rsp + 8], rax
mov qword [rsp + 16], 3 ;0 = Datatype Bool
ret 

NotEqualDouble:
movq xmm0, rax
movq xmm1, qword [rsp + 8]
ucomisd xmm1, xmm0 
setne al 
movzx eax, al
mov qword [rsp + 8], rax 
mov qword [rsp + 16], 3 
ret 

string_err:
mov rdi, 1 
jmp runtime_error 

mod_err:
mov rdi, 2 
jmp runtime_error

div_zero_err:
mov rdi, 3
jmp runtime_error

runtime_error:
mov rsi, qword [rel current_instruction] 
and rsp, -16
call PrintError WRT ..plt
mov rdi, -1 
mov rax, 60
syscall 
