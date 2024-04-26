#include "ML51.h"
#include <RDataFlash.h>

bit BIT_TMP;
UINT8 ResetCount=0;
extern UINT8 xdata QrArray[];
extern UINT16 xdata GlcdImageCnt;

void Page_Erase(UINT16 Add)
{
		IAPAL = Add;
		IAPAH = Add>>8;
		IAPFD = 0xFF;
	  set_CHPCON_IAPEN; 
    set_IAPUEN_APUEN;
    IAPCN = 0x22; 		
 		set_IAPTRG_IAPGO; 
}

void Write_Data_Byte(UINT16 Add, UINT8 Data)
{
	  IAPAL = Add;
		IAPAH = Add>>8;
		set_CHPCON_IAPEN; 
    set_IAPUEN_APUEN;
	  IAPCN = 0x21;
		IAPFD = Data;
		set_IAPTRG_IAPGO;
		clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}



UINT8 Read_Data_Byte(UINT16 code *Add)
{
	UINT8 rdata;
	rdata = *Add>>8;
	return rdata;
}

void Write_Flash(void)
{
	
	UINT16 xdata TempVar = 0;
	EA=0;
	Page_Erase(FLASH_START_ADD);
	Write_Data_Byte(FLASH_START_ADD,0x01);
	Write_Data_Byte(FLASH_QRLEN_LADD,GlcdImageCnt%100);
	Write_Data_Byte(FLASH_QRLEN_HADD,GlcdImageCnt/100);
	
	EA=1;
}

void WriteFlashQr(UINT16 Length)
{
	UINT16 k=0;
	Page_Erase(FLASH_START_IMG_ADD);
	for(k=1;k<=Length;k++)
	{
		if(k%128==0)
		{
			Page_Erase(FLASH_START_IMG_ADD+k);
		}
		Write_Data_Byte(FLASH_START_IMG_ADD+(k-1),QrArray[k-1]);
	}
}

void ReadFlash(void)
{
	UINT16 TmpCnt=0;
	ResetCount  = Read_Data_Byte(FLASH_START_ADD);
	if(ResetCount != 0x01)
		Default_Settings();
	
	GlcdImageCnt = Read_Data_Byte(FLASH_QRLEN_HADD)*100 + Read_Data_Byte(FLASH_QRLEN_LADD);
	if(GlcdImageCnt>0)
	{
		for(TmpCnt=0;TmpCnt<GlcdImageCnt;TmpCnt++)
		{
			QrArray[TmpCnt] = Read_Data_Byte(FLASH_START_IMG_ADD+TmpCnt);
		}
	}
		
}

UINT16 ConvertToUINT16(UINT8 Tenth,UINT8 Unit)
{
	return((Tenth*100)+(Unit));
}

void Default_Settings(void)
{
	GlcdImageCnt=0;
	Write_Flash();
}