push %rbp
mov %rsp, %rbp
mov %rdi,%rsi
imul $0x80, %rsi
mov %rsi,%rax
pop %rbp
ret
