
#define UART0_Timer1  1
#define UART0_Timer3  2
#define UART1_Timer3	3
#define UART2 4
#define UART3 5
#define UART0 6
#define UART1 7




void UART_Open(unsigned long u32SysClock, unsigned char u8UARTPort,unsigned long u32Baudrate);
void UART_Send_Data(unsigned char UARTPort, unsigned char c);
void UART_Send_String(unsigned char UARTPort, unsigned char *c);
unsigned char UART_Receive_Data(unsigned char UARTPort);
void UART_Interrupt_Enable(unsigned char UARTPort, unsigned char u8UARTINTStatus);
void UART0_LIRC_Baudrate2400_Open(void);

void UARTDataRec(char RecByte);

//typedef UINT32 (*Callback_ATResponse)(char* line, UINT32 len, void* userData);
//UINT8 SendAtCmd(char*  atCmd, UINT16 atCmdLen, Callback_ATResponse atRsp_callBack, void* userData, UINT32 timeOut);



