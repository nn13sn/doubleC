section .bss
global current_instruction 
current_instruction: resq 1 

section .rodata 
align 8 
int64_min: dq 0xC3E0000000000000  ; -2^63
int64_max: dq 0x43E0000000000000 ; +2^63

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
global GeneralCast  
global UnaryNeg 
global UnaryNot

GeneralCast:   ;rdi - datatype to cast into 
mov rsi, qword [rsp + 8]
cmp rdi, 0 
je .toint
cmp rdi, 1 
je .todouble 
cmp rdi, 2 
je .tochar 
cmp rdi, 3 
je .tobool
cmp rdi, 4 
je string_err
.todouble:
mov rdi, qword [rsp + 16] 
call ToDouble
movq qword [rsp + 8], xmm0 
mov [rsp + 16], 1
ret 
.toint:
mov rdi, qword [rsp + 16]
call ToInt
mov qword [rsp + 8], rsi
mov [rsp + 16], 0
ret 
.tochar:
mov rdi, qword [rsp + 16]
call ToChar 
mov qword [rsp + 8], rsi
mov [rsp + 16], 2
ret 
.tobool:
mov rdi, qword [rsp + 16]
call ToBool
mov qword [rsp + 8], rsi
mov [rsp + 16], 3
ret 


ToDouble:  ;rdi - datatype, rsi - value
cmp rdi, 1 
je .done 
cmp rdi, 4 
je double_cast_err
cvtsi2sd xmm0, rsi 
ret 
.done:
movq xmm0, rsi
ret 

ToInt:
cmp rdi, 4 
je int_cast_err
cmp rdi, 1 
je .fromDouble
ret
.fromDouble:
ucomisd xmm0, [rel int64_min]
jb out_of_boundary_cast_err
ucomisd xmm0, [rel int64_max]
jae out_of_boundary_cast_err
movq xmm0, rsi
cvttsd2si rsi, xmm0 
ret 


ToChar:
cmp rdi,4 
je char_cast_err
cmp rdi, 1 
jne .boundarycheck
ucomisd xmm0, [rel int64_min]
jb out_of_boundary_cast_err
ucomisd xmm0, [rel int64_max]
jae out_of_boundary_cast_err
movq xmm0, rsi
cvttsd2si rsi, xmm0 
.boundarycheck:
mov rax, 0xFFFFFFFF
cmp rsi, rax  
jg out_of_boundary_cast_err 
xor rax, rax 
cmp rsi, rax
jl out_of_boundary_cast_err
ret

ToBool:
cmp rdi, 4 
je bool_cast_err
cmp rdi, 1 
je .checkDouble 
cmp rsi, 0 
setne al
movzx esi, al
ret 
.checkDouble:
shl rsi, 1 
setnz al 
movzx esi, al 
ret 



UnaryNeg:
cmp [rsp + 16], 0 
je .fromInt 
cmp [rsp + 16], 1 
je .fromDouble 
jmp neg_operator_err

.fromInt:
mov rax, [rsp + 8] 
neg rax 
jo out_of_boundary_cast_err
mov [rsp + 8], rax 
ret 
.fromDouble: 
mov rax, [rsp + 8]
btc rax, 63 ;changing the first bit as in double it stands for the sign
mov [rsp + 8], rax  
ret 

UnaryNot:
mov rdi, [rsp + 16] 
cmp rdi, 4 
je string_err 
mov rsi, [rsp + 8]
call ToDouble 
test rsi, rsi 
sete al 
movzx esi, al 
mov [rsp + 8], rsi 
mov [rsp + 16], 3 
ret



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
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
addsd xmm0, xmm1 
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
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
subsd xmm0, xmm1
movq qword [rsp + 8], xmm0 
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
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
mulsd xmm0, xmm1
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
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
xorps xmm0, xmm0
ucomisd xmm1, xmm0
je div_zero_err
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
divsd xmm0, xmm1
movq qword [rsp + 8], xmm0 
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
mov qword [rsp + 16], 3 
ret 

LessDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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
mov qword [rsp + 16], 3 
ret 

GreaterDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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
mov qword [rsp + 16], 3 
ret 

LessEQDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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
mov qword [rsp + 16], 3 
ret 

GreaterEQDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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
mov qword [rsp + 16], 3 
ret 

EqualDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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
mov qword [rsp + 16], 3 
ret 

NotEqualDouble:
mov rdi, rcx 
mov rsi, rax 
call ToDouble
movq xmm1, xmm0
mov rdi, qword [rsp + 16]
mov rsi, qword [rsp + 8]
call ToDouble
ucomisd xmm0, xmm1 
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

double_cast_err:
mov rdi, 4 
jmp runtime_error 

int_cast_err:
mov rdi, 5 
jmp runtime_error 

char_cast_err:
mov rdi, 6 
jmp runtime_error 

bool_cast_err:
mov rdi, 7 
jmp runtime_error

out_of_boundary_cast_err:
mov rdi, 8 
jmp runtime_error

neg_operator_err:
mov rdi, 9 
jmp runtime_error

runtime_error:
mov rsi, qword [rel current_instruction] 
and rsp, -16
call PrintError WRT ..plt
mov rdi, -1 
mov rax, 60
syscall 
