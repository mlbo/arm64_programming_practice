	.arch armv8-a
	.file	"asm-offsets.c"
// GNU C17 (Ubuntu 13.3.0-6ubuntu2~24.04) version 13.3.0 (aarch64-linux-gnu)
//	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed: -mgeneral-regs-only -mlittle-endian -mabi=lp64 -g -g -ffreestanding -fomit-frame-pointer -fasynchronous-unwind-tables -fstack-clash-protection
	.text
.Ltext0:
	.file 0 "/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos" "arch/arm64/kernel/asm-offsets.c"
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.file 1 "arch/arm64/kernel/asm-offsets.c"
	.loc 1 26 1
	.cfi_startproc
// arch/arm64/kernel/asm-offsets.c:27: 	DEFINE(S_FRAME_SIZE, sizeof(struct pt_regs));
	.loc 1 27 2
#APP
// 27 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_FRAME_SIZE 320 sizeof(struct pt_regs)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:28: 	DEFINE(S_X0,	offsetof(struct pt_regs, regs[0]));
	.loc 1 28 2
// 28 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X0 0 offsetof(struct pt_regs, regs[0])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:29: 	DEFINE(S_X1,	offsetof(struct pt_regs, regs[1]));
	.loc 1 29 2
// 29 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X1 8 offsetof(struct pt_regs, regs[1])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:30: 	DEFINE(S_X2,	offsetof(struct pt_regs, regs[2]));
	.loc 1 30 2
// 30 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X2 16 offsetof(struct pt_regs, regs[2])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:31: 	DEFINE(S_X3,	offsetof(struct pt_regs, regs[3]));
	.loc 1 31 2
// 31 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X3 24 offsetof(struct pt_regs, regs[3])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:32: 	DEFINE(S_X4,	offsetof(struct pt_regs, regs[4]));
	.loc 1 32 2
// 32 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X4 32 offsetof(struct pt_regs, regs[4])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:33: 	DEFINE(S_X5,	offsetof(struct pt_regs, regs[5]));
	.loc 1 33 2
// 33 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X5 40 offsetof(struct pt_regs, regs[5])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:34: 	DEFINE(S_X6,	offsetof(struct pt_regs, regs[6]));
	.loc 1 34 2
// 34 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X6 48 offsetof(struct pt_regs, regs[6])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:35: 	DEFINE(S_X7,	offsetof(struct pt_regs, regs[7]));
	.loc 1 35 2
// 35 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X7 56 offsetof(struct pt_regs, regs[7])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:36: 	DEFINE(S_X8,	offsetof(struct pt_regs, regs[8]));
	.loc 1 36 2
// 36 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X8 64 offsetof(struct pt_regs, regs[8])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:37: 	DEFINE(S_X10,	offsetof(struct pt_regs, regs[10]));
	.loc 1 37 2
// 37 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X10 80 offsetof(struct pt_regs, regs[10])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:38: 	DEFINE(S_X12,	offsetof(struct pt_regs, regs[12]));
	.loc 1 38 2
// 38 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X12 96 offsetof(struct pt_regs, regs[12])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:39: 	DEFINE(S_X14,	offsetof(struct pt_regs, regs[14]));
	.loc 1 39 2
// 39 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X14 112 offsetof(struct pt_regs, regs[14])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:40: 	DEFINE(S_X16,	offsetof(struct pt_regs, regs[16]));
	.loc 1 40 2
// 40 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X16 128 offsetof(struct pt_regs, regs[16])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:41: 	DEFINE(S_X18,	offsetof(struct pt_regs, regs[18]));
	.loc 1 41 2
// 41 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X18 144 offsetof(struct pt_regs, regs[18])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:42: 	DEFINE(S_X20,	offsetof(struct pt_regs, regs[20]));
	.loc 1 42 2
// 42 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X20 160 offsetof(struct pt_regs, regs[20])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:43: 	DEFINE(S_X22,	offsetof(struct pt_regs, regs[22]));
	.loc 1 43 2
// 43 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X22 176 offsetof(struct pt_regs, regs[22])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:44: 	DEFINE(S_X24,	offsetof(struct pt_regs, regs[24]));
	.loc 1 44 2
// 44 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X24 192 offsetof(struct pt_regs, regs[24])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:45: 	DEFINE(S_X26,	offsetof(struct pt_regs, regs[26]));
	.loc 1 45 2
// 45 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X26 208 offsetof(struct pt_regs, regs[26])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:46: 	DEFINE(S_X28,	offsetof(struct pt_regs, regs[28]));
	.loc 1 46 2
// 46 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_X28 224 offsetof(struct pt_regs, regs[28])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:47: 	DEFINE(S_FP,	offsetof(struct pt_regs, regs[29]));
	.loc 1 47 2
// 47 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_FP 232 offsetof(struct pt_regs, regs[29])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:48: 	DEFINE(S_LR,	offsetof(struct pt_regs, regs[30]));
	.loc 1 48 2
// 48 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_LR 240 offsetof(struct pt_regs, regs[30])	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:49: 	DEFINE(S_SP,	offsetof(struct pt_regs, sp));
	.loc 1 49 2
// 49 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_SP 248 offsetof(struct pt_regs, sp)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:50: 	DEFINE(S_PSTATE, offsetof(struct pt_regs, pstate));
	.loc 1 50 2
// 50 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_PSTATE 264 offsetof(struct pt_regs, pstate)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:51: 	DEFINE(S_PC,	 offsetof(struct pt_regs, pc));
	.loc 1 51 2
// 51 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_PC 256 offsetof(struct pt_regs, pc)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:52: 	DEFINE(S_ORIG_X0, offsetof(struct pt_regs, orig_x0));
	.loc 1 52 2
// 52 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_ORIG_X0 272 offsetof(struct pt_regs, orig_x0)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:53: 	DEFINE(S_SYSCALLNO, offsetof(struct pt_regs, syscallno));
	.loc 1 53 2
// 53 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_SYSCALLNO 280 offsetof(struct pt_regs, syscallno)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:54: 	DEFINE(S_ORIG_ADDR_LIMIT, offsetof(struct pt_regs, orig_addr_limit));
	.loc 1 54 2
// 54 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_ORIG_ADDR_LIMIT 288 offsetof(struct pt_regs, orig_addr_limit)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:55: 	DEFINE(S_STACKFRAME, offsetof(struct pt_regs, stackframe));
	.loc 1 55 2
// 55 "arch/arm64/kernel/asm-offsets.c" 1
	
->S_STACKFRAME 304 offsetof(struct pt_regs, stackframe)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:57: 	DEFINE(THREAD_CPU_CONTEXT, offsetof(struct task_struct, cpu_context));
	.loc 1 57 2
// 57 "arch/arm64/kernel/asm-offsets.c" 1
	
->THREAD_CPU_CONTEXT 24 offsetof(struct task_struct, cpu_context)	//
// 0 "" 2
// arch/arm64/kernel/asm-offsets.c:58: 	return 0;
	.loc 1 58 9
#NO_APP
	mov	w0, 0	// _1,
// arch/arm64/kernel/asm-offsets.c:59: }
	.loc 1 59 1
	ret	
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.file 2 "include/type.h"
	.file 3 "include/asm/system.h"
	.file 4 "include/asm/processor.h"
	.file 5 "include/sched.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x2b6
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0xb
	.4byte	.LASF34
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0xc
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x6
	.string	"u32"
	.byte	0x23
	.byte	0x16
	.4byte	0x5c
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x6
	.string	"u64"
	.byte	0x26
	.byte	0x1c
	.4byte	0x75
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0xd
	.2byte	0x110
	.byte	0x3
	.byte	0x10
	.byte	0x2
	.4byte	0xba
	.uleb128 0x4
	.4byte	.LASF9
	.byte	0x3
	.byte	0x11
	.byte	0x7
	.4byte	0xba
	.byte	0
	.uleb128 0x1
	.string	"sp"
	.byte	0x3
	.byte	0x12
	.byte	0x7
	.4byte	0x6a
	.byte	0xf8
	.uleb128 0xe
	.string	"pc"
	.byte	0x3
	.byte	0x13
	.byte	0x7
	.4byte	0x6a
	.2byte	0x100
	.uleb128 0x3
	.4byte	.LASF10
	.byte	0x14
	.byte	0x7
	.4byte	0x6a
	.2byte	0x108
	.byte	0
	.uleb128 0x7
	.4byte	0x6a
	.4byte	0xca
	.uleb128 0x8
	.4byte	0xca
	.byte	0x1e
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0xf
	.4byte	.LASF18
	.2byte	0x140
	.byte	0x3
	.byte	0xf
	.byte	0x8
	.4byte	0x134
	.uleb128 0x10
	.4byte	0x7c
	.byte	0
	.uleb128 0x3
	.4byte	.LASF12
	.byte	0x17
	.byte	0x6
	.4byte	0x6a
	.2byte	0x110
	.uleb128 0x3
	.4byte	.LASF13
	.byte	0x18
	.byte	0x6
	.4byte	0x51
	.2byte	0x118
	.uleb128 0x3
	.4byte	.LASF14
	.byte	0x19
	.byte	0x6
	.4byte	0x51
	.2byte	0x11c
	.uleb128 0x3
	.4byte	.LASF15
	.byte	0x1b
	.byte	0x6
	.4byte	0x6a
	.2byte	0x120
	.uleb128 0x3
	.4byte	.LASF16
	.byte	0x1c
	.byte	0x6
	.4byte	0x6a
	.2byte	0x128
	.uleb128 0x3
	.4byte	.LASF17
	.byte	0x1d
	.byte	0x6
	.4byte	0x134
	.2byte	0x130
	.byte	0
	.uleb128 0x7
	.4byte	0x6a
	.4byte	0x144
	.uleb128 0x8
	.4byte	0xca
	.byte	0x1
	.byte	0
	.uleb128 0x9
	.4byte	.LASF19
	.byte	0x68
	.byte	0x4
	.byte	0x1
	.4byte	0x1f7
	.uleb128 0x1
	.string	"x19"
	.byte	0x4
	.byte	0x2
	.byte	0x10
	.4byte	0xca
	.byte	0
	.uleb128 0x1
	.string	"x20"
	.byte	0x4
	.byte	0x3
	.byte	0x10
	.4byte	0xca
	.byte	0x8
	.uleb128 0x1
	.string	"x21"
	.byte	0x4
	.byte	0x4
	.byte	0x10
	.4byte	0xca
	.byte	0x10
	.uleb128 0x1
	.string	"x22"
	.byte	0x4
	.byte	0x5
	.byte	0x10
	.4byte	0xca
	.byte	0x18
	.uleb128 0x1
	.string	"x23"
	.byte	0x4
	.byte	0x6
	.byte	0x10
	.4byte	0xca
	.byte	0x20
	.uleb128 0x1
	.string	"x24"
	.byte	0x4
	.byte	0x7
	.byte	0x10
	.4byte	0xca
	.byte	0x28
	.uleb128 0x1
	.string	"x25"
	.byte	0x4
	.byte	0x8
	.byte	0x10
	.4byte	0xca
	.byte	0x30
	.uleb128 0x1
	.string	"x26"
	.byte	0x4
	.byte	0x9
	.byte	0x10
	.4byte	0xca
	.byte	0x38
	.uleb128 0x1
	.string	"x27"
	.byte	0x4
	.byte	0xa
	.byte	0x10
	.4byte	0xca
	.byte	0x40
	.uleb128 0x1
	.string	"x28"
	.byte	0x4
	.byte	0xb
	.byte	0x10
	.4byte	0xca
	.byte	0x48
	.uleb128 0x1
	.string	"fp"
	.byte	0x4
	.byte	0xc
	.byte	0x10
	.4byte	0xca
	.byte	0x50
	.uleb128 0x1
	.string	"sp"
	.byte	0x4
	.byte	0xd
	.byte	0x10
	.4byte	0xca
	.byte	0x58
	.uleb128 0x1
	.string	"pc"
	.byte	0x4
	.byte	0xe
	.byte	0x10
	.4byte	0xca
	.byte	0x60
	.byte	0
	.uleb128 0xa
	.4byte	.LASF25
	.4byte	0x5c
	.byte	0xe
	.4byte	0x224
	.uleb128 0x5
	.4byte	.LASF20
	.byte	0
	.uleb128 0x5
	.4byte	.LASF21
	.byte	0x1
	.uleb128 0x5
	.4byte	.LASF22
	.byte	0x2
	.uleb128 0x5
	.4byte	.LASF23
	.byte	0x3
	.uleb128 0x5
	.4byte	.LASF24
	.byte	0x4
	.byte	0
	.uleb128 0xa
	.4byte	.LASF26
	.4byte	0x5c
	.byte	0x16
	.4byte	0x239
	.uleb128 0x5
	.4byte	.LASF27
	.byte	0x1
	.byte	0
	.uleb128 0x9
	.4byte	.LASF28
	.byte	0x80
	.byte	0x5
	.byte	0x1b
	.4byte	0x294
	.uleb128 0x4
	.4byte	.LASF29
	.byte	0x5
	.byte	0x1c
	.byte	0x12
	.4byte	0x1f7
	.byte	0
	.uleb128 0x4
	.4byte	.LASF30
	.byte	0x5
	.byte	0x1d
	.byte	0x12
	.4byte	0x224
	.byte	0x4
	.uleb128 0x4
	.4byte	.LASF31
	.byte	0x5
	.byte	0x1e
	.byte	0x7
	.4byte	0x294
	.byte	0x8
	.uleb128 0x4
	.4byte	.LASF32
	.byte	0x5
	.byte	0x1f
	.byte	0x6
	.4byte	0x4a
	.byte	0x10
	.uleb128 0x1
	.string	"pid"
	.byte	0x5
	.byte	0x20
	.byte	0x6
	.4byte	0x4a
	.byte	0x14
	.uleb128 0x4
	.4byte	.LASF19
	.byte	0x5
	.byte	0x21
	.byte	0x15
	.4byte	0x144
	.byte	0x18
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF33
	.uleb128 0x11
	.4byte	.LASF35
	.byte	0x1
	.byte	0x19
	.byte	0x5
	.4byte	0x4a
	.8byte	.LFB0
	.8byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 2
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0x21
	.sleb128 7
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 5
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.8byte	0
	.8byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF19:
	.string	"cpu_context"
.LASF12:
	.string	"orig_x0"
.LASF25:
	.string	"task_state"
.LASF22:
	.string	"TASK_UNINTERRUPTIBLE"
.LASF29:
	.string	"state"
.LASF31:
	.string	"count"
.LASF20:
	.string	"TASK_RUNNING"
.LASF28:
	.string	"task_struct"
.LASF8:
	.string	"long long unsigned int"
.LASF3:
	.string	"unsigned char"
.LASF14:
	.string	"unused2"
.LASF11:
	.string	"long unsigned int"
.LASF5:
	.string	"short unsigned int"
.LASF32:
	.string	"priority"
.LASF26:
	.string	"task_flags"
.LASF27:
	.string	"PF_KTHREAD"
.LASF35:
	.string	"main"
.LASF6:
	.string	"unsigned int"
.LASF30:
	.string	"flags"
.LASF17:
	.string	"stackframe"
.LASF2:
	.string	"char"
.LASF9:
	.string	"regs"
.LASF21:
	.string	"TASK_INTERRUPTIBLE"
.LASF24:
	.string	"TASK_STOPPED"
.LASF16:
	.string	"unused"
.LASF18:
	.string	"pt_regs"
.LASF7:
	.string	"long long int"
.LASF23:
	.string	"TASK_ZOMBIE"
.LASF34:
	.string	"GNU C17 13.3.0 -mgeneral-regs-only -mlittle-endian -mabi=lp64 -g -g -ffreestanding -fomit-frame-pointer -fasynchronous-unwind-tables -fstack-clash-protection"
.LASF4:
	.string	"short int"
.LASF13:
	.string	"syscallno"
.LASF10:
	.string	"pstate"
.LASF15:
	.string	"orig_addr_limit"
.LASF33:
	.string	"long int"
	.section	.debug_line_str,"MS",@progbits,1
.LASF1:
	.string	"/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos"
.LASF0:
	.string	"arch/arm64/kernel/asm-offsets.c"
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
