#include "stc8h.h"

#define MAIN_Fosc       24000000L
#define SERVO P53 
typedef unsigned char   u8;
typedef unsigned int    u16;
typedef unsigned long   u32;



bit servo_high = 1; 
u16 pulse_width = 1500; 

void delay_ms(u16 ms)
{
     u16 i;
     do{
          i = MAIN_Fosc / 13000; // Adjusted slightly for 1T loop execution overhead
          while(--i);   
     }while(--ms);
}

void TIMER0_ISR(void) interrupt 1 {
	// high_ticks = high phase in wave, low_ticks = low phase in wave. 
	TR0 = 0; 
	if (servo_high){
		 u32 low_ticks = (20000 - pulse_width) * 24; // 20000ms because servo motor works at 50Hz
	   SERVO = 0; 
		 servo_high = 0; 
		 TH0 = (65536 - low_ticks) >> 8;
     TL0 = (65536 - low_ticks) & 0xFF; 		
	}
	else if (servo_high == 0) {
		u32 high_ticks = (u32)pulse_width * 24; 
		TH0 = (65536 - high_ticks) >> 8; 
		TL0 = (65536 - high_ticks) & 0xFF; 
		servo_high = 1; 
		SERVO = 1; 
	}
	TR0 = 1; 
}

void main()
{
    
    unsigned int pw; 
	  P3M1 &= ~0x10; 
    P3M0 |= 0x10; 
    P5M1 = 0x00; P5M0 = 0x08;
    // Timer setup
		TMOD = 0x01; 
	  TH0 = (65536 - pulse_width * 24) >> 8; 
		TL0 = (65536 - pulse_width * 24) & 0xFF; 
		
		ET0 = 1; 
		EA = 1; 
		SERVO = 1; 
		servo_high = 1; 
		TR0=1;	
    while(1)
    {
        P34 = !P34;       
        delay_ms(2000);   
			
			 // Phase 1: stop ? full CW
        for (pw = 1500; pw >= 1000; pw -= 10)
        {
            pulse_width = pw;
            delay_ms(20);   // one PWM period per step
        }

        // hold full CW 2 seconds
        pulse_width = 1000;
        delay_ms(2000);

        // Phase 2: full CW ? stop
        for (pw = 1000; pw <= 1500; pw += 10)
        {
            pulse_width = pw;
            delay_ms(20);
        }

        // hold stop 1 second
        pulse_width = 1500;
        delay_ms(1000);

        // Phase 3: stop ? full CCW
        for (pw = 1500; pw <= 2000; pw += 10)
        {
            pulse_width = pw;
            delay_ms(20);
        }

        // hold full CCW 2 seconds
        pulse_width = 2000;
        delay_ms(2000);

        // Phase 4: full CCW ? stop
        for (pw = 2000; pw >= 1500; pw -= 10)
        {
            pulse_width = pw;
            delay_ms(20);
        }

        // hold stop 1 second
        pulse_width = 1500;
        delay_ms(1000);
    }
    
}
