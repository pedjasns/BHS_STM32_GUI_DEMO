#ifndef __ILI932X_H
#define __ILI932X_H



//��������IC��ILI9320����ILI9325
#define  ILI9325 

//��Ļ��ת���� ���ְ��� ID[1:0]AM ����PDF�е����ö���
#define ID_AM  110

#define  XMAX   240
#define  YMAX   320

//��Ļ��ʼʱ��ʾ��ʽ��ע�⣺��IDelayʱ��ʾ��һ��������������ˢ�µ�
//��ʱ�����ֶ���ˢ�½��������  LCD_WR_REG(0x0007,0x0173);������ʾ
//��Immediatelyʱû�б�ע�͵��ǲ���Ҫ�˹���
//#define Delay //Display Delay//Immediately

//Ӳ����ص��Ӻ���

#define CS_TFT_H()   myGPIO_SetBits(GPIOD, 0);
#define CS_TFT_L()   myGPIO_ResetBits(GPIOD, 0);

#define RS_TFT_H()   myGPIO_SetBits(GPIOD, 1);
#define RS_TFT_L()   myGPIO_ResetBits(GPIOD, 1);

#define WR_TFT_H()   myGPIO_SetBits(GPIOD, 2);
#define WR_TFT_L()   myGPIO_ResetBits(GPIOD, 2);

#define RD_TFT_H()   myGPIO_SetBits(GPIOD, 3);
#define RD_TFT_L()   myGPIO_ResetBits(GPIOD, 3);

#define RST_TFT_H()  myGPIO_SetBits(GPIOD, 4);
#define RST_TFT_L()  myGPIO_ResetBits(GPIOD, 4);
//-------------------------------------------------------

//#define Set_Cs  SetBit32(GPIOD->ODR, 0);
//#define Clr_Cs  ClrBit32(GPIOD->ODR, 0);
//
//#define Set_Rs  SetBit32(GPIOD->ODR, 1);
//#define Clr_Rs  ClrBit32(GPIOD->ODR, 1);
//
//#define Set_nWr SetBit32(GPIOD->ODR, 2);
//#define Clr_nWr ClrBit32(GPIOD->ODR, 2);
//
//#define Set_nRd SetBit32(GPIOD->ODR, 3);
//#define Clr_nRd ClrBit32(GPIOD->ODR, 3);
//
//#define Set_Rst SetBit32(GPIOD->ODR, 4);
//#define Clr_Rst ClrBit32(GPIOD->ODR, 4);
//-------------------------------------------------------

//#define Set_Cs  myGPIO_SetBits(GPIOD, 0);
//#define Clr_Cs  myGPIO_ResetBits(GPIOD, 0);
//
//#define Set_Rs  myGPIO_SetBits(GPIOD, 1);
//#define Clr_Rs  myGPIO_ResetBits(GPIOD, 1);
//
//#define Set_nWr myGPIO_SetBits(GPIOD, 2);
//#define Clr_nWr myGPIO_ResetBits(GPIOD, 2);
//
//#define Set_nRd myGPIO_SetBits(GPIOD, 3);
//#define Clr_nRd myGPIO_ResetBits(GPIOD, 3);
//
//#define Set_Rst myGPIO_SetBits(GPIOD, 4);
//#define Clr_Rst myGPIO_ResetBits(GPIOD, 4);

//-------------------------------------------------------

//#define Set_Cs  GPIO_SetBits(GPIOD,GPIO_Pin_0);
//#define Clr_Cs  GPIO_ResetBits(GPIOD,GPIO_Pin_0);
//
//#define Set_Rs  GPIO_SetBits(GPIOD,GPIO_Pin_1);
//#define Clr_Rs  GPIO_ResetBits(GPIOD,GPIO_Pin_1);
//
//#define Set_nWr GPIO_SetBits(GPIOD,GPIO_Pin_2);
//#define Clr_nWr GPIO_ResetBits(GPIOD,GPIO_Pin_2);
//
//#define Set_nRd GPIO_SetBits(GPIOD,GPIO_Pin_3);
//#define Clr_nRd GPIO_ResetBits(GPIOD,GPIO_Pin_3);
//
//#define Set_Rst GPIO_SetBits(GPIOD,GPIO_Pin_4);
//#define Clr_Rst GPIO_ResetBits(GPIOD,GPIO_Pin_4);


#define LCD_Light_ON   myGPIO_SetBits(GPIOD, 12);
#define LCD_Light_OFF  myGPIO_ResetBits(GPIOD, 12);


#define DataToWrite(dat)  {GPIOE->ODR = dat;}


   //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
   //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   //GPIO_Init(GPIOE, &GPIO_InitStructure);
   
   //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
   //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   //GPIO_Init(GPIOE, &GPIO_InitStructure);
   
#define SET_DATA_IN() {\
   GPIOE->CRL=0x88888888;\
   GPIOE->CRH=0x88888888;\
   GPIOE->BSRR=0x0000ffff;}

#define SET_DATA_OUT() {\
	 GPIOE->CRL=0x33333333;\
   GPIOE->CRH=0x33333333;}

void LCD_Configuration(void);

/****************************************************************************
* ��    �ƣ�u16 CheckController(void)
* ��    �ܣ����ؿ���������
* ��ڲ�������
* ���ڲ������������ͺ�
* ˵    �������ú󷵻ؼ����ͺŵĿ������ͺ�
* ���÷�����code=CheckController();
****************************************************************************/
u16 CheckController(void);

/**********************************************
��������LCD��ʼ������
���ܣ���ʼ��LCD
��ڲ�������
����ֵ����
***********************************************/
void LCD_Initialize(void);

/******************************************
��������LCDд�����
���ܣ���LCDָ��λ��д��Ӧ�����������
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
����ֵ����
******************************************/
void LCD_WR_REG(u16 Index, u16 CongfigTemp);

//������������д��
void LCD_WR_CmdPar(u16 Index, u16 par);

//��������������д��
void LCD_WR_Cmd(u16 cmd);

void LCD_WR_DATA(u16 par);

  
u16 LCD_Read(void);

//��ȡLCD�� 1�����ص���ɫֵ
u16 LCD_ReadPixel(u16 x, u16 y);

/************************************************
��������LCDд��ʼ����
���ܣ�����LCD�������� ִ��д����
��ڲ�������
����ֵ����
************************************************/
void LCD_WR_Start(void);


/****************************************************************
��������LCDд1��8*16 ASCII�ַ�����
��ڲ�����x,�������꣬�����ҷֱ���0~240-1 
          y,�������꣬���ϵ��·ֱ�Ϊ0~320-1
          CharColaor,�ַ�����ɫ 
          CharBackColor,�ַ�������ɫ 
         ASCIICode,��Ӧ�ַ���ASCII��
Ҳ����˵��320240�ֱ��ʵ���ʾ������������ʾ30��ASCII�ַ�����������ʾ20��
����ֵ����

ע�⣡���������������ʹ�ô˺�����Ӧ�ü���LCD_Rs_H()��Set_Cs;Ϊ���Ż�ϵͳʡȥ��
���ָ�����˺���ִ�е���һ�������д�����RS_L�������д�뽫����
����ΪILI9320��Ϊ��RS_Lʱд���������
*****************************************************************/
void LCDWriteEN(u16 x, u16 y, u16 CharColor, u8 ASCIICode);

void LCDWriteEN16(u16 x, u16 y, u8 ASCIICode, u16 CharColor, u16 bkColor);

/****************************************************************
��������LCDд1��16*16 �����ַ�����
��ڲ�����x,�������꣬�����ҷֱ���0~240-1 
          y,�������꣬���ϵ��·ֱ�Ϊ0~320-1
          CharColaor,�ַ�����ɫ 
          CharBackColor,�ַ�������ɫ 
         ASCIICode,��Ӧ�ַ���ASCII��
Ҳ����˵��320240�ֱ��ʵ���ʾ������������ʾ30��ASCII�ַ�����������ʾ20��
����ֵ����

ע�⣡���������������ʹ�ô˺�����Ӧ�ü���LCD_Rs_H()��Set_Cs;Ϊ���Ż�ϵͳʡȥ��
���ָ�����˺���ִ�е���һ�������д�����RS_L�������д�뽫����
����ΪILI9320��Ϊ��RS_Lʱд���������
*****************************************************************/
void LCDWriteCN(u16 x, u16 y, u16 CharColor, u8 *p);

//����������ʾ����
void SetRectText(u16 x, u16 y, u16 w, u16 h);

/************************************************************
��������LCDд�ַ�������
���ܣ���ָ��λ��д��һ�������ַ������������Զ����й���
��ڲ�����x,�������꣬�����ҷֱ���0~29 
          y,�������꣬���ϵ��·ֱ�Ϊ0~19
          CharColaor,�ַ�����ɫ
          *s ָ��Ҫд���ַ���
����ֵ����
*************************************************************/
void LCD_WriteString(uint16 x, uint16 y, uint16 CharColor, char* p, uint8 font);
void LCD_WriteString16(uint16 x, uint16 y, uint16 CharColor, char* p);

void LCD_WriteString24(uint16 x, uint16 y, uint16 CharColor, char* p);

/*********************************************************
��������SPIȡASCII���ӳ���
���������u8 ASCII �����ASCII�룬��'A'
����ֵ����
˵��������һ��ASCII�룬ȡ������SPI FLASH�е�16Byte��ʾ����
�������ŵ�һ��16byte��ASCII��ʾ����CharBuffer[]��
**********************************************************/
//void GetASCIICode(u8* buf, u8 ASCII);
void GetASCIICode(u8* buf, u8 ASCII, u8 dot);

/*********************************************************
��������SPI������ʾ���ӳ���
���������u16 ASCII ��������ģ���"��"
          BaseAddr ���� ����ʾ������FLASH�е���ʼλ��
����ֵ����
˵��������һ�����ģ�ȡ������SPI FLASH�е�32Byte��ʾ����
�������ŵ�һ��32byte����ʾ����ChineseBuffer[]
**********************************************************/
void GetChineseCode(u8* dot, u8 *p);
void GetChineseCode24(u8* dot, u8 *p);

void DrawPixel(u16 x, u16 y, u16 Color);

void Delay_nms(int n);

void test_color(void);

#endif

