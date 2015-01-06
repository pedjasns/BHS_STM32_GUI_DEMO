/********************************************************************/
#include "..\APP\includes.h"
//#include "include.h"
// This program is SST25LF080A, SST25VF080A drive, due to incompatibility 
// between the two high-speed byte programming, so ordinary byte programming, 
// multi-byte programming efficiency is slightly lower, actually is not obvious

//sbit	CE_SPI          =P1^3;   //SPI Control
//#define LockSPI()    WP = 0  // Write-protected
//#define UnLockSPI()  WP = 1  // Write protection off

//#define EnableSPI()  CE_SPI=0  // Operation enable
//#define DisableSPI() CE_SPI=1  // Operating ban

//#define EnableSPI()  SPI_FLASH_CS_LOW()   
//#define DisableSPI() SPI_FLASH_CS_HIGH()  

#define EnableSPI()    SPI_FLASH_CS_LOW()    // Operating enabled
#define DisableSPI()   SPI_FLASH_CS_HIGH()   // Operation disabled

#define  EWSR_CMD           0x50     // Enable write status register command
#define  RDSR_CMD           0x05     // Read Status Register command
#define  WRSR_CMD           0x01     // Write Status Register command
#define  WREN_CMD           0x06     // Write Enable command
#define  WRDI_CMD           0x04     // Write Disable command
#define  READ_CMD           0x03     // Read Data command
#define  HIGH_SPEED_READ    0x0B     // High-speed data read (address automatically increase) command
#define  CHIP_ERASE_CMD     0x60     // Chip Erase command
#define  BLOCK_ERASE_CMD    0x52     // Block Erase command
#define  SECTOR_ERASE_CMD   0x20     // Sector Erase command
#define  AAI_PROGRAM_CMD    0xAF     // SST25LF080A Byte programming (address automatically increase) command
//#define  AAI_PROGRAM_CMD    0xAD    //SST25VF080A Byte programming (address automatically increase) command
#define  BYTE_PROGRAM_CMD   0x02     // Byte programming command
#define  READ_ID            0x90     // Reading Device ID
#define  READ_ID2           0xAB     // Reading Device ID (fully functional with READ_ID)

//SPI Sending a low 3-byte address
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

// FLASH memory flag definition
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
* Reading SST25LF080 status register    
* Input:                
* None
* Returns: Status           
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
Enable write status register
************************************************************************/
static void EnableWRReg(void)
{
  EnableSPI();
  
  SPI_Send_Byte( EWSR_CMD ); //0x50 enable writing to the status register
  
  DisableSPI();
}

/************************************************************************
WriteSReg
Write to the status register 1byte
dat--Data
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
Write Enable
************************************************************************/
static void EnableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WREN_CMD ); //0x06 send WREN command
  
  DisableSPI();
}

/************************************************************************
*DisableWrite()
Write Disable
************************************************************************/
static void DisableWrite()
{
  EnableSPI();
  
  SPI_Send_Byte( WRDI_CMD );    //0x04 send WRDI command
  
  DisableSPI();
}

// On Protects
void UnProtectSST25VF080(void)
{
   EnableWRReg();
   WriteSReg(0);
}


/************************************************************************
Read_ID
Read vendor / device ID
Input--ID_addr(A7~A1 =0)
     :Manufacturer¡¯s ID is read with A0=0
     :Device ID is read with A0=1.
Returns--dat
       :ID1(Manufacture's ID = BFh
       :Device ID = 80h(SST25LF080))
************************************************************************/
uint8 ReadDeviceID(void)//( uint8 ID_addr )
{uint8 dat;
  EnableSPI();
  
  SPI_Send_Byte( READ_ID ); 	// 0x90 send read ID command (90h or ABh)
  SPI_Send_Byte( 0x00 );    	// send address
  SPI_Send_Byte( 0x00 );    	// send address
  SPI_Send_Byte( 1 ); 			// send address - either 00H or 01H
  dat = SPI_Send_Byte(0xff);    // receive byte
  
  DisableSPI();
  return dat;
}


/************************************************************************
 PROCEDURE: Read
 Read 1byte Data
 Input--Address (3byte)
   Dst:  Destination Address 000000H - 0FFFFFH
 Returns:--Data
************************************************************************/
/*
static uint8 Read_Byte( uint32 Address )
{uint8 dat; 
  EnableSPI();
  
  SPI_Send_Byte( READ_CMD );  // 0x03 read command 
  SPI_Send32Address(Address); // send 3 address bytes
  dat = SPI_Send_Byte(0xff);
  
  DisableSPI();
  return dat;                 // return one byte read
}
*/

/************************************************************************
Read_Cont
Sequential read data
Input:
  Address--Read address 000000H - 0FFFFFH
  Buf--Read data buffer
  n--Read the number (max = 255)
************************************************************************/
/*
static uint8 Read_MultiByte( uint32 Address, uint8 n, uint8 *Buf )
{
  uint8 i;

  EnableSPI();
  
  SPI_Send_Byte( READ_CMD );  //0x03 read command
  SPI_Send32Address(Address); // send 3 address bytes
  for ( i = 0; i < n; i++ )   // read until no_bytes is reached
  {
    Buf[i] = SPI_Send_Byte(0xff); // receive byte and store at address 80H - FFH
  }
  
  DisableSPI();
  
  return(0);
}
*/

/************************************************************************
HighSpeed_Read
High-speed data read 1byte
Address--Address
Returns - Data
************************************************************************/
/*
uint8 HighSpeed_Read_Byte( uint32 Address )
{uint8 dat = 0;

  EnableSPI();
  
  SPI_Send_Byte( HIGH_SPEED_READ );      //0x0B read command
  SPI_Send32Address(Address); // send 3 address bytes
  SPI_Send_Byte( 0xFF );      //dummy byte, Can send any number
  dat = SPI_Send_Byte(0xff);
  
  DisableSPI();
  return dat;
}
*/

/************************************************************************
HighSpeed_Read_MultiByte
High-speed sequential read data
Address--Address
Buf--Data
n -- The amount of data
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
Byte Programming
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
  EnableWrite();                	// Write Enable
  
  EnableSPI();
  SPI_Send_Byte( BYTE_PROGRAM_CMD ); //0x02 send Byte Program command
  SPI_Send32Address(Address); 		// send 3 address bytes
  SPI_Send_Byte( dat );				// send byte to be programmed
  DisableSPI();
  
  Wait_Busy();
  
}
//*/

/************************************************************************
Programming multiple bytes, use the address is automatically increased command    
Address--Address
Buf--Data
n--The amount of data
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
   EnableWrite();              // Write Enable

   EnableSPI();
   SPI_Send_Byte( AAI_PROGRAM_CMD ); //0xAF send AAI command 
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

   // DisableWrite(), Read_Status_Register() Indicates that the address 
   // is automatically increased programming command ends 
   DisableWrite();
   Read_Status_Register();
   */
}


/************************************************************************
Chip_Erase
Chip Erase
************************************************************************/
/*
void Chip_Erase(void)
{
  EnableWrite();         // Write Enable
  
  EnableSPI();
  SPI_Send_Byte( CHIP_ERASE_CMD );      //0x60 send Block Erase command
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
Sector_Erase
Sector Erase (a sector 4Kbyte)
Input:
Dst:    Destination Address 000000H - 0FFFFFH
************************************************************************/
uint8 Sector_Erase( uint32 Address )
{
  EnableWrite();              // Write Enable

  EnableSPI();
  SPI_Send_Byte( SECTOR_ERASE_CMD );      //0x20 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
  return(0);
}

/************************************************************************
Block_Erase
Block Erase (one block 64Kbyte)
 Input:
   Dst:    Destination Address 000000H - 0FFFFFH

************************************************************************/
/*
void Block_Erase( uint32 Address )
{
  
  EnableWrite();              // Write Enable
  
  EnableSPI();
  SPI_Send_Byte( BLOCK_ERASE_CMD );      //0x52 send Block Erase command
  SPI_Send32Address(Address); // send 3 address bytes
  DisableSPI();

  Wait_Busy();
}
*/

/************************************************************************
* Function prototype: Wait_Busy ()
* Function: judge busy
* Input parameters:
* Output parameters: 0: Idle, not 0: busy, overtime failed launch
* Function Description: The use of the following features:
  £¨Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
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
stm32 72MHz The time constant under
The time constant from Read_Status_Register() Decide
100   = 410us
1000  = 4.09ms
10000 = 40.1ms
20000 = 80.2ms
*/
{uint32 counter;
  
  counter=0;
  while ((Read_Status_Register() & 0x03) == 0x03)
  {
     if( ++counter > (20000) ) // Approximately 80ms
        break;
  }
  return(0);
}

/************************************************************************
* Function prototype: Wait_Busy_AAI ()
* Function: judge busy
* Input parameters:
* Output parameters: 0: Idle, not 0: busy, overtime failed launch
* Function Description: The use of the following features:
  £¨Byte-Program, Sector-Erase, Block-Erase, Chip-Erase)
************************************************************************/
/*
static uint8 Wait_Busy_AAI(void)
{uint8 time;
  
  time=10;
  
  while ((Read_Status_Register() & 0x43) == 0x43)
  {
    if( (time=Timer_Over_Ms(time))==0 )
      return(1);   // Exit Timeout
  }
  return(0);
}
*/

/************************************************************************
Verify
Check
Address-- address
Buf-- data
n-- amount of data
Returns: 0: the same one: not the same
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
After verification program
Address-- address
Buf-- data
n-- amount of data
Returns: 0: Success 1: Failed
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


// If the write data is automatically erased the sector header
uint8 Write_Flash_Page_FirstErase( uint32 Address, uint16 n, uint8 *Buf )
{//uint8  status;
   
   if((Address%FLASH_SECTOR_SIZE)==0) // When moving to a header, the sector erase
      Erase_Flash_Sector( Address );
     
   return( Write_Flash_Page( Address, n, Buf ) );
}

/*
Write flash, can not be written to erase rewritable

Note: You can not write across sectors
       Sector can not exceed the maximum number of bytes
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
//       if(status==0) // Directly written
//       {
//          status=Write_Flash_Page( Address, n, Buf );
//       }
//       else // Must be erased to write
//       {
//          Read_Flash_Page(Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp); // Read a sector from the first address of the sector
//          memcpy(temp+(Address&FLASH_SECTOR_MASK), Buf, n); // Copy the data to the sector address
//          Erase_Flash_Sector( Address );
//          status=Write_Flash_Page( Address&FLASH_SECTOR_NOTMASK, FLASH_SECTOR_SIZE, temp ); // Write an entire sector
//       }
//    }
//    return(status);
//}


/*
Test data are not erased if you can write flash, flash the original is 1-> 0 can be written, 
the original is 0-> 1 can not be written
Input: WriteInfo-- write information, FlashInfo - Flash in the data obtained
      1001_0000   1111_0100, You can write
Returns:
     0 = can be written
     1 = not write
    
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

