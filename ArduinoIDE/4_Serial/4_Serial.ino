#include <Arduino.h>

uint8_t dem = 0;
void setup() {
    Serial.begin(9600);
    Serial.println("Hello, World!");
}

void loop() {
    dem++;
    Serial.println(dem);
    delay(200); // Delay for 200 milliseconds
}
