//--7 SEGMENT LED--
#define LEDa 6  
#define LEDb 8  
#define LEDc 9  
#define LEDd 5  
#define LEDe 10 
#define LEDf 11 
#define LEDg 12 

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

// Segment pins in the same order as rows of the digit[] table: a,b,c,d,e,f,g
const unsigned char segPins[7] = { LEDa, LEDb, LEDc, LEDd, LEDe, LEDf, LEDg };

// Function to display a digit on the 7-segment LED
void Display7SEGMENT(unsigned char num)
{
    unsigned char k;

    if (num == 16)          // blank the display
    {
        for (k = 0; k < 7; k++) digitalWrite(segPins[k], LOW);
    }
    else if (num == 20)     // all segments on (test pattern)
    {
        for (k = 0; k < 7; k++) digitalWrite(segPins[k], HIGH);
    }
    else if (num < 10)      // digits 0..9 from the pattern table
    {
        for (k = 0; k < 7; k++) digitalWrite(segPins[k], digit[num][k]);
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode (LEDa, OUTPUT);
  pinMode (LEDb, OUTPUT);
  pinMode (LEDc, OUTPUT);
  pinMode (LEDd, OUTPUT);
  pinMode (LEDe, OUTPUT);
  pinMode (LEDf, OUTPUT);
  pinMode (LEDg, OUTPUT);

  Display7SEGMENT(20); // Turn on all segments
  delay(1000);     // Wait for 1000 ms

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 10; i++)
  {
    Display7SEGMENT(i); // Display digits 0 to 9
    delay(200);     // Wait for 200 ms
  }
  Display7SEGMENT(16); // Turn off all segments
  delay(500);     // Wait for 500 ms

}
