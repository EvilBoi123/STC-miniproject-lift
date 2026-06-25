 /*Documentation: 
P13, P11, P10: button
P2.2, P2.3: Direction
P3.0, P3.1: led indicator

*/ 

#include "stc8h.h"       //包含此头文件后，不需要再包含"reg51.h"头文件


#define     MAIN_Fosc       24000000L   //定义主时钟


#define B1 P10
#define B2 P11
#define B3 P13


typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

void  delay_ms(u8 ms);
void direction(int cur, int prev); 




/******************** 主函数 **************************/

void main(void)

{

      /* Variable would be up here, bro  */

      int prev_floor = 0, cur_floor = 0;

  
   // P_SW2 |= 0x80;  

    P_SW2 |= 0x80;  //扩展寄存器(XFR)访问使能
    P1M1 = 0x00;   P1M0 = 0x00;    
    P2M1 &= 0x0c;   P2M0 |= 0x0c;   
    P3M1 &= 0x03;   P3M0 |= 0x03;  
    P4M1 = 0x00; P4M0 = 0x00;
    P5M1 = 0x00; P5M0 = 0x00;
    P6M1 = 0x00; P6M0 = 0x00;
    P7M1 = 0x00; P7M0 = 0x00;  

   P0 = 0xFF;
   P1 = 0x0b; 
    P2 = 0xFF;    
    P40 = 0;        //LED Power On
     P3 = 0xe0;
    while(1)

    {

            if (B1 == 0){
                delay_ms(20); 
                if(B1 == 0){ 
                    P3 &= 0xe0;
                    while (B1 == 0);
                    P3 = 0xe1; 
                    cur_floor = 1; 
                    direction(cur_floor, prev_floor); 
                    prev_floor = cur_floor; 
                    delay_ms(20); 
                }
            }
          
            else if (B2 == 0){
                delay_ms(20); 
                if(B2 == 0){ 
                    P3 &= 0xe0;
                    while (B2 == 0);
                   P3 = 0xe2;
                   cur_floor = 2; 
                    direction(cur_floor, prev_floor); 
                    prev_floor = cur_floor;
                    delay_ms(20); 
                }
            }          

            else if (B3 == 0){
                delay_ms(20); 
                if(B3 == 0){ 
                    P3 &= 0xe0;
                   while (B3 == 0);
                    P3 = 0xe3;
                    cur_floor = 3; 
                    direction(cur_floor, prev_floor); 
                    prev_floor = cur_floor;
                   delay_ms(20); 

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


void direction(int cur, int prev){
    if (cur > prev){ 
        P2 = 0x04; 
    }
		
    else if (cur < prev) {
        P2 = 0x08; 
    }
    else if (cur == prev){
        P2 = 0x0; 
    }

} 