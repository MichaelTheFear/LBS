function1:

pushq %rbp
movq %rsp, %rbp


imul $0xFFFF, %rax
imul $0xFFFF, %rcx
imul $0xFFFF, %rdx
imul $0xFFFF, %rsi
imul $0xFFFF, %rdi
imul $0xFFFF, %r8
imul $0xFFFF, %r9

popq %rbp
ret	
