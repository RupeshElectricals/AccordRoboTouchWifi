#include "ML51.h"
#include "RTimer0.h"
bit FlagSystemConnected=0;
bit FlagChairWorking=0;
bit FlagChairlastMin=0;
bit FlagStopAud=0;
bit FlagOffAud=0;
UINT8 OffTime=0;
extern bit BIT_TMP; 
// UART.c
extern UINT8 xdata UartRecBuff[MAX_UART_LINE][MAX_UART_DATA],UartTempRecBuff[ ];
extern UINT8 UartRecFlag,UartRecCount;
extern UINT8 UartLine;
extern UINT8 UartStarRec;

extern UINT16 xdata GlcdImageCnt;
extern UINT8 GlcdUartRec;
extern UINT8 xdata QrArray[];

extern struct Flag Flags;
extern code char PhonePeLogo[];
extern code char  paytmlogo [];

extern UINT8 PreStartTime;

UINT8 PreUartLine=0;
UINT8 SMSRecFlag=0;


int TimerForChair=0;
code UINT8 CREG[] = "+CREG:";
code UINT8 GLCD_IMG_START[] = "IMG_START:";
code UINT8 GLCD_IMG_END[] = "IMG_STOP";
code UINT8 MCU_TIME[] = "TIME:";
code UINT8 PID_RECV[] = "*PID:";
code UINT8 MQTT_CONNECT[] = "MQTT_CONN";
code UINT8 MQTT_DISCONNECT[] = "MQTT_DISCONN";
code UINT8 GET_PID[] = "GET_PID";
xdata UINT8 PidArray[15];
xdata char etxt[200]; /* text buffer */

void DebugPrintf (char *fmt, ...) {
  va_list arg_ptr;
  va_start (arg_ptr, fmt); /* format string */
  vsprintf (etxt, fmt, arg_ptr);
  va_end (arg_ptr);
	UART_Send_String(DEBUG_UART,etxt);
	
}


void NW_Responce_Handler(UINT8 *Line,void * UserData)
{
	UART_Send_String(DEBUG_UART,Line);
	if(strstr(Line,"+CREG: ") && strstr(Line,","))
	  sscanf( Line, "+CREG: %d,%d %*[^\r\n]\r\n", UserData,UserData);
	else  if(strstr(Line,"+CREG:"))
		sscanf( Line, "+CREG: %d %*[^\r\n]\r\n", UserData);	 
}


void CheckUrc(void)
{
		while(PreUartLine!=UartLine)
		{
		  PreUartLine++;
			//UART_Send_String(DEBUG_UART,UartRecBuff[PreUartLine-1]);
			
			DebugPrintf("<*****URC = %s @UARTNO:%d",UartRecBuff[PreUartLine-1],Flags.UartNo);
			 //UART_Send_String(GSM_UART,UartRecBuff[PreUartLine-1]);
			
			//------------------------------------------------------------------------------------------//
//			if(strstr(UartRecBuff[PreUartLine-1],RING))         // RING NOTIFICATION
//			{
//				//  SendAtCmd("ATA\r\n",AT_RESP,0,0);
//				 // Flags.CallState = INCOMMING;
//				//	DebugPrintf("<------DEBUG MSG => Flags.CallState = %d---------\r\n",Flags.CallState);
//					SendAtCmd("AT+CLCC\r\n",CLCC_RESP,&MOB_NO,1000);
//					UART_Send_String(DEBUG_UART,MOB_NO);
//				 if(strstr(MOB_NO,"8600687322"))
//				 {
//					 SendAtCmd("ATA\r\n",AT_RESP,0,0);
//				   Flags.CallState = INCOMMING;
//				   DebugPrintf("<------DEBUG MSG => Flags.CallState = %d---------\r\n",Flags.CallState);
//				 }
//				
//				  
//			}
			//------------------------------------------------------------------------------------------//
		
			
			
//	    //------------------------------------------------------------------------------------------//
//			if(strstr(UartRecBuff[PreUartLine-1],CREG))         // CREG N/W NOTIFICATION
//			{
//			  Responce_Handler(NW_RESP,UartRecBuff[PreUartLine-1],&Flags.NwReg);
//				DebugPrintf("<------DEBUG MSG => Flags.NwReg = %d---------\r\n",Flags.NwReg);
//			}
//			
			
			
			 if(strstr(UartRecBuff[PreUartLine-1],GLCD_IMG_START))
			 {
				 Flags.GlcdImgStart=1;
				 GlcdUartRec=0;
				 GlcdImageCnt=0;
				 
         memset(QrArray,0x00,1024);
				 TIMER0_INTR_DIS;
			 }
			 
			 if(strstr(UartRecBuff[PreUartLine-1],GLCD_IMG_END))
			 {
				 QrArray[GlcdImageCnt]='\0';
				 Flags.GlcdImgStart=0;
				 GlcdUartRec=0;
				 
				 //GLCD_ClearAll();
				
				 WriteFlashQr(GlcdImageCnt);
				 Write_Flash();
				 memset(QrArray,0x00,1024);
				 ReadFlash();
				 //QrScreen();
				 
				 //GLCD_Image(0,0,0, QrArray,64,64);
				 
				 //Delay_1ms(1000);
				 //GLCD_Image(10,0,1, PhonePeLogo,32,32);
				 //GLCD_Image(0,4,1, paytmlogo,64,32);
				 
				 
				 
				 
				 Delay_1ms(2000);
				 
				 if(Flags.UartNo==1)
				 {
					 UART_Send_String(WIFI_UART,"IMG_RECV\n");
					 Delay_1ms(2000);
					 UART_Send_String(WIFI_UART,"IMG_RECV\n");
					 DebugPrintf("img receive from wifi");
				 }
				 else
				{
					 UART_Send_String(GSM_UART,"IMG_RECV\r\n");
					 Delay_1ms(2000);
					 UART_Send_String(GSM_UART,"IMG_RECV\r\n");
					 DebugPrintf("img receive from GSM");
				 }
				TIMER0_INTR_EN;	 
				 
				 //set_CHPCON_SWRST;
				 //GlcdImageCnt`=0;
			 }
			 
			 
			  if(strstr(UartRecBuff[PreUartLine-1],MQTT_CONNECT))
			 {
				 FlagSystemConnected=1;
				 if(TimerForChair==0)
				 {
					 
				 TIMER0_INTR_DIS;	 
				 FlagSystemConnected=1;
				
				 QrScreen();
				 Delay_1ms(1000);
				 TIMER0_INTR_EN;	 
				 }
				 
			 }
			 
			 if(strstr(UartRecBuff[PreUartLine-1],MQTT_DISCONNECT))
			 {
				 FlagSystemConnected=0;
				 if(TimerForChair==0 && Flags.RunnigScreen!=LoadingScr && FlagChairWorking==0)
				 {
					GLCD_ClearAll();
					// TIMER0_INTR_EN;	
				 }
				// QrScreen();
				 
			 }
			 
			  if(strstr(UartRecBuff[PreUartLine-1],GET_PID)&& (Flags.PidReceived==1))
			 {
	//			 TIMER0_INTR_DIS;	 
//				memset(PidArray,0x00,15);
//				strcpy(PidArray,UartRecBuff[PreUartLine-1]);
//				PidArray[11]='\0';
//        PidArray[12]='\0';
			
				UART_Send_String(WIFI_UART,PidArray);
				//UART_Send_String(WIFI_UART,"\n");
			//	UART_Send_String(WIFI_UART,"\n"); 
				Flags.PidReceived=1;
				//DebugPrintf("\r\n-- PID REQUEST=%s--\r\n",PidArray);
				 DebugPrintf("<------PID REQUEST = %s,%d---------\r\n",PidArray,strlen(PidArray)); 
	//			 TIMER0_INTR_EN;	 
			 }
			 
			 if(strstr(UartRecBuff[PreUartLine-1],MCU_TIME))
			 {
				  DebugPrintf("\r\n-- uart rec %s--\r\n",UartRecBuff[PreUartLine-1]);
				  sscanf(UartRecBuff[PreUartLine-1],"TIME:%d\r\n", &TimerForChair);
				  if(TimerForChair>0)
					{
						//TimerForChair = 1;
						PreStartTime  = PRE_START_TIME;
						TimerForChair = (TimerForChair*60);
						//TimerForChair = (1*10);
						FlagChairWorking = 1;
						GLCD_ClearAll();
						ChairAnimation();
					//	GLCD_Image(24,0,0, chair_left,80,64);
					//	RELAY = RELAY_ON;
					}
				  DebugPrintf("<------timer value = %d---------\r\n",TimerForChair);
			 }
			 
			 if(strstr(UartRecBuff[PreUartLine-1],PID_RECV))
			 {
			//	TIMER0_INTR_DIS;	  
				memset(PidArray,0x00,15);
				//for(k=0;k<11;k++) 
				strcpy(PidArray,UartRecBuff[PreUartLine-1]);
				PidArray[11]='\n';
        PidArray[12]='\0';				 
				DebugPrintf("<------PidArray = %s,%d---------\r\n",PidArray,strlen(PidArray)); 
				
				Flags.PidReceived=1;
				 
					
					//------UART 1 INIT-----------//  WIFI
//					if(P14==1)
//					{
//					UART_Open(24000000,UART1_Timer3,9600); 
//					UART_Interrupt_Enable(UART1,Enable);
//					}
//					//else
//					//	UART_Interrupt_Enable(UART1,Disable); 
			//TIMER0_INTR_EN;	 	 
			 }
			ClearUart();
		DebugPrintf("<------DEBUG MSG => UARTLINE EXIT = %d---------\r\n",(int*)UartLine);
		}
		
}

void ClearUart(void)
{
	PreUartLine=UartLine=0;	
  UartStarRec=0;	
	Flags.UartNo=0;
	memset(UartRecBuff[0],'\0',50);
	memset(UartRecBuff[1],'\0',50);
}



