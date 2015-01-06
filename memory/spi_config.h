#if !defined( _SPI_CONFIG_H )
#define _SPI_CONFIG_H


#define  mytsk_lock()    //tsk_lock()
#define  mytsk_unlock()  //tsk_unlock()

//#define  mytsk_lock()    os_sem_wait (SPI2semaphore, 0xffff)
//#define  mytsk_unlock()  os_sem_send (SPI2semaphore)


#define  SPI_MEM1_PIN   5
#define  SPI_MEM1_PORT  GPIOB
/* Select SPI MEM1: ChipSelect pin low  */
#define SPI_MEM1_CS_LOW()     myGPIO_ResetBits(SPI_MEM1_PORT, SPI_MEM1_PIN)
/* Deselect SPI MEM1: ChipSelect pin high */
#define SPI_MEM1_CS_HIGH()    myGPIO_SetBits(SPI_MEM1_PORT, SPI_MEM1_PIN)

#define  SPI_MEM2_PIN   6
#define  SPI_MEM2_PORT  GPIOB
/* Select SPI MEM2: ChipSelect pin low  */
#define SPI_MEM2_CS_LOW()     myGPIO_ResetBits(SPI_MEM2_PORT, SPI_MEM2_PIN)
/* Deselect SPI MEM2: ChipSelect pin high */
#define SPI_MEM2_CS_HIGH()    myGPIO_SetBits(SPI_MEM2_PORT, SPI_MEM2_PIN)


#define  SPI_MEM3_PIN   7
#define  SPI_MEM3_PORT  GPIOB
/* Select SPI MEM3: ChipSelect pin low  */
#define SPI_MEM3_CS_LOW()     myGPIO_ResetBits(SPI_MEM3_PORT, SPI_MEM3_PIN)
/* Deselect SPI MEM3: ChipSelect pin high */
#define SPI_MEM3_CS_HIGH()    myGPIO_SetBits(SPI_MEM3_PORT, SPI_MEM3_PIN)

//#define  SPI_FLASH_PIN   6
//#define  SPI_FLASH_PORT  GPIOB
/* Select SPI FLASH: ChipSelect pin low  */
#define SPI_FLASH_CS_LOW()     {mytsk_lock(); SPI_MEM2_CS_LOW();}
/* Deselect SPI FLASH: ChipSelect pin high */
#define SPI_FLASH_CS_HIGH()    {SPI_MEM2_CS_HIGH(); mytsk_unlock();}



//下面的定义3.50与3.22的库不一样
//----------------------------------------------
#define SPI_FLAG_RXNE           SPI_I2S_FLAG_RXNE
#define SPI_FLAG_TXE            SPI_I2S_FLAG_TXE
//#define SPI_FLAG_CRCERR                    ((u16)0x0010)
//#define SPI_FLAG_MODF                      ((u16)0x0020)
#define SPI_FLAG_OVR            SPI_I2S_FLAG_OVR
#define SPI_FLAG_BSY            SPI_I2S_FLAG_BSY
//----------------------------------------------
#define SPI_GetFlagStatus       SPI_I2S_GetFlagStatus
#define SPI_SendData            SPI_I2S_SendData
#define SPI_ReceiveData         SPI_I2S_ReceiveData


void mySPI_Init(void);

uint8 SPI_Send_Byte(uint8 byte);

#endif
