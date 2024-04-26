#ifndef __RDATAFLASH_H__
#define __RDATAFLASH_H__


#define FLASH_START_ADD   0x7F80
#define FLASH_QRLEN_LADD  0x7F81
#define FLASH_QRLEN_HADD  0x7F82
#define FLASH_START_IMG_ADD  0x7B80

void Page_Erase(UINT16 Add);
void Write_Data_Byte(UINT16 Add, UINT8 Data);
UINT8 Read_Data_Byte(UINT16 code *Add);
void Write_Flash(void);
void ReadFlash(void);
UINT16 ConvertToUINT16(UINT8 Tenth,UINT8 Unit);
void Default_Settings(void);
void WriteFlashQr(UINT16 Length);

#endif //__RDATAFLASH_H__