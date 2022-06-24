/*rax r12 rdx rsi rdi r8 r9*/


/*
int boo(int d,int n){
    int i;
    int v[n];

    v[0] = d;
    for(i=1;i<n;i++){
        v[i] = v[i-1]*d;
    }
    return v[n-1];
}
*/

    pushq %rbp
    movq %rsp,%rbp
    subq $48,%rsp
    movq %rdi,-48(%rbp)
    movq $0xFFF, -40(%rbp)
    movq -40(%rbp),%r12
    addq -40(%rbp),%r12

    movq -32(%rbp),%rax
    leave
    ret    





/*
function1:
pushq %rbp
movq %rsp,%rbp
subq $40,%rsp

call function2

movq %rax,-8(%rbp)
movq $16,-16(%rbp)
movq $17,-24(%rbp)
movq $1,-32(%rbp)
movq $1,-40(%rbp)

movq -8(%rbp),%r12
imul -16(%rbp),%r12
movq %r12,-16(%rbp)

movq -16(%rbp),%r12
imul -24(%rbp),%r12
movq %r12,-24(%rbp)

movq -24(%rbp),%r12
imul -32(%rbp),%r12
movq %r12,-32(%rbp)

movq -32(%rbp),%r12
imul -40(%rbp),%r12
movq %r12,-40(%rbp)

movq -40(%rbp),%rax
leave
ret


function2:
pushq %rbp
movq %rsp,%rbp
subq $40,%rsp

movq $11,-8(%rbp)
movq $12,-16(%rbp)
movq $13,-24(%rbp)
movq $14,-32(%rbp)
movq $15,-40(%rbp)

movq -8(%rbp),%r12
imul -16(%rbp),%r12
movq %r12,-16(%rbp)

movq -16(%rbp),%r12
imul -24(%rbp),%r12
movq %r12,-24(%rbp)

movq -24(%rbp),%r12
imul -32(%rbp),%r12
movq %r12,-32(%rbp)

movq -32(%rbp),%r12
imul -40(%rbp),%r12
movq %r12,-40(%rbp)

movq -40(%rbp),%rax
leave
ret


*/