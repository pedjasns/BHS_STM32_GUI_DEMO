/********************************************************************/
#include "..\APP\includes.h"
//#include "include.h"
//��������SST25LF080A��SST25VF080A�������������߸����ֽڱ�̲����ݣ�����
//������ͨ�ֽڱ�̣����ֽڱ��Ч�ʽ���΢��Щ��ʵ�ʲ�������
//sbit	CE_SPI          =P1^3;   //SPI����
//#define LockSPI()    WP = 0  //д����
//#define UnLockSPI()  WP = 1  //�ر�д����

//#define EnableSPI()  CE_SPI=0  //��������
//#define DisableSPI() CE_SPI=1  //������ֹ

//#define EnableSPI()  SPI_FLASH_CS_LOW()   
//#define DisableSPI() SPI_FLASH_CS_HIGH()  

#define EnableSPI()    SPI_FLASH_CS_LOW()    //��������
#define DisableSPI()   SPI_FLASH_CS_HIGH()   //������ֹ



#define  EWSR_CMD           0x50     //ʹ��д״̬�Ĵ�������
#define  RDSR_CMD           0x05     //��״̬�Ĵ�������
#define  WRSR_CMD           0x01     //д״̬�Ĵ�������
#define  WREN_CMD           0x06     //дʹ������
#define  WRDI_CMD           0x04     //д��ֹ����
#define  READ_CMD           0x03     //����������
#define  HIGH_SPEED_READ    0x0B     //�������ݶ�(��ַ�Զ�����)����
#define  CHIP_ERASE_CMD     0x60     //оƬ��������
#define  BLOCK_ERASE_CMD    0x52     //���������
#define  SECTOR_ERASE_CMD   0x20     //������������
#define  AAI_PROGRAM_CMD    0xAF     //SST25LF080A�ֽڱ��(��ַ�Զ�����)����
//#define  AAI_PROGRAM_CMD    0xAD     //SST25VF080A�ֽڱ��(��ַ�Զ�����)����
#define  BYTE_PROGRAM_CMD   0x02     //�ֽڱ������  
#define  READ_ID            0x90     //������ID
#define  READ_ID2           0xAB     //������ID(������ȫͬREAD_ID)



//SPI���͵�3�ֽڵ�ַ
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

//����FLASH�洢����־
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
* ��SST25LF080 ״̬�Ĵ���     
* Input:                
* None
* Returns:                
״̬              
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
ʹ��д״̬�Ĵ���
************************************************************************/
static void EnableWRReg(void)
{
  EnableSPI();
  
  SPI_Send_Byte( EWSR_CMD ); //0x50 enable writing to the status register
  
  DisableSPI();
}

/************************************************************************
WriteSReg
д1byte��״̬�Ĵ���
dat--����
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
дʹ��
************************************************************************/
static void EnableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WREN_CMD ); //0x06 send WREN command
  
  DisableSPI();
}

/************************************************************************
*DisableWrite()
д��ֹ
************************************************************************/
static void DisableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WRDI_CMD );    //0x04 send WRDI command
  
  DisableSPI();
}

//������
void UnProtectSST25VF080(void)
{
   EnableWRReg();
   WriteSReg(0);
}


/************************************************************************
Read_ID
������/����ID
Input--ID_addr(A7~A1 =0)
     :Manufacturer��s ID is read with A0=0
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
 ��1byte����
 Input--��ַ(3byte)
   Dst:  Destination Address 000000H - 0FFFFFH
 Returns:--����
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
����������
Input:
  Address--����ַ 000000H - 0FFFFFH
  Buf--�������ݻ��� 
  n--������ (max = 255)
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
���ٶ�1byte����
Address--��ַ
����--����
************************************************************************/
/*
uint8 HighSpeed_Read_Byte( uint32 Address )
{uint8 dat = 0;

  EnableSPI();
  
  SPI_Send_Byte( HIGH_SPEED_READ );      //0x0B read command
  SPI_Send32Address(Address); // send 3 address bytes
  SPI_Send_Byte( 0xFF );      //dummy byte,����������������
  dat = SPI_Send_Byte(0xff);
  
  DisableSPI();
  return dat;
}
*/

/************************************************************************
HighSpeed_Read_MultiByte
��������������
Address--��ַ
Buf--����
n--������
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
�ֽڱ��
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
  EnableWrite();                //дʹ��
  
  EnableSPI();
  SPI_Send_Byte( BYTE_PROGRAM_CMD );      //0x02 send Byte Program command
  SPI_Send32Address(Address); // send 3 address bytes
  SPI_Send_Byte( dat );       // send byte to be programmed
  DisableSPI();
  
  Wait_Busy();
  
}
//*/

/************************************************************************
��̶���ֽڣ�ʹ�õ�ַ�Զ���������         
Address--��ַ
Buf--����
n--������
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
   EnableWrite();              //дʹ��

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

   //DisableWrite(),Read_Status_Register()��ʾ��ַ�Զ����ӱ���������
   DisableWrite();
   Read_Status_Register();
   */
}


/************************************************************************
Chip_Erase
оƬ����
************************************************************************/
/*
void Chip_Erase(void)
{
  EnableWrite();         //дʹ��
  
  EnableSPI();
  SPI_Send_Byte( CHIP_ERASE_CMD );      //0x60 send Block Erase command
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
Sector_Erase
��������(1������4Kbyte)
Input:
Dst:    Destination Address 000000H - 0FFFFFH
************************************************************************/
uint8 Sector_Erase( uint32 Address )
{
  EnableWrite();              //дʹ��

  EnableSPI();
  SPI_Send_Byte( SECTOR_ERASE_CMD );      //0x20 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
  return(0);
}

/************************************************************************
Block_Erase
�����(1����64Kbyte)
 Input:
   Dst:    Destination Address 000000H - 0FFFFFH

************************************************************************/
/*
void Block_Erase( uint32 Address )
{
  
  EnableWrite();              //дʹ��
  
  EnableSPI();
  SPI_Send_Byte( BLOCK_ERASE_CMD );      //0x52 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
* ����ԭ��:Wait_Busy()
* ��������:�ж�æ״̬
* �������:
* �������:0:����, ��0��æ����ʱʧ���Ƴ�
* ����˵��:�����¹���ʹ�ã�
  ��Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
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
stm32 72MHz�µ�ʱ�䳣��
ʱ�䳣����Read_Status_Register()����
100   = 410us
1000  = 4.09ms
10000 = 40.1ms
20000 = 80.2ms
*/
{uint32 counter;
  
  counter=0;
  while ((Read_Status_Register() & 0x03) == 0x03)
  {
     if( ++counter > (20000) )//Լ80ms
        break;
  }
  return(0);
}

/************************************************************************
* ����ԭ��:Wait_Busy_AAI()
* ��������:�ж�æ״̬
* �������:
* �������:0:����, ��0��æ����ʱʧ���Ƴ�
* ����˵��:�����¹���ʹ�ã�
  ��Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
************************************************************************/
/*
static uint8 Wait_Busy_AAI(void)
{uint8 time;
  
  time=10;
  
  while ((Read_Status_Register() & 0x43) == 0x43)
  {
    if( (time=Timer_Over_Ms(time))==0 )
      return(1);   //��ʱ�˳�
  }
  return(0);
}
*/

/************************************************************************
Verify
У��
Address--��ַ
Buf--����
n--������
���أ�0����ͬ��1������ͬ
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
��̺�У��
Address--��ַ
Buf--����
n--������
���أ�0���ɹ���1��ʧ��
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


//���д����������ͷ�Զ�����������
uint8 Write_Flash_Page_FirstErase( uint32 Address, uint16 n, uint8 *Buf )
{//uint8  status;
   
   if((Address%FLASH_SECTOR_SIZE)==0)//���ƶ�������ͷ������������
      Erase_Flash_Sector( Address );
     
   return( Write_Flash_Page( Address, n, Buf ) );
}

/*
дflash, ����д���Ȳ�����д

ע�⣺���ܿ�����д
      ����ܳ�����������ֽ���
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
//       if(status==0)//����ֱ��д��
//       {
//          status=Write_Flash_Page( Address, n, Buf );
//       }
//       else//���������д
//       {
//          Read_Flash_Page(Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp);//�������׵�ַ��1����
//          memcpy(temp+(Address&FLASH_SECTOR_MASK), Buf, n);//�������ݵ�������ַ
//          Erase_Flash_Sector( Address );
//          status=Write_Flash_Page( Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp );//д��������
//       }
//    }
//    return(status);
//}


/*
�������ݲ��ò������Ƿ����д��flash, flash��ԭ����1->0����д�룬ԭ����0->1������д��
���룺WriteInfo--д����Ϣ��FlashInfo--Flash�е�����
      1001_0000   1111_0100, ����д��
���أ�
    0=����д��
    1=������д��
    
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

