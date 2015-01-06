#if !defined( _SST25LF080_H )
#define _SST25LF080_H

//#define SST28   0         
//#define SST29   1

#define  FLASH_SECTOR_SIZE   (4*1024)
//#define  FLASH_SECTOR_SIZE   (4*1024)
#define  FLASH_SECTOR_MASK   (FLASH_SECTOR_SIZE-1)//=0xfff
#define  FLASH_SECTOR_NOTMASK   (~(uint32)(FLASH_SECTOR_SIZE-1))//=0xfffff000

#define  Erase_Flash_Sector   Sector_Erase
#define  Read_Flash_Page      HighSpeed_Read_MultiByte
#define  Write_Flash_Page     ProgramVerify_MultiByte

//开保护
void UnProtectSST25VF080(void);

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
uint8 ReadDeviceID(void);//( uint8 ID_addr );


uint8 Sector_Erase( uint32 Address );

uint8 HighSpeed_Read_MultiByte( uint32 Address, uint16 n, uint8 *Buf );


uint8 ProgramVerify_MultiByte( uint32 Address, uint16 n, uint8 *Buf );

//如果写数据是扇区头自动擦除该扇区
uint8 Write_Flash_Page_FirstErase( uint32 Address, uint16 n, uint8 *Buf );

uint8 Force_Write_Flash_Page( uint32 Address, uint16 n, uint8 *Buf );

uint8 testFlashWrite( uint8 *WriteInfo, uint8 *FlashInfo, uint8 cmplen);

#endif
