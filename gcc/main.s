	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp2:
	.cfi_def_cfa_offset 16
Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp4:
	.cfi_def_cfa_register %rbp
	movl	$0, %eax
	movl	-4(%rbp), %ecx
	addl	-4(%rbp), %ecx
	movl	%ecx, -4(%rbp)
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
