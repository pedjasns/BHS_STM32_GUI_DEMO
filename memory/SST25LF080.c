/********************************************************************/
#include "..\APP\includes.h"
//#include "include.h"
//本程序是SST25LF080A，SST25VF080A驱动，由于两者高速字节编程不兼容，所以
//采用普通字节编程，多字节编程效率将略微低些，实际并不明显
//sbit	CE_SPI          =P1^3;   //SPI控制
//#define LockSPI()    WP = 0  //写保护
//#define UnLockSPI()  WP = 1  //关闭写保护

//#define EnableSPI()  CE_SPI=0  //操作允许
//#define DisableSPI() CE_SPI=1  //操作禁止

//#define EnableSPI()  SPI_FLASH_CS_LOW()   
//#define DisableSPI() SPI_FLASH_CS_HIGH()  

#define EnableSPI()    SPI_FLASH_CS_LOW()    //操作允许
#define DisableSPI()   SPI_FLASH_CS_HIGH()   //操作禁止



#define  EWSR_CMD           0x50     //使能写状态寄存器命令
#define  RDSR_CMD           0x05     //读状态寄存器命令
#define  WRSR_CMD           0x01     //写状态寄存器命令
#define  WREN_CMD           0x06     //写使能命令
#define  WRDI_CMD           0x04     //写禁止命令
#define  READ_CMD           0x03     //读数据命令
#define  HIGH_SPEED_READ    0x0B     //高速数据读(地址自动增加)命令
#define  CHIP_ERASE_CMD     0x60     //芯片擦除命令
#define  BLOCK_ERASE_CMD    0x52     //块擦除命令
#define  SECTOR_ERASE_CMD   0x20     //扇区擦除命令
#define  AAI_PROGRAM_CMD    0xAF     //SST25LF080A字节编程(地址自动增加)命令
//#define  AAI_PROGRAM_CMD    0xAD     //SST25VF080A字节编程(地址自动增加)命令
#define  BYTE_PROGRAM_CMD   0x02     //字节编程命令  
#define  READ_ID            0x90     //读器件ID
#define  READ_ID2           0xAB     //读器件ID(功能完全同READ_ID)



//SPI发送低3字节地址
//#define  SPI_Send32Address(Address) {    \
//SPI_Send_Byte(HL_ByteOfLong(Address));   \
//SPI_Send_Byte(LH_ByteOfLong(Address));   \
//SPI_Send_Byte(LL_ByteOfLong(Address));   \
//}
#define  SPI_Send32Address(Address) {    \
SPI_Send_Byte((uint8)(Address>>16));     \
SPI_Send_Byte((uint8)((Address>>8)&0xff));   \
SPI_Send_Byte((uint8)(Address&0xff));   \
}

//定义FLASH存储器标志
uint8  FLASH_FLAG;



uint8 HighSpeed_Read_Byte( uint32 Address );


void Chip_Erase(void);

void Block_Erase( uint32 Address );

static uint8 Wait_Busy(void);

//static uint8 Wait_Busy_AAI(void);



/*
void SPI_Init()
{
	//P1=0xff;
	SPSR = SPSR & 0x7F;
	SPCR = 0x50;
}


uint8 SPI_Send_Byte(uint8 dat)
{//uint8 temp;
	
	SPDR = dat;
	//do
	//{
	//	temp = SPSR & 0x80;
	//}while (temp != 0x80);
	
	while((SPSR&0x80)^0x80);
	
	SPSR = SPSR & 0x7F;
	
	//BUZZ_ON();
	return SPDR;
}
*/


/************************************************************************
* Read_Status_Register
* 读SST25LF080 状态寄存器     
* Input:                
* None
* Returns:                
状态              
************************************************************************/
static uint8 Read_Status_Register(void)
{uint8 dat;
  EnableSPI();
  
  SPI_Send_Byte( RDSR_CMD );  // 0x05 send RDSR command
  dat = SPI_Send_Byte(0xff);       // receive byte
  
  DisableSPI();
  return dat;
}

/************************************************************************
EnableWRSReg
使能写状态寄存器
************************************************************************/
static void EnableWRReg(void)
{
  EnableSPI();
  
  SPI_Send_Byte( EWSR_CMD ); //0x50 enable writing to the status register
  
  DisableSPI();
}

/************************************************************************
WriteSReg
写1byte到状态寄存器
dat--数据
************************************************************************/
static void WriteSReg( uint8 dat )
{
  EnableSPI();
  
  SPI_Send_Byte( WRSR_CMD );  //0x01 select write to status register
  SPI_Send_Byte( dat );   //data that will change the status of BPx 
                          //or BPL (only bits 2,3,7 can be written)
  DisableSPI();
  
  Wait_Busy();
}

/************************************************************************
EnableWrite
写使能
************************************************************************/
static void EnableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WREN_CMD ); //0x06 send WREN command
  
  DisableSPI();
}

/************************************************************************
*DisableWrite()
写禁止
************************************************************************/
static void DisableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WRDI_CMD );    //0x04 send WRDI command
  
  DisableSPI();
}

//开保护
void UnProtectSST25VF080(void)
{
   EnableWRReg();
   WriteSReg(0);
}


/************************************************************************
Read_ID
读厂商/器件ID
Input--ID_addr(A7~A1 =0)
     :Manufacturer’s ID is read with A0=0
     :Device ID is read with A0=1.
Returns--dat
       :ID1(Manufacture's ID = BFh
       :Device ID = 80h(SST25LF080))
************************************************************************/
uint8 ReadDeviceID(void)//( uint8 ID_addr )
{uint8 dat;
  EnableSPI();
  
  SPI_Send_Byte( READ_ID );    //0x90 send read ID command (90h or ABh)
  SPI_Send_Byte( 0x00 );    // send address
  SPI_Send_Byte( 0x00 );    // send address
  SPI_Send_Byte( 1 ); // send address - either 00H or 01H
  dat = SPI_Send_Byte(0xff);         // receive byte
  
  DisableSPI();
  return dat;
}


/************************************************************************
 PROCEDURE: Read
 读1byte数据
 Input--地址(3byte)
   Dst:  Destination Address 000000H - 0FFFFFH
 Returns:--数据
************************************************************************/
/*
static uint8 Read_Byte( uint32 Address )
{uint8 dat; 
  EnableSPI();
  
  SPI_Send_Byte( READ_CMD );      //0x03 read command 
  SPI_Send32Address(Address); // send 3 address bytes
  dat = SPI_Send_Byte(0xff);
  
  DisableSPI();
  return dat;                 // return one byte read
}
*/

/************************************************************************
Read_Cont
连续读数据
Input:
  Address--读地址 000000H - 0FFFFFH
  Buf--读出数据缓冲 
  n--读数量 (max = 255)
************************************************************************/
/*
static uint8 Read_MultiByte( uint32 Address, uint8 n, uint8 *Buf )
{uint8 i;

  EnableSPI();
  
  SPI_Send_Byte( READ_CMD );      //0x03 read command
  SPI_Send32Address(Address); // send 3 address bytes
  for ( i = 0; i < n; i++ )   // read until no_bytes is reached
  {
    Buf[i] = SPI_Send_Byte(0xff);      // receive byte and store at address 80H - FFH
  }
  
  DisableSPI();
  
  return(0);
}
*/

/************************************************************************
HighSpeed_Read
高速读1byte数据
Address--地址
返回--数据
************************************************************************/
/*
uint8 HighSpeed_Read_Byte( uint32 Address )
{uint8 dat = 0;

  EnableSPI();
  
  SPI_Send_Byte( HIGH_SPEED_READ );      //0x0B read command
  SPI_Send32Address(Address); // send 3 address bytes
  SPI_Send_Byte( 0xFF );      //dummy byte,发送任意数都可以
  dat = SPI_Send_Byte(0xff);
  
  DisableSPI();
  return dat;
}
*/

/************************************************************************
HighSpeed_Read_MultiByte
高速连续读数据
Address--地址
Buf--数据
n--数据量
************************************************************************/
uint8 HighSpeed_Read_MultiByte( uint32 Address, uint16 n, uint8 *Buf )
{uint16 i;
  
  EnableSPI();
  
  SPI_Send_Byte( HIGH_SPEED_READ );       //0x0B read command
  SPI_Send32Address(Address);  // send 3 address bytes
  SPI_Send_Byte( 0xFF );       //dummy byte
  for ( i = 0; i < n; i++ )    // read until no_bytes is reached
  {
    Buf[i] = SPI_Send_Byte(0xff);       // receive byte and store at address 80H - FFH
  }
  
  DisableSPI();
  
  return(0);
}


/************************************************************************
Byte_Program 
字节编程
 This procedure programs one address of the device.
 Assumption:  Address being programmed is already erased and is NOT 
    block protected.

 Input:
    Dst:    Destination Address 000000H - 0FFFFFH
    byte:   byte to be programmed
************************************************************************/
//*
static void Program_Byte( uint32 Address, uint8 dat )
{
  EnableWrite();                //写使能
  
  EnableSPI();
  SPI_Send_Byte( BYTE_PROGRAM_CMD );      //0x02 send Byte Program command
  SPI_Send32Address(Address); // send 3 address bytes
  SPI_Send_Byte( dat );       // send byte to be programmed
  DisableSPI();
  
  Wait_Busy();
  
}
//*/

/************************************************************************
编程多个字节，使用地址自动增加命令         
Address--地址
Buf--数据
n--数据量
************************************************************************/
static void Program_MultiByte( uint32 Address, uint16 n, uint8 *Buf )
{uint16 i;
   
   if(n==0) return;
   
   for(i=0; i<n; i++)
   {
     Program_Byte( Address+i,  Buf[i]);
   }
   DisableWrite();
   /*
   EnableWrite();              //写使能

   EnableSPI();
   SPI_Send_Byte( AAI_PROGRAM_CMD );      //0xAF send AAI command 
   SPI_Send32Address(Address); // send 3 address bytes
   SPI_Send_Byte( Buf[0] );    // send byte to be programmed
   DisableSPI();
   Wait_Busy_AAI();
   
   for(i=1; i<n; i++)
   {
     
     EnableSPI();
     SPI_Send_Byte( AAI_PROGRAM_CMD );    //0xAF send AAI command
     SPI_Send_Byte( Buf[i] );  // send byte to be programmed
     DisableSPI();
     
     Wait_Busy_AAI(); 
   }

   //DisableWrite(),Read_Status_Register()表示地址自动增加编程命令结束
   DisableWrite();
   Read_Status_Register();
   */
}


/************************************************************************
Chip_Erase
芯片擦除
************************************************************************/
/*
void Chip_Erase(void)
{
  EnableWrite();         //写使能
  
  EnableSPI();
  SPI_Send_Byte( CHIP_ERASE_CMD );      //0x60 send Block Erase command
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
Sector_Erase
扇区擦除(1个扇区4Kbyte)
Input:
Dst:    Destination Address 000000H - 0FFFFFH
************************************************************************/
uint8 Sector_Erase( uint32 Address )
{
  EnableWrite();              //写使能

  EnableSPI();
  SPI_Send_Byte( SECTOR_ERASE_CMD );      //0x20 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
  return(0);
}

/************************************************************************
Block_Erase
块擦除(1个块64Kbyte)
 Input:
   Dst:    Destination Address 000000H - 0FFFFFH

************************************************************************/
/*
void Block_Erase( uint32 Address )
{
  
  EnableWrite();              //写使能
  
  EnableSPI();
  SPI_Send_Byte( BLOCK_ERASE_CMD );      //0x52 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
* 函数原型:Wait_Busy()
* 函数功能:判断忙状态
* 输入参数:
* 输出参数:0:空闲, 非0：忙，超时失败推出
* 函数说明:被以下功能使用：
  （Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
************************************************************************/
static uint8 Wait_Busy(void)
//{uint32 counter;
//  
//  counter=gTimer_1ms;
//  while ((Read_Status_Register() & 0x03) == 0x03)
//  {
//     if( gTimer_1ms-counter > (100) )//100000us=100ms
//        break;
//  }
//  return(0);
//}

/*
stm32 72MHz下的时间常数
时间常数由Read_Status_Register()决定
100   = 410us
1000  = 4.09ms
10000 = 40.1ms
20000 = 80.2ms
*/
{uint32 counter;
  
  counter=0;
  while ((Read_Status_Register() & 0x03) == 0x03)
  {
     if( ++counter > (20000) )//约80ms
        break;
  }
  return(0);
}

/************************************************************************
* 函数原型:Wait_Busy_AAI()
* 函数功能:判断忙状态
* 输入参数:
* 输出参数:0:空闲, 非0：忙，超时失败推出
* 函数说明:被以下功能使用：
  （Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
************************************************************************/
/*
static uint8 Wait_Busy_AAI(void)
{uint8 time;
  
  time=10;
  
  while ((Read_Status_Register() & 0x43) == 0x43)
  {
    if( (time=Timer_Over_Ms(time))==0 )
      return(1);   //超时退出
  }
  return(0);
}
*/

/************************************************************************
Verify
校验
Address--地址
Buf--数据
n--数据量
返回：0：相同，1：不相同
************************************************************************/
static uint8 Verify( uint32 Address, uint16 n, uint8 *Buf )
{uint16 i;
 uint8 temp;
	
	EnableSPI();
  
  SPI_Send_Byte( HIGH_SPEED_READ );       //0x0B read command
  SPI_Send32Address(Address);  // send 3 address bytes
  SPI_Send_Byte( 0xFF );       //dummy byte
  for ( i = 0; i < n; i++ )    // read until no_bytes is reached
  {
    temp = SPI_Send_Byte(0xff);       // receive byte and store at address 80H - FFH
    if(Buf[i] != temp)
      return(1);
  }
  
  DisableSPI();
  
  return(0);
}

/************************************************************************
ProgramVerify_MultiByteApp
编程后校验
Address--地址
Buf--数据
n--数据量
返回：0：成功，1：失败
************************************************************************/
uint8 ProgramVerify_MultiByte( uint32 Address, uint16 n, uint8 *Buf )
{uint16 i;
   
   for(i=0; i<3; i++)
   {
     Program_MultiByte( Address, n, Buf );
     if( Verify( Address, n, Buf )==0 )
       return(0);
   }
   return(1);
}


//如果写数据是扇区头自动擦除该扇区
uint8 Write_Flash_Page_FirstErase( uint32 Address, uint16 n, uint8 *Buf )
{//uint8  status;
   
   if((Address%FLASH_SECTOR_SIZE)==0)//当移动到扇区头，擦除该扇区
      Erase_Flash_Sector( Address );
     
   return( Write_Flash_Page( Address, n, Buf ) );
}

/*
写flash, 不能写就先擦除再写

注意：不能跨扇区写
      最大不能超过扇区最大字节数
*/
//uint8 Force_Write_Flash_Page( uint32 Address, uint16 n, uint8 *Buf )
//{//uint8 temp[FLASH_SECTOR_SIZE];
// uint8* temp=gBigBuf;
// uint8 status;
//    
//    status =Read_Flash_Page(Address, n, temp);
//    if(status==0)
//    {
//       status=testFlashWrite( Buf, temp, n);
//       if(status==0)//可以直接写入
//       {
//          status=Write_Flash_Page( Address, n, Buf );
//       }
//       else//必须擦除再写
//       {
//          Read_Flash_Page(Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp);//从扇区首地址读1扇区
//          memcpy(temp+(Address&FLASH_SECTOR_MASK), Buf, n);//复制数据到扇区地址
//          Erase_Flash_Sector( Address );
//          status=Write_Flash_Page( Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp );//写整个扇区
//       }
//    }
//    return(status);
//}


/*
测试数据不用擦除都是否可以写入flash, flash中原来是1->0可以写入，原来是0->1不可以写入
输入：WriteInfo--写入信息，FlashInfo--Flash中得数据
      1001_0000   1111_0100, 可以写入
返回：
    0=可以写入
    1=不可以写入
    
*/
uint8 testFlashWrite( uint8 *WriteInfo, uint8 *FlashInfo, uint8 cmplen)
{
    while( (cmplen--)>0 )
    {
       if( ((*WriteInfo)&(*FlashInfo)) != (*WriteInfo) )
          return(1);
       
       WriteInfo++;FlashInfo++;
    }
    return(0);
}

