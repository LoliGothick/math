	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.section	__TEXT,__literal16,16byte_literals
	.align	4
LCPI0_0:
	.long	1127219200              ## 0x43300000
	.long	1160773632              ## 0x45300000
	.long	0                       ## 0x0
	.long	0                       ## 0x0
LCPI0_1:
	.quad	4841369599423283200     ## double 4.503600e+15
	.quad	4985484787499139072     ## double 1.934281e+25
LCPI0_4:
	.quad	9223372036854775807     ## 0x7fffffffffffffff
	.quad	9223372036854775807     ## 0x7fffffffffffffff
	.section	__TEXT,__literal8,8byte_literals
	.align	3
LCPI0_2:
	.quad	4626475558128695902     ## double 20.542999999999999
LCPI0_3:
	.quad	4593527504729830064     ## double 0.123
LCPI0_5:
	.quad	-4503599627370496       ## double -Inf
LCPI0_6:
	.quad	9218868437227405312     ## double +Inf
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## BB#0:
	pushq	%rbp
Ltmp8:
	.cfi_def_cfa_offset 16
Ltmp9:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp10:
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	subq	$16, %rsp
Ltmp11:
	.cfi_offset %rbx, -32
Ltmp12:
	.cfi_offset %r14, -24
	xorps	%xmm0, %xmm0
	xorl	%eax, %eax
	movdqa	LCPI0_0(%rip), %xmm8    ## xmm8 = [1127219200,1160773632,0,0]
	movapd	LCPI0_1(%rip), %xmm9    ## xmm9 = [4.503600e+15,1.934281e+25]
	movsd	LCPI0_2(%rip), %xmm10   ## xmm10 = mem[0],zero
	movsd	LCPI0_3(%rip), %xmm4    ## xmm4 = mem[0],zero
	movsd	LCPI0_4(%rip), %xmm5    ## xmm5 = mem[0],zero
	movsd	LCPI0_5(%rip), %xmm6    ## xmm6 = mem[0],zero
	movsd	LCPI0_6(%rip), %xmm7    ## xmm7 = mem[0],zero
	.align	4, 0x90
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	movd	%rax, %xmm1
	punpckldq	%xmm8, %xmm1    ## xmm1 = xmm1[0],xmm8[0],xmm1[1],xmm8[1]
	subpd	%xmm9, %xmm1
	haddpd	%xmm1, %xmm1
	addsd	%xmm10, %xmm1
	addsd	%xmm4, %xmm1
	sqrtsd	%xmm1, %xmm2
	andpd	%xmm5, %xmm2
	cmplesd	%xmm6, %xmm1
	movapd	%xmm1, %xmm3
	andnpd	%xmm2, %xmm3
	andpd	%xmm7, %xmm1
	orpd	%xmm3, %xmm1
	addsd	%xmm0, %xmm1
	leaq	1(%rax), %rcx
	movd	%rcx, %xmm0
	punpckldq	%xmm8, %xmm0    ## xmm0 = xmm0[0],xmm8[0],xmm0[1],xmm8[1]
	subpd	%xmm9, %xmm0
	haddpd	%xmm0, %xmm0
	addsd	%xmm10, %xmm0
	addsd	%xmm4, %xmm0
	xorps	%xmm2, %xmm2
	sqrtsd	%xmm0, %xmm2
	andpd	%xmm5, %xmm2
	cmplesd	%xmm6, %xmm0
	movapd	%xmm0, %xmm3
	andnpd	%xmm2, %xmm3
	andpd	%xmm7, %xmm0
	orpd	%xmm3, %xmm0
	addsd	%xmm1, %xmm0
	addq	$2, %rax
	cmpq	$100000000, %rax        ## imm = 0x5F5E100
	jne	LBB0_1
## BB#2:
	movq	__ZNSt3__14coutE@GOTPCREL(%rip), %rdi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
	movq	%rax, %rbx
	movq	(%rbx), %rax
	movq	-24(%rax), %rsi
	addq	%rbx, %rsi
	leaq	-24(%rbp), %r14
	movq	%r14, %rdi
	callq	__ZNKSt3__18ios_base6getlocEv
Ltmp0:
	movq	__ZNSt3__15ctypeIcE2idE@GOTPCREL(%rip), %rsi
	movq	%r14, %rdi
	callq	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp1:
## BB#3:
	movq	(%rax), %rcx
	movq	56(%rcx), %rcx
Ltmp2:
	movl	$10, %esi
	movq	%rax, %rdi
	callq	*%rcx
	movb	%al, %r14b
Ltmp3:
## BB#4:                                ## %_ZNKSt3__19basic_iosIcNS_11char_traitsIcEEE5widenEc.exit
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__16localeD1Ev
	movsbl	%r14b, %esi
	movq	%rbx, %rdi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc
	movq	%rbx, %rdi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB0_5:
Ltmp4:
	movq	%rax, %rbx
Ltmp5:
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__16localeD1Ev
Ltmp6:
## BB#6:
	movq	%rbx, %rdi
	callq	__Unwind_Resume
LBB0_7:
Ltmp7:
	movq	%rax, %rdi
	callq	___clang_call_terminate
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.align	2
GCC_except_table0:
Lexception0:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.byte	73                      ## @TType base offset
	.byte	3                       ## Call site Encoding = udata4
	.byte	65                      ## Call site table length
Lset0 = Lfunc_begin0-Lfunc_begin0       ## >> Call Site 1 <<
	.long	Lset0
Lset1 = Ltmp0-Lfunc_begin0              ##   Call between Lfunc_begin0 and Ltmp0
	.long	Lset1
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lset2 = Ltmp0-Lfunc_begin0              ## >> Call Site 2 <<
	.long	Lset2
Lset3 = Ltmp3-Ltmp0                     ##   Call between Ltmp0 and Ltmp3
	.long	Lset3
Lset4 = Ltmp4-Lfunc_begin0              ##     jumps to Ltmp4
	.long	Lset4
	.byte	0                       ##   On action: cleanup
Lset5 = Ltmp3-Lfunc_begin0              ## >> Call Site 3 <<
	.long	Lset5
Lset6 = Ltmp5-Ltmp3                     ##   Call between Ltmp3 and Ltmp5
	.long	Lset6
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lset7 = Ltmp5-Lfunc_begin0              ## >> Call Site 4 <<
	.long	Lset7
Lset8 = Ltmp6-Ltmp5                     ##   Call between Ltmp5 and Ltmp6
	.long	Lset8
Lset9 = Ltmp7-Lfunc_begin0              ##     jumps to Ltmp7
	.long	Lset9
	.byte	1                       ##   On action: 1
Lset10 = Ltmp6-Lfunc_begin0             ## >> Call Site 5 <<
	.long	Lset10
Lset11 = Lfunc_end0-Ltmp6               ##   Call between Ltmp6 and Lfunc_end0
	.long	Lset11
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.byte	1                       ## >> Action Record 1 <<
                                        ##   Catch TypeInfo 1
	.byte	0                       ##   No further actions
                                        ## >> Catch TypeInfos <<
	.long	0                       ## TypeInfo 1
	.align	2

	.section	__TEXT,__textcoal_nt,coalesced,pure_instructions
	.private_extern	___clang_call_terminate
	.globl	___clang_call_terminate
	.weak_def_can_be_hidden	___clang_call_terminate
	.align	4, 0x90
___clang_call_terminate:                ## @__clang_call_terminate
## BB#0:
	pushq	%rbp
	movq	%rsp, %rbp
	callq	___cxa_begin_catch
	callq	__ZSt9terminatev


.subsections_via_symbols
