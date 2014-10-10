/*
Barebones Blinky example MacOSX using arm-none-eabi and ST_Link

This is sample code to blink the user LEDs using three different methods:
1. Use the STD_Periph driver functions
2. Use the STD_Periph driver structures, NOT functions
3. Use assembly level functions defined in CortexM4asmOps.asm
*/
#include <stdio.h>
#include "stm32f4xx.h"

// ASM function prototypes (defined in CortexM4asmOps.asm)
extern void turnOnLED(uint32_t, int);
extern void turnOffLED(uint32_t, int);

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
void SysTick_Handler(void) {
	msTicks++;
}

//  Delays number of Systicks (happens every 1 ms)
static void Delay(__IO uint32_t dlyTicks){                                              
  uint32_t curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

void setSysTick(){
	// ---------- SysTick timer (1ms) -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}
}

void init_GPIO() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// ---------- GPIO  for LEDS -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


int main(void) {
	setSysTick();
	init_GPIO();

int i=0;

//Blink LED14 using Periph Driver functions
	for(i=0; i<10; i++){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		Delay(1000);	// Wait 1 second
	}

//Blink LED13 using Periph Driver structures, NOT functions
	for(i=0; i<10; i++){	
		GPIOD->BSRRL = GPIO_Pin_13;
		Delay(1000);
		GPIOD->BSRRH = GPIO_Pin_13;
		Delay(1000);
	}

// Blink LED12 using assembly functions
	while(1){
		turnOnLED(0x40020C00, 12);
		Delay(1000);
		turnOffLED(0x40020C00, 12);
		Delay(1000);
	}

	return 0;
}



