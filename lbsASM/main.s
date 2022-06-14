function1:
pushq %rbp
movq %rsp, %rbp
mov %rdi, %rcx
cmp $0, %rcx
jz r0
call function2
sub $1, %rcx
sub $1, %rcx
cmp $0, %rcx
jz r1
sub $1, %rcx
mov $3, %rax
popq %rbp
ret	
r0:
mov $10, %rax
popq %rbp
ret
r1:
mov $20, %rax
popq %rbp
ret


function2:
pushq %rbp
movq %rsp, %rbp
call function3
mov %rdi, %rcx
mov $0, %rax
popq %rbp
ret


function3:
pushq %rbp
movq %rsp, %rbp
cmp $0, %rdi
jz r2
call function1
mov $9, %rax
popq %rbp
ret

r2:
mov $19, %rax
popq %rbp
ret
