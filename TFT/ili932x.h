#ifndef __ILI932X_H
#define __ILI932X_H



//定义驱动IC是ILI9320还是ILI9325
#define  ILI9325 

//屏幕旋转定义 数字按照 ID[1:0]AM 按照PDF中的配置定义
#define ID_AM  110

#define  XMAX   240
#define  YMAX   320

//屏幕开始时显示方式，注意：当IDelay时显示第一幅画面是逐像素刷新的
//此时必须手动在刷新结束后加上  LCD_WR_REG(0x0007,0x0173);才能显示
//当Immediately时没有被注释掉是不需要此过程
//#define Delay //Display Delay//Immediately

//硬件相关的子函数

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
* 名    称：u16 CheckController(void)
* 功    能：返回控制器代码
* 入口参数：无
* 出口参数：控制器型号
* 说    明：调用后返回兼容型号的控制器型号
* 调用方法：code=CheckController();
****************************************************************************/
u16 CheckController(void);

/**********************************************
函数名：LCD初始化函数
功能：初始化LCD
入口参数：无
返回值：无
***********************************************/
void LCD_Initialize(void);

/******************************************
函数名：LCD写命令函数
功能：向LCD指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
返回值：无
******************************************/
void LCD_WR_REG(u16 Index, u16 CongfigTemp);

//带参数的命令写入
void LCD_WR_CmdPar(u16 Index, u16 par);

//不带参数的命令写入
void LCD_WR_Cmd(u16 cmd);

void LCD_WR_DATA(u16 par);

  
u16 LCD_Read(void);

//读取LCD上 1个像素的颜色值
u16 LCD_ReadPixel(u16 x, u16 y);

/************************************************
函数名：LCD写开始函数
功能：控制LCD控制引脚 执行写操作
入口参数：无
返回值：无
************************************************/
void LCD_WR_Start(void);


/****************************************************************
函数名：LCD写1个8*16 ASCII字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示30个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和Set_Cs;为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteEN(u16 x, u16 y, u16 CharColor, u8 ASCIICode);

void LCDWriteEN16(u16 x, u16 y, u8 ASCIICode, u16 CharColor, u16 bkColor);

/****************************************************************
函数名：LCD写1个16*16 汉字字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示30个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和Set_Cs;为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteCN(u16 x, u16 y, u16 CharColor, u8 *p);

//设置文字显示区域
void SetRectText(u16 x, u16 y, u16 w, u16 h);

/************************************************************
函数名：LCD写字符串函数
功能：向指定位置写入一个或多个字符，本函数带自动换行功能
入口参数：x,横向坐标，由左到右分别是0~29 
          y,纵向坐标，由上到下分别为0~19
          CharColaor,字符的颜色
          *s 指向要写的字符串
返回值：无
*************************************************************/
void LCD_WriteString(uint16 x, uint16 y, uint16 CharColor, char* p, uint8 font);
void LCD_WriteString16(uint16 x, uint16 y, uint16 CharColor, char* p);

void LCD_WriteString24(uint16 x, uint16 y, uint16 CharColor, char* p);

/*********************************************************
函数名：SPI取ASCII码子程序
输入参数：u8 ASCII 输入的ASCII码，如'A'
返回值：无
说明：输入一个ASCII码，取得它在SPI FLASH中的16Byte显示代码
并将其存放到一个16byte的ASCII显示缓冲CharBuffer[]中
**********************************************************/
//void GetASCIICode(u8* buf, u8 ASCII);
void GetASCIICode(u8* buf, u8 ASCII, u8 dot);

/*********************************************************
函数名：SPI中文显示码子程序
输入参数：u16 ASCII 输入的中文，如"我"
          BaseAddr 基地 即显示代码在FLASH中的起始位置
返回值：无
说明：输入一个中文，取得它在SPI FLASH中的32Byte显示代码
并将其存放到一个32byte的显示缓冲ChineseBuffer[]
**********************************************************/
void GetChineseCode(u8* dot, u8 *p);
void GetChineseCode24(u8* dot, u8 *p);

void DrawPixel(u16 x, u16 y, u16 Color);

void Delay_nms(int n);

void test_color(void);

#endif

