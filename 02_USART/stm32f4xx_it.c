#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
__attribute__((weak)) void SysTick_Handler(void)
{

}


#define MAX_WORDLEN 10
volatile char received_str[MAX_WORDLEN + 1];
extern void USART_puts(USART_TypeDef *USARTx, volatile char *str);

// USART2 Interrupt request handler for ALL (Tx/Rx) USART2 interrupts
void USART2_IRQHandler(void){

  // Check the Interrupt status to ensure the Rx interrupt was triggered, not Tx
  if( USART_GetITStatus(USART2, USART_IT_RXNE)){
    static int cnt = 0;
    // Get the byte that was transferred
    char ch = USART2->DR;

    // Check for "Enter" key, or Maximum characters
    if((ch != '\n') && (cnt < MAX_WORDLEN)){
      received_str[cnt++] = ch;
    }
    else{
      received_str[cnt] = '\0';
      cnt = 0;
      USART_puts(USART2, received_str);
    }
  }

}











