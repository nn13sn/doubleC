section .text

extern ReturnInt 
extern ReturnDouble 
extern ReturnChar 
extern ReturnBool 
extern ReturnString 


global ReturnValue 

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
