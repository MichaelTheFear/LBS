	.file	"lbs_funcs.c"
	.text
	.globl	return_constant
	.type	return_constant, @function
return_constant:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
	.size	return_constant, .-return_constant
	.globl	inc_constant
	.type	inc_constant, @function
inc_constant:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	popq	%rbp
	ret
	.size	inc_constant, .-inc_constant
	.globl	calculate_factorial
	.type	calculate_factorial, @function
calculate_factorial:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L6
	movl	$1, %eax
	jmp	.L7
.L6:
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	calculate_factorial
	imull	-4(%rbp), %eax
.L7:
	leave
	ret
	.size	calculate_factorial, .-calculate_factorial
	.globl	number_square
	.type	number_square, @function
number_square:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	imull	%eax, %eax
	popq	%rbp
	ret
	.size	number_square, .-number_square
	.globl	sum_square
	.type	sum_square, @function
sum_square:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$24, %rsp
	movl	%edi, -20(%rbp)
	movl	$0, -8(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L11
.L12:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	number_square
	addl	%eax, -8(%rbp)
	addl	$1, -4(%rbp)
.L11:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jle	.L12
	movl	-8(%rbp), %eax
	leave
	ret
	.size	sum_square, .-sum_square
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
