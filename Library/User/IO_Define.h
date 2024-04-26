#ifndef __IO_DEFINE_H__
#define __IO_DEFINE_H__



//#define RST   P30
//#define RW    P31
//#define RS    P32
//#define EN    P33
////#define CS1   P55
////#define CS2   P54

//#define CS1(x)			SFRS=0;P5 = ((P5 & 0xDF) | (x<<5)); //P55
//#define CS2(y)      SFRS=0;P5 = ((P5 & 0xEF) | (y<<4)); //P54 

#define RST(y)   SFRS=0;P5 = ((P5 & 0xEF) | (y<<4)); //P54 
//#define RW    P31
#define RS    P25
#define EN    P24

#define CS1(x)			SFRS=0;P5 = ((P5 & 0xDF) | (x<<5)); //P55
#define CS2   P32 //P54 


#define GLCD_LOW_PORT   P2
#define GLCD_HIGH_PORT  P0

#define RELAY P33

#define RELAY_ON  1
#define RELAY_OFF 0

#endif //__IO_DEFINE_H__