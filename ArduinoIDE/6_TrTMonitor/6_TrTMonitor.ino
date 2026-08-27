// HAHA
#include <TrTmonitor.h>

TrTmonitor Monitor(0);

//khai báo các biến để đồng bộ truyền nhận Arduino và App
int16_t goimaytinh1, goimaytinh2; //gửi lên đồng hồ READ1 & READ2 của App
uint8_t DENgoimaytinh; //gửi hiển thị DEN trên App
uint8_t nhanMaytinh1, nhanMaytinh2, nutMaytinh; //WRITE1 & WRITE2 && nút nhấn trên App

uint32_t thoidiem;

uint16_t kqADC6, kqADC7; //kết quả đọc ADC6 & ADC7 của Arduino


uint8_t LEDgoimaytinh; //gửi hiển thị LED trên App

//Khai báo các biến bit để dễ xử lý
unsigned char NUT1, NUT2, NUT3, NUT4;
unsigned char DEN1, DEN2, DEN3, DEN4;

unsigned char nutSW1, nutSW2, nutSW1truoc, nutSW2truoc;  // Variables to hold the state of the switches
uint16_t kqADC0;

#define NUT3_ADC_MIN 80
#define NUT3_ADC_MAX 100
#define NUT4_ADC_MIN 523
#define NUT4_ADC_MAX 543

bool nutSW3pressed = false, nutSW3pressed_truoc = false;
bool nutSW4pressed = false, nutSW4pressed_truoc = false;


void setup() {
  pinMode(A6, INPUT); pinMode(A7, INPUT);
  pinMode(4, INPUT_PULLUP); pinMode(7, INPUT_PULLUP);
  Monitor.begin(19200);
}

void loop() {

  // DENgoimaytinh = nutMaytinh; 
  // goimaytinh1 = nhanMaytinh1; goimaytinh2 = nhanMaytinh2;
  nutSW1truoc = nutSW1; // Store the previous state of switch 1
  nutSW2truoc = nutSW2; // Store the previous state of switch 2
  nutSW1 = digitalRead(4); // Read the current state of switch 1
  nutSW2 = digitalRead(7); // Read the current state of switch 2

  kqADC6 = analogRead(A6); kqADC7 = analogRead(A7); kqADC0 = analogRead(A0); // Read the analog values from A6, A7, and A0
  goimaytinh1 = kqADC0; goimaytinh2 = kqADC7; 

  // Detect nutSW3 (ADC 80-100)
  nutSW3pressed_truoc = nutSW3pressed;
  nutSW3pressed = (kqADC0 >= NUT3_ADC_MIN && kqADC0 <= NUT3_ADC_MAX);

  // Detect nutSW4 (ADC 523-543)
  nutSW4pressed_truoc = nutSW4pressed;
  nutSW4pressed = (kqADC0 >= NUT4_ADC_MIN && kqADC0 <= NUT4_ADC_MAX);

  // Toggle DEN3 on nutSW3 rising edge
  if (nutSW3pressed && !nutSW3pressed_truoc) {
    DEN3 ^= 1;
  }

  // Toggle DEN4 on nutSW4 rising edge
  if (nutSW4pressed && !nutSW4pressed_truoc) {
    DEN4 ^= 1;
  }

  if (nutSW2 == 0 && nutSW2truoc == 1) // If PORTD pin 7 is LOW (button pressed) and was HIGH before
  {
    DEN1 ^= 1;
  }
  if (nutSW1 == 0 && nutSW1truoc == 1) // If PORTD pin 4 is LOW (button pressed) and was HIGH before
  {
    DEN2 ^= 1;
  }
  // đóng gói các bit vào byte trước khi gửi
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; 


  //đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);
  
  while (millis() - thoidiem < 20) { //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
    ;
  }
  thoidiem = millis();
}