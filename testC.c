/*Documentation: 
Buttons: P1.0 (B1), P1.1 (B2), P1.3 (B3)
Sensors: P1.4 (S1), P1.5 (S2), P1.6 (S3) -> Active Low (0 = object detected)
Direction: P2.2 (UP), P2.3 (DOWN)
Indicator: P3.0, P3.1 (LEDs)
*/ 

#include "stc8h.h"       

#define  MAIN_Fosc  24000000L   

// Button Definitions
#define B1 P10
#define B2 P11
#define B3 P13

// Sensor Definitions
#define S1 P14
#define S2 P15
#define S3 P16

typedef unsigned char u8;
typedef unsigned int  u16;

void delay_ms(u8 ms);

void main(void)
{
    // 1. Variables: Now we track where we ARE vs where we WANT to be
    int cur_floor = 1;    // Assume we power on at Floor 1
    int target_floor = 1; // We start with no pending requests

    // 2. Hardware Initialization (Using the safe bitmasking!)
    P_SW2 |= 0x80;  
    
    // Port 1 (Buttons AND Sensors) as Quasi-Bidirectional Input
    P1M1 = 0x00;   P1M0 = 0x00;    
    
    // Port 2 (Direction Motor) as Push-Pull Output
    P2M1 &= ~0x0C; P2M0 |= 0x0C;   
    
    // Port 3 (LED Indicator) as Push-Pull Output
    P3M1 &= ~0x03; P3M0 |= 0x03;  

    // Default Starting State
    P2 &= ~0x0C;             // Ensure motor is stopped (P2.2=0, P2.3=0)
    P3 = (P3 & 0xFC) | 0x01; // Set LED display to Floor 1 (01)

    while(1)
    {

        if (cur_floor == target_floor) 
        {
            if (B1 == 0) { delay_ms(20); if(B1 == 0){ target_floor = 1; while(B1 == 0); } }
            else if (B2 == 0) { delay_ms(20); if(B2 == 0){ target_floor = 2; while(B2 == 0); } }
            else if (B3 == 0) { delay_ms(20); if(B3 == 0){ target_floor = 3; while(B3 == 0); } }

            // If a new target was just set, start the motor!
            if (target_floor > cur_floor) {
                P2 = (P2 & ~0x0C) | 0x04; // Motor UP (P2.2=1, P2.3=0)
            } 
            else if (target_floor < cur_floor) {
                P2 = (P2 & ~0x0C) | 0x08; // Motor DOWN (P2.2=0, P2.3=1)
            }
        }
        
      
        else 
        {
            // We only look for the sensor of the floor we are traveling to
            if (target_floor == 1 && S1 == 0) {
                delay_ms(10); // Quick debounce for sensor
                if (S1 == 0) cur_floor = 1; // We arrived!
            }
            else if (target_floor == 2 && S2 == 0) {
                delay_ms(10);
                if (S2 == 0) cur_floor = 2; 
            }
            else if (target_floor == 3 && S3 == 0) {
                delay_ms(10);
                if (S3 == 0) cur_floor = 3; 
            }

            // Once the sensor updates cur_floor to match target_floor:
            if (cur_floor == target_floor) {
                P2 &= ~0x0C; // 1. KILL THE MOTOR IMMEDIATELY
                P3 = (P3 & 0xFC) | cur_floor; // 2. UPDATE THE LED DISPLAY (01, 10, or 11)
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