/******************************************************************************
* 文件名称：ili932x.c
* 摘 要：支持ILI9320和ILI9325驱动IC控制的QVGA显示屏，使用16位并行传输
  到头文件中配置 屏幕使用方向和驱动IC类型
  注意：16位数据线色彩分布>>  BGR(565)

* 当前版本：V1.3
* 修改说明：版本修订说明：
  1.修改翻转模式下的ASCII字符写Bug
  2.增加可以在翻转模式下的自动行写
  3.优化刷图片 使用流水线方法提效率
*重要说明！
在.h文件中，#define Immediately时是立即显示当前画面
而如果#define Delay，则只有在执行了LCD_WR_CmdPar(0x0007,0x0173);
之后才会显示，执行一次LCD_WR_CmdPar(0x0007,0x0173)后，所有写入数
据都立即显示。
#define Delay一般用在开机画面的显示，防止显示出全屏图像的刷新
过程
******************************************************************************/
#include "..\APP\includes.h"





/****************************************************************
函数名：LCD配置函数
功能：配置所有和LCD相关的GPIO和时钟
引脚分配为：
PE8--PE15——16Bit数据总线低8位
PCE--PE7 ——16Bit数据总线高8位
PC8 ——LCD_cs
PC9 ——LCD_rs*
PC10——LCD_wr
PC11——LCD_rd*
PC12——LCD_rst
PC13——LCD_blaklight 背光靠场效应管驱动背光模块
*****************************************************************/
void LCD_Configuration(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   /*开启相应时钟 */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
   /*所有LCD引脚配置为推挽输出*/
   /*16位数据低8位*//*16位数据高8位*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;

   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOE, &GPIO_InitStructure);

   /*控制脚*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

   /*背光控制*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：u16 CheckController(void)
* 功    能：返回控制器代码
* 入口参数：无
* 出口参数：控制器型号
* 说    明：调用后返回兼容型号的控制器型号
* 调用方法：code=CheckController();
****************************************************************************/
u16 CheckController(void)
{
   u16 tmp = 0;//, tmp1 = 0, tmp2 = 0; 
   GPIO_InitTypeDef GPIO_InitStructure;

   DataToWrite(0xffff);//数据线全高
   RST_TFT_H();
   WR_TFT_H();
   CS_TFT_H();
   RS_TFT_H();
   RD_TFT_H();
   RST_TFT_H();
   Delay_nms(1);
   RST_TFT_L();
   Delay_nms(1);
   RST_TFT_H();
   Delay_nms(1);
   LCD_WR_CmdPar(0x0000, 0x0001);  //start oscillation
   Delay_nms(1);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOE, &GPIO_InitStructure);
  
   CS_TFT_L();//GPIO_ResetBits(GPIOC,GPIO_Pin_8);
  
   RS_TFT_H();//GPIO_SetBits(GPIOC,GPIO_Pin_9);
  
   RD_TFT_L();//GPIO_ResetBits(GPIOC,GPIO_Pin_11);
   tmp = GPIO_ReadInputData(GPIOE);
  
   RD_TFT_H();//GPIO_SetBits(GPIOC,GPIO_Pin_11);
  
   CS_TFT_H();//GPIO_SetBits(GPIOC,GPIO_Pin_8);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOE, &GPIO_InitStructure);

   return tmp;
}

/**********************************************
函数名：LCD初始化函数
功能：初始化LCD
入口参数：无
返回值：无
***********************************************/
void LCD_Initialize(void)
{
   u16 i;
   
   LCD_Light_ON;
   DataToWrite(0xffff);//数据线全高
   RST_TFT_H();
   WR_TFT_H();
   CS_TFT_H();
   RS_TFT_H();
   RD_TFT_H();
   
   RST_TFT_H();
   Delay_nms(1);
   RST_TFT_L();
   Delay_nms(1);
   RST_TFT_H();
   Delay_nms(1); 


   i = CheckController();
	//我买的屏返回不确定.lzt
	i=0x9325;
   if (i == 0x9325 || i == 0x9328)
   {
      LCD_WR_CmdPar(0x00e7, 0x0010);      
      LCD_WR_CmdPar(0x0000, 0x0001);         //start internal osc
      LCD_WR_CmdPar(0x0001, 0x0100);     
      LCD_WR_CmdPar(0x0002, 0x0700);            //power on sequence                     
      LCD_WR_CmdPar(0x0003, (1 << 12) | (1 << 5) | (1 << 4));  //65K 
      LCD_WR_CmdPar(0x0004, 0x0000);                                   
      LCD_WR_CmdPar(0x0008, 0x0207);              
      LCD_WR_CmdPar(0x0009, 0x0000);         
      LCD_WR_CmdPar(0x000a, 0x0000);            //display setting         
      LCD_WR_CmdPar(0x000c, 0x0001);            //display setting          
      LCD_WR_CmdPar(0x000d, 0x0000);            //0f3c          
      LCD_WR_CmdPar(0x000f, 0x0000);
      LCD_WR_CmdPar(0x0010, 0x0000);   
      LCD_WR_CmdPar(0x0011, 0x0007);
      LCD_WR_CmdPar(0x0012, 0x0000);                                                                 
      LCD_WR_CmdPar(0x0013, 0x0000);                 
      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0010, 0x1590);   
      LCD_WR_CmdPar(0x0011, 0x0227);
      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0012, 0x009c);                 
      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0013, 0x1900);   
      LCD_WR_CmdPar(0x0029, 0x0023);
      LCD_WR_CmdPar(0x002b, 0x000e);
      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0020, 0x0000);                                                            
      LCD_WR_CmdPar(0x0021, 0x0000);                 

      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0030, 0x0007); 
      LCD_WR_CmdPar(0x0031, 0x0707);   
      LCD_WR_CmdPar(0x0032, 0x0006);
      LCD_WR_CmdPar(0x0035, 0x0704);
      LCD_WR_CmdPar(0x0036, 0x1f04); 
      LCD_WR_CmdPar(0x0037, 0x0004);
      LCD_WR_CmdPar(0x0038, 0x0000);        
      LCD_WR_CmdPar(0x0039, 0x0706);     
      LCD_WR_CmdPar(0x003c, 0x0701);
      LCD_WR_CmdPar(0x003d, 0x000f);
      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x0050, 0x0000);        
      LCD_WR_CmdPar(0x0051, 0x00ef);   
      LCD_WR_CmdPar(0x0052, 0x0000);     
      LCD_WR_CmdPar(0x0053, 0x013f);
      LCD_WR_CmdPar(0x0060, 0xa700);        
      LCD_WR_CmdPar(0x0061, 0x0001); 
      LCD_WR_CmdPar(0x006a, 0x0000);
      LCD_WR_CmdPar(0x0080, 0x0000);
      LCD_WR_CmdPar(0x0081, 0x0000);
      LCD_WR_CmdPar(0x0082, 0x0000);
      LCD_WR_CmdPar(0x0083, 0x0000);
      LCD_WR_CmdPar(0x0084, 0x0000);
      LCD_WR_CmdPar(0x0085, 0x0000);

      LCD_WR_CmdPar(0x0090, 0x0010);     
      LCD_WR_CmdPar(0x0092, 0x0000);  
      LCD_WR_CmdPar(0x0093, 0x0003);
      LCD_WR_CmdPar(0x0095, 0x0110);
      LCD_WR_CmdPar(0x0097, 0x0000);        
      LCD_WR_CmdPar(0x0098, 0x0000);  
      //display on sequence     
      LCD_WR_CmdPar(0x0007, 0x0133);

      LCD_WR_CmdPar(0x0020, 0x0000);                                                            
      LCD_WR_CmdPar(0x0021, 0x0000);
   }
   else if (i == 0x9320)
   {
      LCD_WR_CmdPar(0x00, 0x0000);
      LCD_WR_CmdPar(0x01, 0x0100);  //Driver Output Contral.
      LCD_WR_CmdPar(0x02, 0x0700);  //LCD Driver Waveform Contral.
      LCD_WR_CmdPar(0x03, 0x1030);  //Entry Mode Set.

      LCD_WR_CmdPar(0x04, 0x0000);  //Scalling Contral.
      LCD_WR_CmdPar(0x08, 0x0202);  //Display Contral 2.(0x0207)
      LCD_WR_CmdPar(0x09, 0x0000);  //Display Contral 3.(0x0000)
      LCD_WR_CmdPar(0x0a, 0x0000);  //Frame Cycle Contal.(0x0000)
      LCD_WR_CmdPar(0x0c, (1 << 0));   //Extern Display Interface Contral 1.(0x0000)
      LCD_WR_CmdPar(0x0d, 0x0000);  //Frame Maker Position.
      LCD_WR_CmdPar(0x0f, 0x0000);  //Extern Display Interface Contral 2.

      for (i = 50000; i > 0; i--)
         ;
      LCD_WR_CmdPar(0x07, 0x0101);  //Display Contral.
      for (i = 50000; i > 0; i--)
         ;

      LCD_WR_CmdPar(0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4));   //Power Control 1.(0x16b0)
      LCD_WR_CmdPar(0x11, 0x0007);                       //Power Control 2.(0x0001)
      LCD_WR_CmdPar(0x12, (1 << 8) | (1 << 4) | (0 << 0));              //Power Control 3.(0x0138)
      LCD_WR_CmdPar(0x13, 0x0b00);                       //Power Control 4.
      LCD_WR_CmdPar(0x29, 0x0000);                       //Power Control 7.

      LCD_WR_CmdPar(0x2b, (1 << 14) | (1 << 4));

      LCD_WR_CmdPar(0x50, 0);    //Set X Start.
      LCD_WR_CmdPar(0x51, 239);  //Set X End.
      LCD_WR_CmdPar(0x52, 0);    //Set Y Start.
      LCD_WR_CmdPar(0x53, 319);  //Set Y End.

      LCD_WR_CmdPar(0x60, 0x2700);  //Driver Output Control.
      LCD_WR_CmdPar(0x61, 0x0001);  //Driver Output Control.
      LCD_WR_CmdPar(0x6a, 0x0000);  //Vertical Srcoll Control.

      LCD_WR_CmdPar(0x80, 0x0000);  //Display Position? Partial Display 1.
      LCD_WR_CmdPar(0x81, 0x0000);  //RAM Address Start? Partial Display 1.
      LCD_WR_CmdPar(0x82, 0x0000);  //RAM Address End-Partial Display 1.
      LCD_WR_CmdPar(0x83, 0x0000);  //Displsy Position? Partial Display 2.
      LCD_WR_CmdPar(0x84, 0x0000);  //RAM Address Start? Partial Display 2.
      LCD_WR_CmdPar(0x85, 0x0000);  //RAM Address End? Partial Display 2.

      LCD_WR_CmdPar(0x90, (0 << 7) | (16 << 0));   //Frame Cycle Contral.(0x0013)
      LCD_WR_CmdPar(0x92, 0x0000);  //Panel Interface Contral 2.(0x0000)
      LCD_WR_CmdPar(0x93, 0x0001);  //Panel Interface Contral 3.
      LCD_WR_CmdPar(0x95, 0x0110);  //Frame Cycle Contral.(0x0110)
      LCD_WR_CmdPar(0x97, (0 << 8));   //
      LCD_WR_CmdPar(0x98, 0x0000);  //Frame Cycle Contral.


      LCD_WR_CmdPar(0x07, 0x0173);  //(0x0173)
   }
}

u16 LCD_ReadPixel(u16 x, u16 y)
{u16 temp;
   
    LCD_WR_CmdPar(0x20, x);
    LCD_WR_CmdPar(0x21, y);
    LCD_WR_Cmd(0x0022);
    
    temp=LCD_Read();
    
    LCD_WR_CmdPar(0x20, x);
    LCD_WR_CmdPar(0x21, y);
    LCD_WR_Cmd(0x0022);
    LCD_WR_DATA(temp);
    //------------------------------
    LCD_WR_CmdPar(0x20, x);
    LCD_WR_CmdPar(0x21, y);
    LCD_WR_Cmd(0x0022);
    
    temp=LCD_Read();
    
    LCD_WR_CmdPar(0x20, x);
    LCD_WR_CmdPar(0x21, y);
    LCD_WR_Cmd(0x0022);
    LCD_WR_DATA(temp);
    
    return(temp);
}

//画1个像素
void DrawPixel(u16 x, u16 y, u16 Color)
{
   //LCD_WR_CmdPar(0x20, x);//起始X坐标
   //LCD_WR_CmdPar(0x21, y);//起始Y坐标
   
   LCD_WR_CmdPar(0x20, y);//起始X坐标
   LCD_WR_CmdPar(0x21, 320-1-x);//起始X坐标
   
   LCD_WR_Cmd(0x0022);//开始读/写
   
   LCD_WR_DATA(Color);
}

/*
24*24阵列Y-->>X变换
[BYTE0]   [BYTE3].......
D7        D7     
D6        D6     
.         .      
.         .      
D1        D1     
D0        D0     
[BYTE1]   [BYTE4]
D7        D7     
D6        D6     
.         .      
.         .      
D1        D1     
D0        D0     
[BYTE2]   [BYTE5]
D7        D7     
D6        D6     
.         .      
.         .      
D1        D1     
D0        D0    

--------------------------------->>

[BYTE0]     [BYTE1]     [BYTE2]
D7D6..D1D0  D7D6..D1D0  D7D6..D1D0
[BYTE3]     [BYTE4]     [BYTE5]
D7D6..D1D0  D7D6..D1D0  D7D6..D1D0
.
.
.
*/
//void MODY24(uint8 * p)
//{
// uint8 buf[72];
// uint8 i, j;
// uint8 temp;
// uint8 ix, iy;
// uint16 xy;
//   
//   memset(buf, 0, 72);
//   ix=0;iy=0;
//   for(j=0; j<72; j++)
//   {
//      temp=p[j];
//      for(i=0; i<8; i++)
//      {
//         xy=24*iy+ix;
//         if(isBit8(temp, 7-i))
//         {
//            SetBit8(buf[xy/8], 7-(xy%8));
//         }
//         iy++;
//      }
//      
//      if(iy==24)
//      {
//         iy=0;
//         ix++;//iy变化24次ix才变化1次
//      }
//   }
//   
//   memcpy(p, buf, 72);
//}

/*
//将8*8 Y方向阵列做 X方向转换
void MODY8(uint8 * p)
{
 uint8 buf[8];
 uint8 i, j;
 uint8 temp;
   
   memset(buf, 0, 8);
   for (j = 0; j < 8; j++)
   {
      temp=p[j];
      for (i = 0; i < 8; i++)
      {
         buf[i] <<= 1;
         if(isBit8(temp, i))
            //SetBit8(buf[i], 7-j); 
            SetBit8(buf[i], 0);
      }
   }
   memcpy(p, buf, 8);
}

//将8*16 Y方向阵列做 X方向转换(用于8*16 ASCII)
void MODY16(uint8 * p)
{
   MODY8(p);
   MODY8(p+8);
}

//将16*16 Y方向阵列做 X方向转换(主要用于16*16 汉字)
void MODY32(uint8 * p)
{uint8 temp[16];
 //uint8 i;

   MODY8(p);
   MODY8(p+8);
   
   memcpy(temp, p, 16);
   
   //p[0]=temp[0];
   p[1]=temp[8];
   p[2]=temp[1];
   p[3]=temp[9];
   p[4]=temp[2];
   p[5]=temp[10];
   p[6]=temp[3];
   p[7]=temp[11];
   
   p[8]=temp[4];
   p[9]=temp[12];
   p[10]=temp[5];
   p[11]=temp[13];
   p[12]=temp[6];
   p[13]=temp[14];
   p[14]=temp[7];
   p[15]=temp[15];
   
   MODY8(p+16);
   MODY8(p+24);
   
   memcpy(temp, p+16, 16);
   //p[16]=temp[0];
   p[17]=temp[8];
   p[18]=temp[1];
   p[19]=temp[9];
   p[20]=temp[2];
   p[21]=temp[10];
   p[22]=temp[3];
   p[23]=temp[11];
   
   p[24]=temp[4];
   p[25]=temp[12];
   p[26]=temp[5];
   p[27]=temp[13];
   p[28]=temp[6];
   p[29]=temp[14];
   p[30]=temp[7];
   p[31]=temp[15];
}
*/

/****************************************************************
函数名：LCD写1个8*16 ASCII字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示240/8个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和CS_TFT_H();为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteEN(u16 x, u16 y, u16 CharColor, u8 ASCIICode)
{
 u8* pbuf;
 u8 buf[16];
 
 u16 ix, iy;
   
   //GetASCIICode(buf, ASCIICode);//取这个字符的显示代码
   GetASCIICode(buf, ASCIICode, 16);//取这个字符的显示代码
   pbuf = buf;
   
   for(iy=0; iy<16; iy++)//写16行
   {
      for(ix=0; ix<8; ix++)//写1行中的8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix, y, CharColor);
         }
      }
      pbuf++;
      y++;
   }
}

void LCDWriteEN16(u16 x, u16 y, u8 ASCIICode, u16 CharColor, u16 bkColor)
{
 u8* pbuf;
 u8 buf[16];
 
 u16 ix, iy;
   
   //GetASCIICode(buf, ASCIICode);//取这个字符的显示代码
   GetASCIICode(buf, ASCIICode, 16);//取这个字符的显示代码
   pbuf = buf;
   
   for(iy=0; iy<16; iy++)//写16行
   {
      for(ix=0; ix<8; ix++)//写1行中的8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            DrawPixel(x+ix, y, bkColor);
         }
         else
         {
            DrawPixel(x+ix, y, CharColor);
         }
      }
      pbuf++;
      y++;
   }
}

/****************************************************************
函数名：LCD写1个16*24 ASCII字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示240/16个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和CS_TFT_H();为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteEN24(u16 x, u16 y, u16 CharColor, u8 ASCIICode)
{
 u8* pbuf;
 u8 buf[48];
 
 u16 ix, iy;
   
   //GetASCIICode(buf, ASCIICode);//取这个字符的显示代码
   GetASCIICode(buf, ASCIICode, 24);//取这个字符的显示代码
   pbuf = buf;
   
   for(iy=0; iy<24; iy++)//写24行
   {
      for(ix=0; ix<8; ix++)//写1行中的8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix, y, CharColor);
         }
      }
      
      pbuf++;
      
      for(ix=0; ix<8; ix++)//写1行中的右8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix+8, y, CharColor);
         }
      }
      
      pbuf++;
      y++;
   }
}

/****************************************************************
函数名：LCD写1个16*16 汉字字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示30个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和CS_TFT_H();为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteCN(u16 x, u16 y, u16 CharColor, u8 *p)
{
 u8* pbuf;
 u8 buf[32];
 
 u16 ix, iy;
   
   GetChineseCode(buf, p);//取这个字符的显示代码
   pbuf = buf;
   
   for(iy=0; iy<16; iy++)//写16行
   {
      for(ix=0; ix<8; ix++)//写1行中的左8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix, y, CharColor);
         }
      }
      
      pbuf++;
      
      for(ix=0; ix<8; ix++)//写1行中的右8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix+8, y, CharColor);
         }
      }
      
      pbuf++;
      y++;
   }
}

/****************************************************************
函数名：LCD写1个16*16 汉字字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示240/16个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和CS_TFT_H();为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
void LCDWriteCN24(u16 x, u16 y, u16 CharColor, u8 *p)
{
 u8* pbuf;
 u8 buf[72];
 
 u16 ix, iy;
   
   GetChineseCode24(buf, p);//取这个字符的显示代码
   //MODY24(buf);
   pbuf = buf;
   
   for(iy=0; iy<24; iy++)//写24行
   {
      for(ix=0; ix<8; ix++)//写1行中的左8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix, y, CharColor);
         }
      }
      
      pbuf++;
      for(ix=0; ix<8; ix++)//写1行中的中8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix+8, y, CharColor);
         }
      }
      
      pbuf++;
      for(ix=0; ix<8; ix++)//写1行中的右8列
      {
         if( isBit8(*pbuf, 7-ix)==0 )
         {
            ;
         }
         else
         {
            DrawPixel(x+ix+16, y, CharColor);
         }
      }
      
      pbuf++;
      y++;
   }
}


Rect RectText={0, 0, 240, 320};//文字显示区域

//设置文字显示区域
void SetRectText(u16 x, u16 y, u16 w, u16 h)
{
   RectText.x=x;
   RectText.y=y;
   RectText.w=w;
   RectText.h=h;
}

#define RECT_XMAX   (RectText.x+RectText.w)
#define RECT_YMAX   (RectText.y+RectText.h)
#define START_X     RectText.x
#define START_Y     RectText.y
/************************************************************
函数名：LCD写字符串函数8*16ASCII,16*16汉字
功能：向指定位置写入一个或多个字符，本函数带自动换行功能
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色
          *p 指向要写的字符串
返回值：无

增加显示区域判断，所以使用是应该先设置显示区域
*************************************************************/
void LCD_WriteString16(uint16 x, uint16 y, uint16 CharColor, char* p)
{
 u8 dat;
   
   for(;*p!=0;p++)
   {
      if(( (u8)*p&0x80)==0)//英文
      {
         if(*p=='\r')//回车
         {
            x=START_X;
            continue;
         }
         if(*p=='\n')//换行
         {
            y+=16;
            if(y > RECT_YMAX-16)//超过行显示
               y=START_Y;
            continue;
         }

         dat=*p-0x20;  
         
         if(x > RECT_XMAX-8)//不够写1个字符就移到下一行
         {
            x=START_X;
           
            y+=16;
            if(y > RECT_YMAX-16)
               y=START_Y;
         }
         LCDWriteEN(x, y, CharColor, dat);
         x+=8;
      }
      else//中文
      {
         if(x > RECT_XMAX-16)//不够写1个字符就移到下一行
         {
            x=START_X;
           
            y+=16;
            if(y > RECT_YMAX-16)
               y=START_Y;
         }
         LCDWriteCN(x, y, CharColor, (u8*)p);
         x+=16;
         p++;
      }
   }
}
//下面的是没有显示区域判断

//{
// u8 dat;
//   
//   for(;*p!=0;p++)
//   {
//      if((*p&0x80)==0)//英文
//      {
//         if(*p=='\r')//回车
//         {
//            x=0;
//            continue;
//         }
//         if(*p=='\n')//换行
//         {
//            y+=16;
//            if(y > YMAX-16)
//               y=0;
//            continue;
//         }
//
//         dat=*p-0x20;  
//         
//         if(x > XMAX-8)//不够写1个字符就移到下一行
//         {
//            x=0;
//           
//            y+=16;
//            if(y > YMAX-16)
//               y=0;
//         }
//         LCDWriteEN(x, y, CharColor, dat);
//         x+=8;
//      }
//      else//中文
//      {
//         if(x > XMAX-16)//不够写1个字符就移到下一行
//         {
//            x=0;
//           
//            y+=16;
//            if(y > YMAX-16)
//               y=0;
//         }
//         LCDWriteCN(x, y, CharColor, (u8*)p);
//         x+=16;
//         p++;
//      }
//      
//   }
//}

void LCD_WriteString(uint16 x, uint16 y, uint16 CharColor, char* p, uint8 font)
{
    if(font==16)
       LCD_WriteString16(x, y, CharColor, p);
    else //if(font==24)
       LCD_WriteString24(x, y, CharColor, p);
}

/************************************************************
函数名：LCD写字符串函数16*24ASCII,24*24汉字
功能：向指定位置写入一个或多个字符，本函数带自动换行功能
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色
          *p 指向要写的字符串
返回值：无
*************************************************************/
void LCD_WriteString24(uint16 x, uint16 y, uint16 CharColor, char* p)
{
 u8 dat;
   
   for(; *p!=0; p++)
   {
      if(( (u8)*p&0x80)==0)//英文
      {
         if(*p=='\r')//回车
         {
            x=START_X;
            continue;
         }
         else if(*p=='\n')//换行
         {
            y+=24;
            if(y > RECT_YMAX-24)
               y=START_Y;
            continue;
         }
         else if(*p=='\1')         //自定义图标.上
            dat='~'-' '+1;
         else if(*p=='\2')    //自定义图标.下
            dat='~'-' '+2;
         else if(*p=='\3')    //自定义图标.左
            dat='~'-' '+3;
         else if(*p=='\4')    //自定义图标.右
            dat='~'-' '+4;
//         else if(*p=='\\')    //自定义图标
//         {
//            p++;
//            if(*p=='U')         //自定义图标.上
//               dat='~'-' '+1;
//            else if(*p=='D')    //自定义图标.下
//               dat='~'-' '+2;
//            else if(*p=='L')    //自定义图标.左
//               dat='~'-' '+3;
//            else// if(*p=='R')    //自定义图标.右
//               dat='~'-' '+4;
//         }
         else //其它正常字符
            dat=*p-0x20;  
         
         if(x > RECT_XMAX-16)//不够写1个字符就移到下一行
         {
            x=START_X;
           
            y+=24;
            if(y > RECT_YMAX-24)
               y=START_Y;
         }
         LCDWriteEN24(x, y, CharColor, dat);
         x+=16;
      }
      else//中文
      {
         if(x > RECT_XMAX-24)//不够写1个字符就移到下一行
         {
            x=START_X;
           
            y+=24;
            if(y > RECT_YMAX-24)
               y=START_Y;
         }
         LCDWriteCN24(x, y, CharColor, (u8*)p);
         x+=24;
         p++;
      }
   }
}


//下面的是没有显示区域判断

//{
// u8 dat;
//   
//   for(;*p!=0;p++)
//   {
//      if((*p&0x80)==0)//英文
//      {
//         if(*p=='\r')//回车
//         {
//            x=0;
//            continue;
//         }
//         if(*p=='\n')//换行
//         {
//            y+=24;
//            if(y > YMAX-24)
//               y=0;
//            continue;
//         }
//
//         dat=*p-0x20;  
//         
//         if(x > XMAX-16)//不够写1个字符就移到下一行
//         {
//            x=0;
//           
//            y+=24;
//            if(y > YMAX-24)
//               y=0;
//         }
//         LCDWriteEN24(x, y, CharColor, dat);
//         x+=16;
//      }
//      else//中文
//      {
//         if(x > XMAX-24)//不够写1个字符就移到下一行
//         {
//            x=0;
//           
//            y+=24;
//            if(y > YMAX-24)
//               y=0;
//         }
//         LCDWriteCN24(x, y, CharColor, (u8*)p);
//         x+=24;
//         p++;
//      }
//   }
//}

/*********************************************************
函数名：SPI取ASCII码子程序
输入参数：u8 ASCII 输入的ASCII码，如'A'
          BaseAddr 基址 即ASCII显示代码在FLASH中的启示位置
返回值：无
说明：输入一个ASCII码，取得它在SPI FLASH中的16Byte显示代码
并将其存放到一个16byte的ASCII显示缓冲CharBuffer[]中
**********************************************************/
//void GetASCIICode(u8* buf, u8 ASCII)
void GetASCIICode(u8* buf, u8 ASCII, u8 dot)
{uint32 address;
 u8 len;
   
   //address=0x3fe40+16UL*ASCII;
   if(dot==16)
      address=0x40000+16UL*ASCII, len=16;
   else if(dot==24)
      address=0xBB000+48UL*ASCII, len=48;
   else
      address=0, len=0;
      
   Read_Flash_Page ( address, len, buf );
}

/*********************************************************
函数名：SPI中文显示码子程序
输入参数：u16 ASCII 输入的中文，如"我"
          BaseAddr 基地 即显示代码在FLASH中的起始位置
返回值：无
说明：输入一个中文，取得它在SPI FLASH中的32Byte显示代码
并将其存放到一个32byte的显示缓冲ChineseBuffer[]
**********************************************************/
void GetChineseCode(u8* dot, u8 *p)
{//uchar m;
 //uint8 dot[32];
 uint32 address;

   //address=(uint32)32*( ((*p)-0xa1)*94 + ((*(p+1))-0xa1) )+4096;
   address=32UL*( ((*p)-0xa1)*94 + ((*(p+1))-0xa1) );
   //address+=HZK_FlashAddress;
   Read_Flash_Page ( address, 32, dot );
}

/*********************************************************
函数名：SPI中文显示码子程序
输入参数：u16 ASCII 输入的中文，如"我"
          BaseAddr 基地 即显示代码在FLASH中的起始位置
返回值：无
说明：输入一个中文，取得它在SPI FLASH中的32Byte显示代码
并将其存放到一个32byte的显示缓冲ChineseBuffer[]
**********************************************************/
void GetChineseCode24(u8* dot, u8 *p)
{//uchar m;
 //uint8 dot[32];
 uint32 address;

   //address=(uint32)32*( ((*p)-0xa1)*94 + ((*(p+1))-0xa1) )+4096;
   address=72UL*( ((*p)-15-0xa1)*94 + ((*(p+1))-0xa1) );
   address+=0x41000;
   Read_Flash_Page ( address, 72, dot );
}

void Delay_nms(int n)
{
   u32 f = n, k;
   for (; f != 0; f--)
   {
      for (k = 0xFFF; k != 0; k--)
         ;
   }
}


