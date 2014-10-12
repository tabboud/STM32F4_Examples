/*
External Interrupt (EXTI) Example
    - Modified blinky with pushbutton code
        to use EXTI instead of Polling
*/
#include <stdio.h>
#include "stm32f4xx.h"
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

void initialize() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    // ---------- GPIO  for LED (PD13) -------- //
    // GPIOD Periph clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    // Configure PD13
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    
    // ---------- GPIO  for User Button (PA0) -------- //
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Enable GPIOA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Configure PA0 pin as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		// PA0 is connected to high, so use pulldown resistor
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Connect EXTI Line0 to PA0 pin (i.e. EXTI0CR[0])*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    // SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI0_PA;		// Same as above, but with direct register access
    
    /* Configure EXTI Line0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;              // PA0 is connected to EXTI0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     // Interrupt mode
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	// Trigger on Rising edge (Just as user presses btn)
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // Enable the interrupt
    EXTI_Init(&EXTI_InitStructure);                         // Initialize EXTI
    
    /* Enable and set priorities for the EXTI0 in NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;                // Function name for EXTI_Line0 interrupt handler
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;    // Set priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;           // Set sub priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 // Enable the interrupt
    NVIC_Init(&NVIC_InitStructure);                                 // Add to NVIC
}


void EXTI0_IRQHandler(void) {
    // Make sure the interrupt flag is set for EXTI0
    if(EXTI_GetITStatus(EXTI_Line0) != RESET){
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        
        // Clear the interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

int main(void) {
    setSysTick();
    initialize();
    
    // Turn on LED
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    
    while(1){
        // Do nothing here, using interrupts
    }
    
    return 0;
}


