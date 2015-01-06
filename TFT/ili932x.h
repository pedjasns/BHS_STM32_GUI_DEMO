#ifndef __ILI932X_H
#define __ILI932X_H


// Define the driver IC is ILI9320 or ILI9325
#define  ILI9325 

// Definition digital screen rotation in accordance with the ID [1: 0] AM PDF 
// configuration in accordance with the definition
#define ID_AM  110

#define  XMAX   240
#define  YMAX   320

// Display screen at the beginning of the way, note: displayed when IDelay 
// when the first picture is refreshed by-pixel at this point you must manually 
// refresh the end after adding LCD_WR_REG (0x0007,0x0173); to display
// If not commented Immediately when this process is not needed
//#define Delay //Display Delay//Immediately

// Hardware-related Functions

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
* Name: u16 CheckController (void)
* Function: Returns the controller code
* Entrance parameters: None
* Export parameters: Controller Model
* Description: Returns After calling the controller model compatible models
* Call the method: code = CheckController ();
****************************************************************************/
u16 CheckController(void);

/**********************************************
Function name: LCD initialization function
Function: Initializes LCD
Entrance parameters: None
Return Value: None
***********************************************/
void LCD_Initialize(void);

/******************************************
Function name: LCD write command function
Function: write commands or data to the LCD should specify the location
Entrance parameters: Index register address to be addressed
           ConfigTemp written data or command value
Return Value: None
******************************************/
void LCD_WR_REG(u16 Index, u16 CongfigTemp);

// Write command with parameters
void LCD_WR_CmdPar(u16 Index, u16 par);

//Write command with no arguments
void LCD_WR_Cmd(u16 cmd);

void LCD_WR_DATA(u16 par);

  
u16 LCD_Read(void);

//Reading the LCD 1 pixel color values
u16 LCD_ReadPixel(u16 x, u16 y);

/************************************************
Function name: LCD write start function
Function: Control LCD control pins to perform a write operation
Entrance parameters: None
Return Value: None
************************************************/
void LCD_WR_Start(void);


/****************************************************************
Function name: LCD write an 8 * 16 ASCII characters function
Entrance parameters: x, transverse coordinates, from left to right are from 0 to 240-1
           y, longitudinal coordinates, from top to bottom, respectively, 0 to 320-1
           Color CharColaor, characters
           CharBackColor, character background color
          ASCIICode, the corresponding ASCII character code
That is to say, 320 240 resolution display, horizontal 30 ASCII characters can be displayed, can display 20 lines of vertical
Return Value: None

Note! ! ! ! ! If you use this function alone should add LCD_Rs_H () and Set_Cs; To optimize the system eliminates the need for
This directive, assuming a statement on the implementation of this function is to write commands, (RS_L case) is written to the error
Because ILI9320 think when RS_L when writing the command
*****************************************************************/
void LCDWriteEN(u16 x, u16 y, u16 CharColor, u8 ASCIICode);

void LCDWriteEN16(u16 x, u16 y, u8 ASCIICode, u16 CharColor, u16 bkColor);

/****************************************************************
Function name: LCD write a 16 * 16 Chinese character function
Entrance parameters: x, transverse coordinates, from left to right are from 0 to 240-1
           y, longitudinal coordinates, from top to bottom, respectively, 0 to 320-1
           Color CharColaor, characters
           CharBackColor, character background color
          ASCIICode, the corresponding ASCII character code
That is to say, 320 240 resolution display, horizontal 30 ASCII characters can be displayed, can display 20 lines of vertical
Return Value: None

Note! ! ! ! ! If you use this function alone should add LCD_Rs_H () and Set_Cs; To optimize the system eliminates the need for
This directive, assuming a statement on the implementation of this function is to write commands, (RS_L case) is written to the error
Because ILI9320 think when RS_L when writing the command
*****************************************************************/
void LCDWriteCN(u16 x, u16 y, u16 CharColor, u8 *p);

//Set the text display area
void SetRectText(u16 x, u16 y, u16 w, u16 h);

/************************************************************
Function name: LCD write String Functions
Function: to write one or more characters to a specified location, this function with automatic wrap feature
Entrance parameters: x, transverse coordinates, from left to right are from 0 to 29
           y, longitudinal coordinates, from top to bottom, respectively, from 0 to 19
           Color CharColaor, characters
           * s point string to be written
Return Value: None
*************************************************************/
void LCD_WriteString(uint16 x, uint16 y, uint16 CharColor, char* p, uint8 font);
void LCD_WriteString16(uint16 x, uint16 y, uint16 CharColor, char* p);

void LCD_WriteString24(uint16 x, uint16 y, uint16 CharColor, char* p);

/*********************************************************
Function name: SPI take ASCII completely different program
Input parameters: u8 ASCII ASCII code input, such as 'A'
Return Value: None
Description: Enter an ASCII code, get it 16Byte in SPI FLASH display code
And store it into a 16byte the ASCII display buffer CharBuffer [] in
**********************************************************/
//void GetASCIICode(u8* buf, u8 ASCII);
void GetASCIICode(u8* buf, u8 ASCII, u8 dot);

/*********************************************************
Function name: SPI Chinese display completely different program
Input parameters: u16 ASCII input of Chinese, such as "I"
           BaseAddr base code is displayed in the starting position FLASH
Return Value: None
Description: Enter a Chinese made it 32Byte in SPI FLASH display code
And store it into a display buffer 32byte ChineseBuffer []
**********************************************************/
void GetChineseCode(u8* dot, u8 *p);
void GetChineseCode24(u8* dot, u8 *p);

void DrawPixel(u16 x, u16 y, u16 Color);

void Delay_nms(int n);

void test_color(void);

#endif

