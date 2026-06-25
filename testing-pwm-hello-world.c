#include "stc8h.h"

sbit Servo = P5^3;


#define FOSC            24000000UL
#define TICKS_PER_US    (FOSC / 1000000UL)

volatile unsigned int pulse_width = 1500;
volatile unsigned int ms_counter = 0;
bit state = 0;

/* ---------------------------
   Timer reload helper (us)
----------------------------*/
void Timer0_Reload_us(unsigned int us)
{
    unsigned long ticks;
    unsigned int reload;

    // 1T mode => 1 tick = 1 / 24MHz = 41.6ns
    ticks = (unsigned long)us * (FOSC / 1000000UL);

    reload = 65536 - ticks;

    TH0 = reload >> 8;
    TL0 = reload & 0xFF;
}

/* ---------------------------
   Timer0 ISR
   Generates servo waveform
----------------------------*/
void Timer0_ISR(void) interrupt 1
{
    if(state == 0)
    {
        Servo = 1;                      // start HIGH pulse
        Timer0_Reload_us(pulse_width); // HIGH duration
        state = 1;
    }
    else
    {
        Servo = 0;                          // LOW phase
        Timer0_Reload_us(20000 - pulse_width); // rest of frame
        state = 0;
    }
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
/* ---------------------------
   simple delay
----------------------------*/
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

/* ---------------------------
   MAIN
----------------------------*/
void main(void)
{
    /* P5.3 push-pull */
    P5M1 &= ~0x08;
    P5M0 |= 0x08;

    Servo = 0;

    /* Timer0 (servo) */
    TMOD = 0x11; 

    AUXR |= 0x80;   // 1T mode

    /* init first cycle */
    Timer0_Reload_us(20000);
    state = 0;

  
    ET0 = 1;
	  EA = 1; 
    TR0 = 1;
    
    TR1 = 1;
    while(1)
    {
        pulse_width = 1000;
        delay_long_us(3000000);

        pulse_width = 1500;
        delay_long_us(3000000);
			
        pulse_width = 2000;
        delay_long_us(3000000);
    }
}