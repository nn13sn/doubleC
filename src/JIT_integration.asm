section .text

extern ReturnInt 
extern ReturnDouble 
extern ReturnChar 
extern ReturnBool 
extern ReturnString 
extern JITPrint 
extern JITInput
extern PushFor 
extern PopFor


global ReturnValue
global asmPrint
global asmInput 
global asmForInit 
global asmForEnd


ReturnValue:

push rbp
mov rbp, rsp
and rsp, -16

;rdi - data type, rsi - value, rdx - return pointer  
cmp rdi , 0 ; 0 - int 
je .intcall 

cmp rdi, 1 ;1 - double 
je .doublecall

cmp rdi, 2 ;2 - char 
je .charcall 

cmp rdi, 3 
je .boolcall ;3 - bool 

cmp rdi, 4 ;4 - string 
je .stringcall

.intcall:
mov rdi, rdx 
call ReturnInt ;rsi still contain value
jmp .done 

.doublecall:
mov rdi, rdx  
movq xmm0, rsi 
call ReturnDouble 
jmp .done 

.charcall:
mov rdi, rdx 
call ReturnChar 
jmp .done 

.boolcall:
mov rdi, rdx 
call ReturnBool 
jmp .done 

.stringcall:
mov rdi, rdx 
call ReturnString ;value is just a pointer to std::string,  
jmp .done 

.done:
mov rsp, rbp 
pop rbp
ret 


asmPrint:  ;registers rdi and rsi already contain the required values
push rbp
mov rbp, rsp
and rsp, -16

call JITPrint 

mov rsp, rbp 
pop rbp
ret 


asmInput:  ;register rdi has the datatype
push rbp
mov rbp, rsp
and rsp, -16

call JITInput 

mov rsp, rbp 
pop rbp

mov r8, [rsp]
sub rsp, 16 

mov [rsp + 16], rax 
mov[rsp + 8], rdx 
mov [rsp], r8

ret



asmForInit:
push rbp
mov rbp, rsp
and rsp, -16

call PushFor 

mov qword [r12 + 24], rax 

mov rsp, rbp 
pop rbp
ret 


asmForEnd:
push rbp
mov rbp, rsp
and rsp, -16

call PopFor 

mov qword [r12 + 24], rax

mov rsp, rbp 
pop rbp
ret 
