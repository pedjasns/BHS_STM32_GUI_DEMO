#ifndef __ADS7843_H
#define __ADS7843_H

// A/D ͨ��ѡ�������ֺ͹����Ĵ���
// A/D ͨ��ѡ�������ֺ͹����Ĵ���
#define	CHX 	0x90 	//ͨ��X+��ѡ�������	
#define	CHY 	0xd0	//ͨ��Y+��ѡ������� 

#define TP_DCLK_H() myGPIO_SetBits(GPIOD, 10)

#define TP_DCLK_L() myGPIO_ResetBits(GPIOD, 10)

#define TP_CS_H()  myGPIO_SetBits(GPIOD, 12)

#define TP_CS_L()  myGPIO_ResetBits(GPIOD, 12)

#define TP_DIN_H() myGPIO_SetBits(GPIOD, 8)

#define TP_DIN_L() myGPIO_ResetBits(GPIOD, 8)

#define TP_DOUT		PD6in  //GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)//( IOPIN1 & MASK_DOUT )	//��������
#define TP_BUSY		PD9in  //GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)//( IOPIN1 & MASK_DOUT )	//��������

void TP_Init(void);
u16 Read_XY(u8 xy);
uint8 TP_GetAdXY(u16 *x, u16 *y);
uint8 TP_GetAdXY2(u16 *x, u16 *y, uint32 delay);
uint8 TP_GetLCDXY(u16 *x, u16 *y);
#endif

