#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

uint8_t dem = 0;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
const char* MESSAGE = "LeThanhPhat";
const int MESSAGE_LEN = 11;
const unsigned long SCROLL_DELAY_MS = 180;

void showOnRow(int row, int startCol)
{
	char line[LCD_COLS + 1];

	for (int i = 0; i < LCD_COLS; i++) {
		line[i] = ' ';
	}
	line[LCD_COLS] = '\0';

	for (int i = 0; i < MESSAGE_LEN; i++) {
		int col = startCol + i;
		if (col >= 0 && col < LCD_COLS) {
			line[col] = MESSAGE[i];
		}
	}

	lcd.setCursor(0, row);
	lcd.print(line);
}

void clearRow(int row)
{
	lcd.setCursor(0, row);
	lcd.print("                ");
}

void HelloWorld()
{
	lcd.setCursor(0, 0);
	lcd.print("Hello, World!");
}

void scrollingText()
{
	// First line: scroll from left to right.
	clearRow(1);
	for (int pos = -MESSAGE_LEN; pos <= LCD_COLS; pos++) {
		showOnRow(0, pos);
		delay(SCROLL_DELAY_MS);
	}

	// Second line: scroll from right to left.
	clearRow(0);
	for (int pos = LCD_COLS; pos >= -MESSAGE_LEN; pos--) {
		showOnRow(1, pos);
		delay(SCROLL_DELAY_MS);
	}
}

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the backlight.
	lcd.backlight();
	lcd.clear();
	HelloWorld();
}

void loop()
{
	dem++;
	lcd.setCursor(0, 1);
	lcd.print(dem/100);
	lcd.print(dem/10%10);
	lcd.print(dem%10);
	delay(100);
}

