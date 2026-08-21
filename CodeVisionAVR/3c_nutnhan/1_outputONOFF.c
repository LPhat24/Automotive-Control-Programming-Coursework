#include <mega328p.h>
#include <delay.h>

//--7 SEGMENT LED--
#define LEDa PORTD.6  // Segment a -> PORTD pin 6
#define LEDb PORTB.0  // Segment b -> PORTB pin 0
#define LEDc PORTB.1  // Segment c -> PORTB pin 1
#define LEDd PORTD.5  // Segment d -> PORTD pin 5
#define LEDe PORTB.2  // Segment e -> PORTB pin 2
#define LEDf PORTB.3  // Segment f -> PORTB pin 3
#define LEDg PORTB.4  // Segment g -> PORTB pin 4

// Segment patterns for digits 0..9 (order: a,b,c,d,e,f,g)
const unsigned char digit[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}, // 9
};

unsigned char i;  // Variable to hold the current digit to display
unsigned char nutSW1, nutSW2;  // Variables to hold the state of the switches

// Function to display a digit on the 7-segment LED
void Display7SEGMENT(unsigned char num) 
{
//     LEDa = digit[num][0];
//     LEDb = digit[num][1];
//     LEDc = digit[num][2];
//     LEDd = digit[num][3];
//     LEDe = digit[num][4];
//     LEDf = digit[num][5];
//     LEDg = digit[num][6];
    if (num == 0 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 0; }
    if (num == 1 ) { LEDa = 0; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
    if (num == 2 ) { LEDa = 1; LEDb = 1; LEDc = 0; LEDd = 1; LEDe = 1; LEDf = 0; LEDg = 1; }
    if (num == 3 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 0; LEDg = 1; }
    if (num == 4 ) { LEDa = 0; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 1; LEDg = 1; }
    if (num == 5 ) { LEDa = 1; LEDb = 0; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 1; LEDg = 1; }
    if (num == 6 ) { LEDa = 1; LEDb = 0; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
    if (num == 7 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
    if (num == 8 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
    if (num == 9 ) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 0; LEDf = 1; LEDg = 1; }

    if (num == 16) { LEDa = 0; LEDb = 0; LEDc = 0; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0; }
    if (num == 20) { LEDa = 1; LEDb = 1; LEDc = 1; LEDd = 1; LEDe = 1; LEDf = 1; LEDg = 1; }
}

//  Turn off all segments of the 7-segment LED
// void turn_off_segments()
// {
//       LEDa = 0; LEDb = 0; LEDc = 0; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0;
// }

void main(void)
{
    // Crystal Oscillator division factor: 1
    #pragma optsize-
    CLKPR = (1<<CLKPCE);
    CLKPR = (0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    DDRD.6 = 1; // Set PORTD pin 6 as output (Segment a)
    DDRB.0 = 1; // Set PORTB pin 0 as output (Segment b)
    DDRB.1 = 1; // Set PORTB pin 1 as output (Segment c)
    DDRD.5 = 1; // Set PORTD pin 5 as output (Segment d)
    DDRB.2 = 1; // Set PORTB pin 2 as output (Segment e)
    DDRB.3 = 1; // Set PORTB pin 3 as output (Segment f)
    DDRB.4 = 1; // Set PORTB pin 4 as output (Segment g)

    DDRD.4 = 0; // Set PORTD pin 4 as input (LED ON/OFF control)
    PORTD.4 = 1; // Enable pull-up resistor on PORTD pin 4
    DDRD.7 = 0; // Set PORTD pin 7 as input (LED ON/OFF control)
    PORTD.7 = 1; // Enable pull-up resistor on PORTD pin 7

    Display7SEGMENT(20); // Turn on all segments
    delay_ms(1000);     // Wait for 1000 ms
    Display7SEGMENT(16); // Turn off all segments
    delay_ms(500);     // Wait for 500 ms

    while (1)
    {
        nutSW1 = PIND.4; // Read the state of switch 1 (PORTD pin 4)
        nutSW2 = PIND.7; // Read the state of switch 2 (PORTD pin 7)
        
        if (nutSW1 == 0) // If PORTD pin 4 is LOW (button pressed)
        {
            Display7SEGMENT(1); // Turn on all segments
            delay_ms(10);     // Wait for 10 ms
        }
        else if (nutSW2 == 0) // If PORTD pin 7 is LOW (button pressed)
        {
            Display7SEGMENT(2); // Turn off all segments
            delay_ms(10);     // Wait for 10 ms
        }
        else 
        {        
            Display7SEGMENT(16); // Turn off all segments
            delay_ms(10);     // Wait for 10 ms
        }
    }
}