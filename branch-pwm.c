 /*Documentation: 
P15, P16, P17: button
P3.0, P3.1: led indicator
*/ 

#include "stc8h.h"     


#define FOSC            24000000UL
#define TICKS_PER_US    (FOSC / 1000000UL)


#define SERVO P53
#define B1 P15
#define B2 P16
#define B3 P17


typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

bit servo_high = 0; 
volatile unsigned int pulse_width = 1500; // about 1.5us for stopping

void  delay_ms(u8 ms);
void turn_servo(int cur, int prev); 
void delay_long_us(unsigned int us); 
void delay_us(unsigned int us); 
// void direction(int cur, int prev); 


void main(void)

{

      /* Variable would be up here, bro  */

    int prev_floor = 0, cur_floor = 0;
			
  

    P_SW2 |= 0x80;  
    P1M1 = 0x00;   P1M0 = 0x00;    // all others are mode 1 except P1.4 as pure output
    P2M1 = 0x00;   P2M0 = 0x07;   
    P3M1 = 0x00;   P3M0 = 0x10;  
    P4M1 = 0x00; P4M0 = 0x00;
    P5M1 = 0x00; P5M0 = 0x00;
    P6M1 = 0x00; P6M0 = 0x00;
    P7M1 = 0x00; P7M0 = 0x08;  
     
	
	 // Timer setup
		TMOD = 0x11; 
	  AUXR |= 0x80;   // 1T mode
	  TH0 = (65536 - pulse_width * 24) >> 8; 
		TL0 = (65536 - pulse_width * 24) & 0xFF; 

		ET0 = 1; 
		EA = 1; 
		
		SERVO = 0; 
		servo_high = 0; 
		
		TR0=1; 
		TR1=1; 
		P3 = 0x10; 
		P1 = 0xe0; 
		P2 = 0; 
    while(1)
    {

            if (B1 == 0){
                delay_ms(20); 
                if(B1 == 0){ 
                    //P3 &= 0xe0;
                    while (B1 == 0);
                    //P3 = 0xe1; 
   							  
									  cur_floor = 1; 
                    //direction(cur_floor, prev_floor);
										turn_servo(cur_floor, prev_floor); 
									
									  P2 = 0x02; 
                    prev_floor = cur_floor; 
                    delay_ms(20); 
                }
            }
          
            else if (B2 == 0){
                delay_ms(20); 
                if(B2 == 0){ 
                    //P3 &= 0xe0;
                    while (B2 == 0);
                    //P3 = 0xe2;
                   cur_floor = 2; 
                    //direction(cur_floor, prev_floor); 
									  turn_servo(cur_floor, prev_floor); 
									   P2 = 0x04; 
                    prev_floor = cur_floor;
                    delay_ms(20); 
                }
            }          

            else if (B3 == 0){
                delay_ms(20); 
                if(B3 == 0){ 
                    //P3 &= 0xe0;
                   while (B3 == 0);
                    //P3 = 0xe3;
                    cur_floor = 3; 
                    //direction(cur_floor, prev_floor); 
									  turn_servo(cur_floor, prev_floor);
										 P2 = 0x08; 
                    prev_floor = cur_floor;
                   delay_ms(20); 

                }

            }       
        }    

}

void TIMER0_ISR(void) interrupt 1 {
	// high_ticks = high phase in wave, low_ticks = low phase in wave. 
	TR0 = 0; 
	
	if (servo_high){
		 unsigned long low_ticks = (20000 - pulse_width) * 24; // 20000ms because servo motor works at 50Hz
	   SERVO = 0; 
		 servo_high = 0; 
		 TH0 = (65536 - low_ticks) >> 8;
     TL0 = (65536 - low_ticks) & 0xFF; 		
	}
	else if (servo_high == 0) {
		unsigned long high_ticks = (u32)pulse_width * 24; 
		TH0 = (65536 - high_ticks) >> 8; 
		TL0 = (65536 - high_ticks) & 0xFF; 
		servo_high = 1; 
		SERVO = 1; 
	}
	TR0 = 1; 
}

void turn_servo(int cur, int prev){
	int pw;  
	if (cur > prev){
		// here is initial phase, it will spin CW slowly from stop. 
		for (pw = 1500; pw >= 1000; pw -= 10){
			pulse_width = pw; 
			delay_long_us(3000);
		}
		
		pulse_width = 1000; 
		delay_long_us(3000);; // example for now, it will spin for 2 seconds
   		
		
		for (pw = 1000; pw <= 1500; pw += 10){ // stop gradually. 
			pulse_width = pw; 
			delay_long_us(3000);
		}
		pulse_width = 1500; 
	}
	else if (cur < prev){
		for (pw = 1500; pw <= 2000; pw += 10){
			pulse_width = pw; 
			delay_long_us(3000); 
		}
		
		pulse_width = 2000;  
		delay_long_us(30000); // example for now, it will spin for 2 seconds
   		
		
		for (pw = 2000; pw >= 1500; pw -= 10){ // stop gradually. 
			pulse_width = pw; 
			delay_long_us(3000); 
		}
		pulse_width = 1500; 
	}
	else if (cur == prev){ // just don't spin at all.. 
        pulse_width = 1500; 
    }
}

void delay_ms(u8 ms)
{
     u16 i;
     do{
          i = FOSC / 10000;
          while(--i);   //10T per loop
     }while(--ms);
}

void delay_us(unsigned int us)
{
    unsigned long ticks;
    unsigned int reload;

    if(us == 0)
        return;

    ticks = (unsigned long)us * TICKS_PER_US;
    reload = (unsigned int)(65536UL - ticks);

    TH1 = (unsigned char)(reload >> 8);
    TL1 = (unsigned char)(reload & 0xFF);

    TF1 = 0;
    TR1 = 1;

    while(TF1 == 0);

    TR1 = 0;
    TF1 = 0;
	}

void delay_long_us(unsigned int us)
{
    while(us > 2500)
    {
        delay_us(2500);
        us -= 2500;
    }

    if(us > 0)
        delay_us(us);
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