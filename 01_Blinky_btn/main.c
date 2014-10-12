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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


		// ---------- GPIO  for Push Button -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Configure PD12, PD13, PD14 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	//PA0 is connected to high, so use pull down resistor
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


int main(void) {
	setSysTick();
	init_GPIO();


while(1){
		//if(GPIOA->IDR & 0x01)
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		
		Delay(200);
	}



	return 0;
}



