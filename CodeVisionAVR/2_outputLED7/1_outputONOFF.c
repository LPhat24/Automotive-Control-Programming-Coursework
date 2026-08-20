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

// Turn off all segments of the 7-segment LED
void turn_off_segments()
{
      LEDa = 0; LEDb = 0; LEDc = 0; LEDd = 0; LEDe = 0; LEDf = 0; LEDg = 0;
}

void main(void)
{
    // Crystal Oscillator division factor: 1
    #pragma optsize-
    CLKPR = (1<<CLKPCE);
    CLKPR = (0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif

    DDRD = 0x60;  // PD5, PD6 as outputs
    DDRB = 0x1F;  // PB0..PB4 as outputs

    Display7SEGMENT(20); // Turn on all segments
    delay_ms(1000);     // Wait for 1000 ms

    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            Display7SEGMENT(i); // Display digits 0 to 9
            delay_ms(200);     // Wait for 200 ms
        }
        Display7SEGMENT(16); // Turn off all segments
        delay_ms(500);     // Wait for 500 ms
    }
}