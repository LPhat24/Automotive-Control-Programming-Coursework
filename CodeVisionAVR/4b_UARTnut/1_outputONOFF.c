#include <mega328p.h>
#include <delay.h>

// ------------UART-----------
#define DATA_REGISTER_EMPTY (1<<UDRE0)
#define RX_COMPLETE (1<<RXC0)
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)

// USART Receiver buffer
#define RX_BUFFER_SIZE0 64
char rx_buffer0[RX_BUFFER_SIZE0];

#if RX_BUFFER_SIZE0 <= 256
unsigned char rx_wr_index0=0,rx_rd_index0=0;
#else
unsigned int rx_wr_index0=0,rx_rd_index0=0;
#endif

#if RX_BUFFER_SIZE0 < 256
unsigned char rx_counter0=0;
#else
unsigned int rx_counter0=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow0;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSR0A;
data=UDR0;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer0[rx_wr_index0++]=data;
#if RX_BUFFER_SIZE0 == 256
   // special case for receiver buffer size=256
   if (++rx_counter0 == 0) rx_buffer_overflow0=1;
#else
   if (rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   if (++rx_counter0 == RX_BUFFER_SIZE0)
      {
      rx_counter0=0;
      rx_buffer_overflow0=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter0==0);
data=rx_buffer0[rx_rd_index0++];
#if RX_BUFFER_SIZE0 != 256
if (rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
#endif
#asm("cli")
--rx_counter0;
#asm("sei")
return data;
}
#pragma used-
#endif

// USART Transmitter buffer
#define TX_BUFFER_SIZE0 64
char tx_buffer0[TX_BUFFER_SIZE0];

#if TX_BUFFER_SIZE0 <= 256
unsigned char tx_wr_index0=0,tx_rd_index0=0;
#else
unsigned int tx_wr_index0=0,tx_rd_index0=0;
#endif

#if TX_BUFFER_SIZE0 < 256
unsigned char tx_counter0=0;
#else
unsigned int tx_counter0=0;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
#if TX_BUFFER_SIZE0 != 256
   if (tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
#if TX_BUFFER_SIZE0 != 256
   if (tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
#endif
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>
//---------------------------------------------


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
//------------------------------------------------------------------------------------------
unsigned char nutSW1, nutSW2, nutSW1truoc, nutSW2truoc;  // Variables to hold the state of the switches
unsigned char demNut1; // Variable to hold the count of button presses for switch 1 and switch 2
unsigned char nutSW3, nutSW3truoc; // Switch 3 (PORTC.0) state + previous state
unsigned char trangThai = 0;           // 0 = system OFF, 1 = system ON
unsigned char nhietdo = 0; // Variable to hold the temperature value received from UART
//------------------------------------------------------------------------------------------

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

// Function to send the temperature value over UART
void guinhietdo(unsigned char nhietdo) {
    putchar(nhietdo/10 + 48); // Send the tens digit as a character over UART
    putchar(nhietdo%10 + 48); // Send the ones digit as a character over UART
    putchar(186); // Degree symbol
    putchar('C'); // Celsius symbol
}

// Function to send an unsigned char value over UART
void gui_unchar(unsigned char so) {
    putchar(so/100 + 48); // Send the hundreds digit as a character over UART
    putchar((so/10)%10 + 48); // Send the tens digit as a character over UART
    putchar(so%10 + 48); // Send the ones digit as a character over UART

}

// Function to send an unsigned int value over UART
void gui_unint(unsigned int so) {
    putchar(so/10000 + 48); // Send the ten-thousands digit as a character over UART
    putchar((so/1000)%10 + 48); // Send the thousands digit as a character over UART
    putchar((so/100)%10 + 48); // Send the hundreds digit as a character over UART
    putchar((so/10)%10 + 48); // Send the tens digit as a character over UART
    putchar(so%10 + 48); // Send the ones digit as a character over UART
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
    
    // USART initialization
    // Communication Parameters: 8 Data, 1 Stop, No Parity
    // USART Receiver: On
    // USART Transmitter: On
    // USART0 Mode: Asynchronous
    // USART Baud Rate: 9600
    UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
    UCSR0B=(1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
    UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
    UBRR0H=0x00;
    UBRR0L=0x67;


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

    DDRC.0 = 0; // Set PORTC pin 0 as input (power button)
    PORTC.0 = 1; // Enable pull-up resistor on PORTC pin 0

    Display7SEGMENT(20); // Turn on all segments
    delay_ms(1000);     // Wait for 1000 ms
    Display7SEGMENT(16); // Turn off all segments
    delay_ms(500);     // Wait for 500 ms

    nutSW1 = PIND.4; // Read the state of switch 1 (PORTD pin 4)
    nutSW2 = PIND.7; // Read the state of switch 2 (PORTD pin 7)
    nutSW1truoc = nutSW1; // Store the initial state of switch 1
    nutSW2truoc = nutSW2; // Store the initial state of switch 2
    nutSW3 = PINC.0; // Read the initial state of switch 3 (PORTC pin 0)
    nutSW3truoc = nutSW3; // Store the initial state of switch 3
    
    // Global enable interrupts
    #asm("sei")
    
    putchar('S'); 
    putchar('E');
    putchar('T');
    putchar('U');
    putchar('P');
    putchar(10);  
    putchar(13);

    
    while (1)
    {
        nutSW1truoc = nutSW1; // Store the previous state of switch 1
        nutSW2truoc = nutSW2; // Store the previous state of switch 2
        nutSW1 = PIND.4; // Read the state of switch 1 (PORTD pin 4)
        nutSW2 = PIND.7; // Read the state of switch 2 (PORTD pin 7)

        nutSW3truoc = nutSW3; // Store the previous state of switch 3
        nutSW3 = PINC.0; // Read the state of switch 3 (PORTC pin 0)

        if (nutSW3 == 0 && nutSW3truoc == 1) // Falling edge = switch 3 pressed
        {
            trangThai = !trangThai;              // Latch ON <-> OFF
            if (trangThai) 
            {
                Display7SEGMENT(nhietdo % 10); // ON: show the ones digit of the last temperature
                putchar('O');
                putchar('N');
                putchar(10);
                putchar(13);
                putchar(7); // Bell sound
            }
            else 
            {
                Display7SEGMENT(16); // OFF: blank display
                putchar('O');
                putchar('F');
                putchar('F');
                putchar(10);
                putchar(13);
                putchar(7); // Bell sound
            }
        }

        if (trangThai) // SW1/SW2 only work when the system is ON
        {
            if (nutSW2 == 0 && nutSW2truoc == 1) // If PORTD pin 7 is LOW (button pressed) and was HIGH before
            {
                if (demNut1 < 10) // Only act if below 30 degrees C
                {
                    demNut1++; // Increase, stops at 10 (30 degrees C)
                    nhietdo = demNut1 + 20; // Convert the count to a temperature value
                    putchar(nhietdo/10 + 48); // Send the count as a character over UART
                    putchar(nhietdo%10 + 48); // Send the count as a character over UART
                    putchar(186); // Degree symbol
                    putchar('C'); // Celsius symbol
                    putchar(10); // New line
                    putchar(13); // Carriage return
                    Display7SEGMENT(nhietdo % 10); // Display the ones digit of the temperature on the 7-segment LED
                }
            }
            if (nutSW1 == 0 && nutSW1truoc == 1) // If PORTD pin 4 is LOW (button pressed) and was HIGH before
            {
                if (demNut1 > 0) // Only act if above 20 degrees C
                {
                    demNut1--; // Decrease, stops at 0 (20 degrees C)
                    nhietdo = demNut1 + 20; // Convert the count to a temperature value
                    putchar(nhietdo/10 + 48); // Send the count as a character over UART
                    putchar(nhietdo%10 + 48); // Send the count as a character over UART
                    putchar(186); // Degree symbol
                    putchar('C'); // Celsius symbol
                    putchar(10); // New line
                    putchar(13); // Carriage return
                    Display7SEGMENT(nhietdo % 10); // Display the ones digit of the temperature on the 7-segment LED
                }
            }
        }


        delay_ms(20); // Wait for 20 ms to debounce the button press
    }
}