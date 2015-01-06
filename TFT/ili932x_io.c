
#include "..\APP\includes.h"

//#include "include.h"


///******************************************
// Function name: LCD write command function
// Function: write commands or data to the LCD should specify the location
// Entrance parameters: Index register address to be addressed
// Data or write command value ConfigTemp
// Return Value: None
//******************************************/
//void LCD_WR_REG(u16 Index, u16 CongfigTemp)
//{
//   CS_TFT_L();
//   
//   RS_TFT_L();
//   RD_TFT_H();
//   DataToWrite(Index);
//   WR_TFT_L();
//   WR_TFT_H();
//   RS_TFT_H();
//   //----------------------
//   DataToWrite(CongfigTemp);       
//   WR_TFT_L();
//   WR_TFT_H();
//   
//   CS_TFT_H();
//}
//
//
///************************************************
// Function name: LCD write start function
// Function: Control LCD control pins to perform a write operation
// Entrance parameters: None
// Return Value: None
//************************************************/
//void LCD_WR_Start(void)
//{
//   CS_TFT_L();
//   
//   RS_TFT_L();
//   RD_TFT_H();
//   DataToWrite(0x0022);
//   WR_TFT_L();
//   WR_TFT_H();
//   RS_TFT_H();
//}

//Write command with parameters
void LCD_WR_CmdPar(u16 Index, u16 par)
{
   CS_TFT_L();
   
   RS_TFT_L();
   DataToWrite(Index);
   WR_TFT_L();
   WR_TFT_H();
   RS_TFT_H();
   
   //----------------------
   DataToWrite(par);       
   WR_TFT_L();
   WR_TFT_H();
   
   CS_TFT_H();
}

// Write command with no arguments
void LCD_WR_Cmd(u16 cmd)
{
   CS_TFT_L();
   
   RS_TFT_L();
   DataToWrite(cmd);
   WR_TFT_L();
   WR_TFT_H();
   RS_TFT_H();
   
   CS_TFT_H();
}


void LCD_WR_DATA(u16 par)
{
   CS_TFT_L();
   
   DataToWrite(par);
   WR_TFT_L();
   WR_TFT_H();
   
   CS_TFT_H();
}

  
u16 LCD_Read(void)
{u16 temp;
   
   
   CS_TFT_L();
   SET_DATA_IN();
   
   // Virtual Reading 1
   RD_TFT_L();
   temp=(u16)GPIOE->IDR;
   RD_TFT_H();
   
   RD_TFT_L();
   temp=(u16)GPIOE->IDR;
   RD_TFT_H();
   
   SET_DATA_OUT();
   CS_TFT_H();
   
   
   return(temp);
}

//Reading the LCD 1 pixel color values
//u16 LCD_ReadPixel(u16 x, u16 y)
//{u16 temp;
//   
//    LCD_WR_CmdPar(0x20, x);
//    LCD_WR_CmdPar(0x21, y);
//    LCD_WR_Cmd(0x0022);
//    
//    temp=LCD_Read();
//    
//    LCD_WR_CmdPar(0x20, x);
//    LCD_WR_CmdPar(0x21, y);
//    LCD_WR_Cmd(0x0022);
//    LCD_WR_DATA(temp);
//    //------------------------------
//    LCD_WR_CmdPar(0x20, x);
//    LCD_WR_CmdPar(0x21, y);
//    LCD_WR_Cmd(0x0022);
//    
//    temp=LCD_Read();
//    
//    LCD_WR_CmdPar(0x20, x);
//    LCD_WR_CmdPar(0x21, y);
//    LCD_WR_Cmd(0x0022);
//    LCD_WR_DATA(temp);
//    
//    return(temp);
//}



