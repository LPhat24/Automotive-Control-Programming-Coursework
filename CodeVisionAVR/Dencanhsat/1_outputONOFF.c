
#include <mega328p.h>
#include <delay.h>
#include <stdint.h>

uint16_t delay_time_ms = 100;

void main(void)
{
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

DDRD.5 = 1; // PORTD pin 5 as output
DDRD.6 = 1; // PORTD pin 6 as output

while (1)
      {
      unsigned char i;

      // Flash PORTD.5 three times
      for (i = 0; i < 3; i++)
            {
            PORTD.5 = 1; // LED 1 on
            delay_ms(delay_time_ms);
            PORTD.5 = 0; // LED 1 off
            delay_ms(delay_time_ms);
            }

      // Flash PORTD.6 three times
      for (i = 0; i < 3; i++)
            {
            PORTD.6 = 1; // LED 2 on
            delay_ms(delay_time_ms);
            PORTD.6 = 0; // LED 2 off
            delay_ms(delay_time_ms);
            }
      }
}
