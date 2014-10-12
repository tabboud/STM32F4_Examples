@;Place Macros in this file

.macro SET_bit addr, bit		@;Set bit to 1 of [addr](which is given by addr)
	ldr r2, =\addr				@;Move address value into r2: 0x40023830
	ldr r1, [r2]				@;Load the value of addr into r1
	orr r1, r1, #1<<\bit		@;Set the bit by shifting a 1 to the position
	str r1, [r2]				@;Store back value
.endm

@;****Definitions for GPIO Port Initializations****
.equ MODER,0x00				@;__IO uint32_t MODER;    /*!< GPIO port mode register,              
.equ OTYPER,0x04			@;__IO uint32_t OTYPER;   /*!< GPIO port output type register,       
.equ OSPEEDR,0x08			@;__IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,      
.equ PUPDR,0x0C				@;__IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register, 
.equ IDR,0x10				@;__IO uint32_t IDR;      /*!< GPIO port input data register,        
.equ ODR,0x14				@;__IO uint32_t ODR;      /*!< GPIO port output data register,       
.equ BSRRL,0x18				@;__IO uint16_t BSRRL;    /*!< GPIO port bit set/reset low register, 
.equ BSRRH,0x1A				@;__IO uint16_t BSRRH;    /*!< GPIO port bit set/reset high register,
.equ LCKR,0x1C				@;__IO uint32_t LCKR;     /*!< GPIO port configuration lock register,
.equ AFR1,0x20				@;__IO uint32_t AFR[2];   /*!< GPIO alternate function registers,    
.equ AFR2,0x24				@;__IO uint32_t AFR[2];   /*!< GPIO alternate function registers,    
@;STD_OUTPIN 	 ==	0
@;STD_INPIN 	 ==	1
@;PULLUP_INPIN 	 == 2
.macro PORTBIT_init mode, base, pin		
	.ifeq \mode-0						@;mode == STD_OUTPIN
		ldr r2, =\base					@;Load in the GPIOx_BASE
		ldr r1, [r2, #MODER]			@;Load contents of base into R1
		bic r1, r1, #3<<(\pin<<1)		@;Clear specified bits 
		str r1, [r2, #MODER]			@;GPIOx->MODER (32)
		ldr r1, [r2, #MODER]			@;
		orr r1, r1, #1<<(\pin<<1)		@;Set the mode == OUTPUT
		str r1, [r2, #MODER] 
	
		ldr r1, [r2, #OTYPER]			@;GPIOx->OTYPER: (16)	
		bic r1, r1, #1<<\pin			@;set pin to 0 == output Push-Pull(reset state)
		str r1, [r2, #OTYPER]			
		
		ldr r1, [r2, #OSPEEDR]			@;GPIOx->OSPEEDR: (32) 
		bic r1, r1, #3<<(\pin<<1)		@;clear bits
		str r1, [r2, #OSPEEDR]
		ldr r1, [r2, #OSPEEDR]			
		orr r1,(2 << (2*\pin))			@;Set speed to 50Mhz (fast speed)
		str r1, [r2, #OSPEEDR]
			
		ldr r1, [r2, #PUPDR]			@;GPIOx->PUPDR: (32)
		bic r1, r1, #3<<(\pin<<1)		@;Clear bits
		str r1, [r2, #PUPDR]
		ldr r1, [r2, #PUPDR]
		orr r1, r1, #1<<(\pin<<1)		@;set to 01 == Pull up
		str r1, [r2, #PUPDR]			@;end PUPDR 
		.else
			.ifeq \mode-1						@;mode == STD_INPIN
				ldr r2, =\base					@;Load in the GPIOx_BASE
				ldr r1, [r2, #MODER]			
				bic r1, r1, #3<<(\pin<<1)		@;Clear bits: Sets to Input
				str r1, [r2, #MODER]
				
				ldr r1, [r2, #OSPEEDR]			@;OSPEEDR   
				bic r1, r1, #3<<(\pin<<1)		
				str r1, [r2, #OSPEEDR]
				ldr r1, [r2, #OSPEEDR]			
				orr r1,(2 << (2*\pin))			@;Set speed to 50Mhz (fast speed)
				str r1, [r2, #OSPEEDR]				
				
				ldr r1, [r2, #PUPDR]			@;PUPDR
				bic r1, r1, #3<<(\pin<<1)		@;Clear: No Pull Up
				str r1, [r2, #PUPDR]			
				.else
					.ifeq \mode-2					@;mode == PULLUP_INPIN
						ldr r2, =\base				@;Load in the GPIOx_BASE
						ldr r1, [r2, #MODER]			
						bic r1, r1, #3<<(\pin<<1)	@;Clear bits: Sets to Input
						str r1, [r2, #MODER]
						
						ldr r1, [r2, #OSPEEDR]			@;OSPEEDR   
						bic r1, r1, #3<<(\pin<<1)		
						str r1, [r2, #OSPEEDR]
						ldr r1, [r2, #OSPEEDR]			
						orr r1,(2 << (2*\pin))			@;Set speed to 50Mhz (fast speed)
						str r1, [r2, #OSPEEDR]
						
						ldr r1, [r2, #PUPDR]
						bic r1, r1, #3<<(\pin<<1)	@;Clear bits: no pull-up
						str r1, [r2, #PUPDR]
						ldr r1, [r2, #PUPDR]
						orr r1, r1, #1<<(\pin<<1)	@;Set bits: Enable Pull-Up
						str r1, [r2, #PUPDR]		
					.endif
			.endif
	.endif
.endm


.macro PORTBIT_write base, pin, val
	ldr r2, =\base				@;Load in the GPIOx_BASE address
	.if (\val == 1)			
		mov r1, #1<<\pin		@;Write to lower 16 bits of BSSR to set the bit
	.else	
		mov r1, #1<<(\pin +16)	@;Write to upper 16 bits of BSSR to clear the bit
	.endif	
	str r1, [r2, #24]			@;Store into the BSRR
.endm


.macro PORTBIT_read base, pin
	ldr r2, =\base
	ldr r0, [r2, #IDR]		@;Get GPIOx->IDR
	ands r0, r0, #1<<\pin	@;Get just the pin value
	mov r0, r0, lsr #\pin	@;Move pin value 0 or 1 into R0 to return
.endm




