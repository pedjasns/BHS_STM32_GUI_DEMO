#if !defined( _CONFIG_H )
#define _CONFIG_H

//-----------------------------------------------------------------------------------------------------
// Alias region ADDRESS = 0x4200 0000 + (0x0001 100C * 0x20) + (bitx * 4); bitx: first x bits
// The "bit address + bit number" Convert macro alias address
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
// This address is converted into a pointer
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

#define BIT_ADDR(addr, bitnum)   MEM_ADDR( BITBAND(addr, bitnum)  )

#define GPIOA_ODR    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR    (GPIOE_BASE+12) //0x4001180C

#define GPIOA_IDR    (GPIOA_BASE+8) //0x4001080C
#define GPIOB_IDR    (GPIOB_BASE+8) //0x40010C0C
#define GPIOC_IDR    (GPIOC_BASE+8) //0x4001100C
#define GPIOD_IDR    (GPIOD_BASE+8) //0x4001140C
#define GPIOE_IDR    (GPIOE_BASE+8) //0x4001180C

//#define PC13 *((volatile unsigned long  *)(0x422201B4))//((volatile unsigned long *) (0x422201B4))
#define PC13  BIT_ADDR(GPIOC_ODR, 13) // Output
#define PC12  BIT_ADDR(GPIOC_ODR, 12) // Output
#define PC11  BIT_ADDR(GPIOC_ODR, 11) // Output
#define PC10  BIT_ADDR(GPIOC_ODR, 10) // Output
#define PC9   BIT_ADDR(GPIOC_ODR, 9) // Output
#define PC8   BIT_ADDR(GPIOC_ODR, 8) // Output

#define PB10in  BIT_ADDR(GPIOB_IDR, 10) // Input
#define PB11in  BIT_ADDR(GPIOB_IDR, 11) // Input

#define PD2in   BIT_ADDR(GPIOD_IDR, 2)  // Input

#define PD6in   BIT_ADDR(GPIOD_IDR, 6)  // Input
#define PD9in   BIT_ADDR(GPIOD_IDR, 9)  // Input

//-----------------------------------------------------------------------------------------------------

//#define  myGPIO_SetBits(PORT, Bit)     SetBit32(PORT->ODR, Bit)
//#define  myGPIO_ResetBits(PORT, Bit)   ClrBit32(PORT->ODR, Bit)

//#define  myGPIO_SetBits(PORT, Bit)     GPIO_SetBits(PORT, BIT16(Bit))
//#define  myGPIO_ResetBits(PORT, Bit)   GPIO_ResetBits(PORT, BIT16(Bit))

#define  myGPIO_SetBits(PORT, Bit)     ( PORT->BSRR=BIT16(Bit) )
#define  myGPIO_ResetBits(PORT, Bit)   ( PORT->BRR=BIT16(Bit) )

// 485 Communication configuration
/******************* definition and PC 485 ****************/
//#define PC_RS485
#define PC_RS485CAN
/******************* 485 Communication with the controller definition ****************/
#define RS485

// Hardware function
#define  RCC_APB2Periph_PC_RS485DIR  RCC_APB2Periph_GPIOB
#define  PC_RS485DIR_PORT    GPIOB
#define  PC_RS485DIR_PIN     0

//#ifdef PC_RS485
#if defined(PC_RS485)
  #define PC_RS485Send_Enable()     myGPIO_SetBits(PC_RS485DIR_PORT, PC_RS485DIR_PIN)
  #define PC_RS485Receive_Enable()  myGPIO_ResetBits(PC_RS485DIR_PORT, PC_RS485DIR_PIN)

#elif defined(PC_RS485CAN)
  #define PC_RS485Send_Enable () // (USART1-> CR1 & = ~ USART_FLAG_RXNE) // close the serial port a receive interrupt
  #define PC_RS485Receive_Enable () // (USART1-> CR1 | = USART_FLAG_RXNE) // open a serial port receive interrupt
   /* If you are using CAN chip, by closing reception interrupt switch receives the transmission state, 
   is likely to cause the receiver can not receive data interrupt has been closed */
#else
  #define PC_RS485Send_Enable()     
  #define PC_RS485Receive_Enable()  

#endif
//--------------------------------------------------------------------

// LED Configuration
#define  RCC_APB2Periph_LED  RCC_APB2Periph_GPIOC
#define LED_PORT    GPIOC  //GPIOC
#define LED_PIN    8  
  
  #define LED_Green_ON()   myGPIO_ResetBits(LED_PORT, LED_PIN) //SetBit32(LEDPORT->ODR, 13)
  #define LED_Green_OFF()  myGPIO_SetBits(LED_PORT, LED_PIN)   //ClrBit32(LEDPORT->ODR, 13)


#endif
