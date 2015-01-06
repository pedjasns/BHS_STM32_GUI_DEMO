#include "..\APP\includes.h"

// A/D channel selection command words and working registers
#define CHX 0x90 // X + channel selection control word
#define CHY 0xd0 // Y + channel selection control word

// # define CHX 0xd0 // X + channel selection control word
// # define CHY 0x90 // Y + channel selection control word

MATRIX Matrix; // In order to avoid duplication of calibration, the calibration values can be saved

//====================================================================================
static void Delayus( int k)
{
 int j;
    
    for(j=k;j > 0;j--);    
}

//====================================================================================
void TP_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 // Push-pull output
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

//====================================================================================
static void WR_CMD (u8 cmd) 
{
 //u8 buf;
 u8 i;
    
    TP_CS_H(); //TP_CS(1);
    TP_DIN_L(); //TP_DIN(0);
    TP_DCLK_L(); //TP_DCLK(0);
    TP_CS_L(); //TP_CS(0);
    for(i=0;i<8;i++) 
    {
        //buf=(cmd>>(7-i))&0x1;
        //TP_DIN(buf);
        if( isBit8(cmd, 7-i) )
           TP_DIN_H();
        else
           TP_DIN_L();
        Delayus(5);
        TP_DCLK_H(); //TP_DCLK(1);
        Delayus(5);
        TP_DCLK_L(); //TP_DCLK(0);
    }
}

//====================================================================================
static u16 RD_AD(void) 
{
 u16 buf=0;
 u8 i;
    
    TP_DIN_L(); //TP_DIN(0);
    TP_DCLK_H(); //TP_DCLK(1);
    for(i=0; i<16; i++) 
    {
        Delayus(5);
        TP_DCLK_L(); //TP_DCLK(0);         
        Delayus(5);
        
        buf <<= 1;
        if(TP_DOUT)
           buf |= 1;
        
        Delayus(5);
        TP_DCLK_H(); //TP_DCLK(1);
    }
    TP_CS_H(); //TP_CS(1);
    buf>>=4;//只有12位有效
    buf&=0x0fff;
    return(buf);
}


#define ReadLoop 13 //Have to > 2
#define LOSS_DATA 5 // The number of lost data before and after
u16 Read_XY(u8 xy) 
{ 
 u16 i, j;
 u16 buf[ReadLoop];
 uint32 sum;
 u16 val;
    
    for(i=0; i<ReadLoop; i++)
    {
       WR_CMD(xy);
       //while(TP_BUSY);
       Delayus(5);
       buf[i]=RD_AD();
       
       //sum += buf[i];
    }
    
    // Sequence
    for(i=0; i<ReadLoop-1; i++)
    {
       for(j=i+1; j<ReadLoop; j++)
       {
          if(buf[i]>buf[j])
          {
             val=buf[i];
             buf[i]=buf[j];
             buf[j]=val;
          }
       }
    }
    
    sum=0;
    for(i=LOSS_DATA; i<ReadLoop-1-LOSS_DATA; i++)
       sum += buf[i];
    val=sum/(ReadLoop-2*LOSS_DATA);
    
    return (val);    
}

//====================================================================================
uint8 TP_GetAdXY(u16 *x, u16 *y) 
{
 //u16 adx,ady;
    
    *y=Read_XY(CHX);
    *x=Read_XY(CHY);
    //*x=adx;
    //*y=ady;
    if(*x<100 || *y<100)
       return(0);
    else
       return(1);
}

/*
Function: Read touchscreen read coordinate that without making the conversion, you can not directly use

Returns: 0 = Invalid coordinates
       1 = active coordinate

Description: This function is continuously sampled twice, twice the sampling results to be valid within + -5 range
*/
uint8 TP_GetAdXY2(u16 *x, u16 *y, uint32 delay) 
{u16 x1,y1;
 u16 x2,y2;
 u8 flag;
 
    flag=TP_GetAdXY(&x1, &y1);
    
    if(flag==0)
       return(0);
    
//    if(delay>=OS_TIME)
//       os_dly_wait(delay/OS_TIME);//300ms After sampling 1
    
    //do{
       flag=TP_GetAdXY(&x2, &y2);
    //}while(flag);
    
    if(flag==0)
       return(0);
    
    if( ( (x2<=x1 && x1<x2+50) || (x1<=x2 && x2<x1+50) ) // Twice before and after sampling within + -5
     && ( (y2<=y1 && y1<y2+50) || (y1<=y2 && y2<y1+50) ) )
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return(1);
    }
    else
       return(0);
}

///*
// Function: Reads the coordinates of the LCD touch screen, the coordinates have been converted into usable view coordinates
// Returns: 0 = not display area
// 1 = display region
//*/
//uint8 TP_GetLCDXY(u16 *x, u16 *y) 
//{POINT   displayPoint;
// POINT   TouchSample;
// uint16 xt,yt;	
// uint8 flag;
// 
//    flag=TP_GetAdXY2(&xt, &yt, 0);
//    if(flag)
//    {
//       TouchSample.x=xt/4; TouchSample.y=yt/4;
//       getDisplayPoint( &displayPoint, &TouchSample, &Matrix );
//       
//       if(IsDisplayArea(&displayPoint)) // Determines whether the display area
//       {
//          flag=1;
//          *x=displayPoint.x;
//          *y=displayPoint.y;
//       }
//       else
//          flag=0;
//    }
//    
//    return(flag);
//}
