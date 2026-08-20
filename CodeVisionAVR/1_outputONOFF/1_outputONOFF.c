
#include <mega328p.h>
#include <delay.h>

void main(void)
{
int delay_time = 500; // Initial delay time in milliseconds
int delay_step = -20; // Step to adjust the delay time

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

while (1)
      {
      // Place your code here
      PORTD.6 = 1; // Turn PORTD pin 6 on
      delay_ms(delay_time); // Wait for the current period
      PORTD.6 = 0; // Turn PORTD pin 6 off
      delay_ms(delay_time); // Wait for the current period

      delay_time += delay_step; // Adjust the delay time for the next cycle
      if (delay_time <= 20)     // Check if the delay time is below the minimum threshold
            {
            delay_time = 20; // Set a minimum delay time
            delay_step = 20; // Change direction to increase delay time
            }
      else if (delay_time >= 500) // Check if the delay time is above the maximum threshold
            {
            delay_time = 500; // Set a maximum delay time
            delay_step = -20; // Change direction to decrease delay time
            }
      }
}
