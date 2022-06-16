/*rax rcx rdx rsi rdi r8 r9*/
function1:
    pushq %rbp
    mov %rsp, %rbp
    mov $0xfff,%r12
    mov %r12,%rdi
    call function2
    mov %rdi,%rax
    popq %rbp
    ret

function2:
    pushq %rbp
    mov %rsp, %rbp
    mov $0x0,%r12
    mov %r12,%rdi
    mov $0x2,%rax
    popq %rbp
    ret


