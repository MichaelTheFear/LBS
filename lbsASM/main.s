/*rax r12 rdx rsi rdi r8 r9*/
function1:
pushq %rbp
movq %rsp,%rbp
subq $16,%rsp
movq %rbx,-8(%rbp)
movq %r12,-16(%rbp)
movq $12,%r12
call function2
movq %r12,%rax
movq -8(%rbp),%rbx
movq -16(%rbp),%r12
leave


function2:
pushq %rbp
movq %rsp,%rbp
subq $16,%rsp
movq $15,%r12
movq %r12,%rax
movq -8(%rbp),%rbx
movq -16(%rbp),%r12
leave



