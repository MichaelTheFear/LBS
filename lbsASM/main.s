function1:
pushq %rbp
mov %rsp, %rbp

imul %rcx, %rax
imul %rdx, %rax
imul %rsi, %rax
imul %rdi, %rax

imul %rcx, %rcx
imul %rdx, %rcx
imul %rsi, %rcx
imul %rdi, %rcx

imul %rcx, %rdx
imul %rdx, %rdx
imul %rsi, %rdx
imul %rdi, %rdx

imul %rcx, %rsi
imul %rdx, %rsi
imul %rsi, %rsi
imul %rdi, %rsi

imul %rcx, %rdi
imul %rdx, %rdi
imul %rsi, %rdi
imul %rdi, %rdi


imul %r8, %rax
imul %r9, %rax

imul %r8, %rcx
imul %r9, %rcx

imul %r8, %rdx
imul %r9, %rdx

imul %r8, %rsi
imul %r9, %rsi

imul %r8, %rdi
imul %r9, %rdi

imul %r8, %r8
imul %r9, %r8
imul %r8, %r9
imul %r9, %r9

imul $0xabcd, %rax
imul $0xabcd, %rcx
imul $0xabcd, %rdx
imul $0xabcd, %rsi
imul $0xabcd, %rdi
imul $0xabcd, %r8
imul $0xabcd, %r9


popq %rbp
ret
