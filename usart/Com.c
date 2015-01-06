#include "..\APP\includes.h"
//#include "include.h"
/**************************************************************************************
* Variable Prototype:
* Variable Description:
************************************************** ************************************ /
/ ************************************************* *************************************
* Prototype:
* Function:
* Input parameters:
* Output parameters:
* Function Description:
**************************************************************************************/

#ifdef __GNUC__ 
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf 
     set to 'Yes') calls __io_putchar() */ 
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch) 
#else 
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f) 
#endif /* __GNUC__ */ 

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
//{
//  /* Write a character to the USART */
//  USART_SendData(USART1, (u8) ch);
//
//  /* Loop until the end of transmission */
//  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {
//  }
//    return ch;
//}
{
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (ch & 0x1FF);
   
   return ch;
}

//void sendchar (int ch)
//{
//    while (!(USART1->SR & USART_FLAG_TXE));
//    USART1->DR = (ch & 0x1FF);
//}
//
//void myprintf(char *buf)
//{
//   while(*buf!='\0')
//   {
//      sendchar(*buf);
//      buf++;
//   }
//}

void USART1_InitConfig(uint32 BaudRate)
{USART_InitTypeDef USART_InitStructure;
  
  //USART1->SR &= ~USART_FLAG_TXE;     // clear interrupt
  //USART1->SR &= ~USART_FLAG_TC;     // clear interrupt
  
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE); // Send only open
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE); 
  /* See the implementation of the simulation here, TC flag is set to 1 actually, 
  I do not know whether the actual operation is in flash */
  
//  USART1->SR &= ~USART_FLAG_TXE;     // clear interrupt
//  USART1->SR &= ~USART_FLAG_TC;     // clear interrupt
  

}
