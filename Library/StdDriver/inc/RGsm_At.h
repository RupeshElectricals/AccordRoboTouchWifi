#ifndef __RGSM_AT_H__
#define __RGSM_AT_H__

#define MAX_UART_LINE		2
#define MAX_UART_DATA   50

#define DEBUG_UART 		UART2
#define GSM_UART			UART0
#define WIFI_UART     UART1 
#define DEFAULT_TOUT	5000 //5SEC
#define AT_TOUT				1000 //1SEC
#define MAX_ERROR_TRY 5

typedef enum
{
	  ATRSP_SUCCESS  =   0,
	  ATRSP_FAILED   =   1,
    ATRSP_TIMEOUT  =   2,
    ATRSP_CONTINUE =   3,
}	EnumAtResp;


void DebugPrintf (char *fmt, ...);
void ClearUart(void);
void NW_Responce_Handler(UINT8 *Line,void * UserData);
void CheckUrc(void);

#endif //__RGSM_AT_H__