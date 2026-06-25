
#include "stc8h.h"       //包含此头文件后，不需要再包含"reg51.h"头文件

#define     MAIN_Fosc       24000000L   //定义主时钟


#define button P35

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

void  delay_ms(u8 ms); 



/******************** 主函数 **************************/
void main(void)
{
	
	  /* Variable would be up here, bro  */
	  int prev_floor = 0, cur_floor = 0;
	
	
    P_SW2 |= 0x80;  

    P0M1 = 0x30;   P0M0 = 0x30;  
    P1M1 = 0x30;   P1M0 = 0x30;   
    P2M1 = 0x3c;   P2M0 = 0x3c;  
    P3M1 = 0x50;   P3M0 = 0x50;   
    P4M1 = 0x3c;   P4M0 = 0x3c;   
    P5M1 = 0x0c;   P5M0 = 0x0c;   
    P6M1 = 0x00;   P6M0 = 0xff; 
    P7M1 = 0x00;   P7M0 = 0x00; 
		
		P35 = 1; 
	  P34 = 0; 
		while (1){
			if (P35 == 0){
				delay_ms(10);
				if(P35 == 0){
					P34 = !P34;
					while(P35 == 0);
					delay_ms(10);
				}
				}
			}
}
void delay_ms(u8 ms)
{
     u16 i;
     do{
          i = MAIN_Fosc / 10000;
          while(--i);   //10T per loop
     }while(--ms);
}