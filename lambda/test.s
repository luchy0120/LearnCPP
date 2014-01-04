	.file	"test.cpp"	
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc				# start procedure
	pushl	%ebp				# push ebp
	.cfi_def_cfa_offset 8		#
	.cfi_offset 5, -8			#
	movl	%esp, %ebp			# esp = ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp			# esp = esp - 16
	movl	$1, 8(%esp)			
	movb	$50, 15(%esp)
	leal	8(%esp), %ecx
	movl	%ecx, %eax
	movb	15(%esp), %dl
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.2 20131219 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
