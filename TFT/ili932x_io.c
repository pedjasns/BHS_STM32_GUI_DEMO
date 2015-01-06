
#include "..\APP\includes.h"

//#include "include.h"


///******************************************
//函数名：LCD写命令函数
//功能：向LCD指定位置写入应有命令或数据
//入口参数：Index 要寻址的寄存器地址
//          ConfigTemp 写入的数据或命令值
//返回值：无
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
//函数名：LCD写开始函数
//功能：控制LCD控制引脚 执行写操作
//入口参数：无
//返回值：无
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

//带参数的命令写入
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

//不带参数的命令写入
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
   
   //虚读1次
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

//读取LCD上 1个像素的颜色值
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



