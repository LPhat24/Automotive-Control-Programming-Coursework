#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char message[] = "Hello, World!!";
const uint8_t msgLen = sizeof(message) - 1;
const uint8_t cols = 16;

void showFrame(uint8_t row, int offset)
{
	char line[17];
	for (uint8_t col = 0; col < cols; col++)
	{
		int idx = col - offset;
		line[col] = (idx >= 0 && idx < msgLen) ? message[idx] : ' ';
	}
	line[cols] = '\0';
	lcd.setCursor(0, row);
	lcd.print(line);
}

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	showFrame(0, -(int)msgLen);
}

void loop()
{
	// Line 1: scroll left -> right (enters at left edge, exits at right edge)
	for (int pos = -(int)msgLen; pos <= (int)cols; pos++)
	{
		showFrame(0, pos);
		delay(200);
	}

	// Line 2: scroll right -> left (enters at right edge, exits at left edge)
	for (int pos = (int)cols; pos >= -(int)msgLen; pos--)
	{
		showFrame(1, pos);
		delay(200);
	}
}
