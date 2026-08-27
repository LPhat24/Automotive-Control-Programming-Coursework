// HAHA
#include <TrTmonitor.h>

TrTmonitor Monitor(0);

//khai báo các biến để đồng bộ truyền nhận Arduino và App
int16_t goimaytinh1, goimaytinh2; //gửi lên đồng hồ READ1 & READ2 của App
uint8_t DENgoimaytinh; //gửi hiển thị DEN trên App
uint8_t nhanMaytinh1, nhanMaytinh2, nutMaytinh; //WRITE1 & WRITE2 && nút nhấn trên App
//Khai báo các biến bit để dễ xử lý
unsigned char NUT1, NUT2, NUT3, NUT4;
unsigned char DEN1, DEN2, DEN3, DEN4;
uint32_t thoidiem;
uint32_t prevMillis = 0; // Variable to store the previous time
uint8_t demHienthi = 0; // Variable to count the number of display updates
uint8_t demHienthi2 = 0; // Variable to count the number of display updates

uint16_t kqADC6, kqADC7; //kết quả đọc ADC6 & ADC7 của Arduino
uint8_t Hazard = 0; // 0: OFF, 1: BLINKING
uint8_t HazardCounter = 0; // Counter for DEN3 blinking

uint8_t LEDgoimaytinh; //gửi hiển thị LED trên App



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
  pinMode(5, OUTPUT); pinMode(6, OUTPUT); pinMode(8, OUTPUT); pinMode(9, OUTPUT); pinMode(10, OUTPUT); pinMode(11, OUTPUT);
  
  Monitor.begin(19200);
}

void loop() {
  kqADC6 = analogRead(A6); kqADC7 = analogRead(A7); kqADC0 = analogRead(A0); // Read the analog values from A6, A7, and A0
  goimaytinh1 = kqADC0; goimaytinh2 = kqADC7; 

  // DENgoimaytinh = nutMaytinh; 
  // goimaytinh1 = nhanMaytinh1; goimaytinh2 = nhanMaytinh2;
  nutSW1truoc = nutSW1; // Store the previous state of switch 1
  nutSW2truoc = nutSW2; // Store the previous state of switch 2
  nutSW1 = digitalRead(4); // Read the current state of switch 1
  nutSW2 = digitalRead(7); // Read the current state of switch 2


  // Detect nutSW3 (ADC 80-100)
  nutSW3pressed_truoc = nutSW3pressed;
  nutSW3pressed = (kqADC0 >= NUT3_ADC_MIN && kqADC0 <= NUT3_ADC_MAX);

  // Detect nutSW4 (ADC 523-543)
  nutSW4pressed_truoc = nutSW4pressed;
  nutSW4pressed = (kqADC0 >= NUT4_ADC_MIN && kqADC0 <= NUT4_ADC_MAX);
  demHienthi2 ++;

  // Toggle DEN3 on nutSW3 rising edge
  if (nutSW3pressed && !nutSW3pressed_truoc) 
  {
    Hazard ^= 1;           // Toggle blink mode ON/OFF
    HazardCounter = 0;     // Reset counter
    if (Hazard == 0) DEN3 = 0; // Ensure off when disabled
  }

  // Toggle DEN4 on nutSW4 rising edge
  if (nutSW4pressed && !nutSW4pressed_truoc) 
  {
    // Tín hiệu điều khiển
    //DEN4 ^= 1;
  }

  if (nutSW2 == 0 && nutSW2truoc == 1) // If PORTD pin 7 is LOW (button pressed) and was HIGH before
  {
    DEN1 ^= 1;
  }
  if (nutSW1 == 0 && nutSW1truoc == 1) // If PORTD pin 4 is LOW (button pressed) and was HIGH before
  {
    DEN2 ^= 1;
  }

  // if (millis() - prevMillis >= 200) { // Check if 200ms have passed
  //   prevMillis = millis(); // Update the previous time
  //   DEN4 ^= 1; // Toggle DEN4 every 200ms
  // }
  // Handle DEN3 blinking (500ms cycle = 250ms on/off)
  if (Hazard) 
  {
    HazardCounter++;
    if (HazardCounter >= 10) 
    {  
        // ~200ms (10 * 20ms = 200ms)
        HazardCounter = 0;
        DEN3 ^= 1;
    }
  } else 
  {
    DEN3 = 0;
  }



  demHienthi ++;
  if (demHienthi >= 10) 
  { 
    // Check if 200ms have passed
    demHienthi = 0; // Update the previous time
    DEN4 ^= 1; // Toggle DEN4 every 200ms
  }

  // demHienthi2 ++;
  // if (demHienthi2 >= 25) 
  // { 
  //   // Check if 1s have passed
  //   demHienthi2 = 0; // Update the previous time
  //   DEN3 ^= 1; // Toggle DEN3 every 1s
  // }
  




  // đóng gói các bit vào byte trước khi gửi
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; 


  //đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);

  //chuyển đổi byte nhận vào bit để tiện sử dụng, dịch bit để dưới if có thể cho bằng 0 hoặc 1
  NUT1 = (nutMaytinh&1);
  NUT2 = (nutMaytinh&2)>>1;
  NUT3 = (nutMaytinh&4)>>2;
  NUT4 = (nutMaytinh&8)>>3;
  // LabVIEW nhấn nút 1,2,3,4 thì Arduino sẽ bật tắt LED 8,9,10,11 tương ứng
  if (NUT1 == 1) { digitalWrite(8, HIGH); } else { digitalWrite(8, LOW); }
  if (NUT2 == 1) { digitalWrite(9, HIGH); } else { digitalWrite(9, LOW); }
  if (NUT3 == 1) { digitalWrite(10, HIGH); } else { digitalWrite(10, LOW); }
  if (NUT4 == 1) { digitalWrite(11, HIGH); } else { digitalWrite(11, LOW); }
  analogWrite(5, nhanMaytinh1); // gửi tín hiệu PWM ra chân 5
  analogWrite(6, nhanMaytinh2); // gửi tín hiệu PWM ra chân 6

  while (millis() - thoidiem < 20) { //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
    ;
  }
  thoidiem = millis();
}