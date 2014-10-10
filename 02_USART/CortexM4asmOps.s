@; --- characterize target syntax, processor
	.syntax unified				@; ARM Unified Assembler Language (UAL). 
	.thumb						@; Use thumb instructions only

	.data						@; start the _initialized_ RAM data section
								@; global? initialized? variables
	
	.bss						@;start an uninitialized RAM data section
	.align						@;pad memory if necessary to align on a word boundary for word storage 


@; ***** Macros ******* 

@; .include "stm32f4_P24_definitions.s"

.macro MOV_imm32 reg val		@;example of use: MOV_imm32 r0,0x12345678 !!note: no '#' on immediate value
	movw \reg,#(0xFFFF & (\val))
	movt \reg,#((0xFFFF0000 & (\val))>>16)
.endm


	.text						@;start the code section
	.align
@; ******* Write your assembly code here ******* @


@;Example to turn on and off an LED 	
	.global turnOnLED	@; R0 -> GPIOx_BASE Address
	.thumb_func			@; R1 -> Pin to set
turnOnLED:
	mov r2, #1
	mov r2, r2, LSL r1 		@; Set a 1 at the pin number (1 << \pin)
	str r2, [r0, #24]		@; Set BSRRL to set LED on
bx lr

	
	.global turnOffLED			@; R0 -> GPIOx_BASE Address
	.thumb_func				@; R1 -> Pin to set
turnOffLED:
	mov r2, #1<<16			@; Shift to upper 16 bits to set for BSRRH
	mov r2, r2, LSL r1 		@; Set a 1 at the pin number (1 << \pin)
	str r2, [r0, #24]		@; Set BSRRH to set LED off
bx lr








