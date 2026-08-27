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

// Application
int16_t kqADC6;
uint8_t VTA;


void setup() {
  pinMode(A6, INPUT); pinMode(A7, INPUT);
  pinMode(4, INPUT_PULLUP); pinMode(7, INPUT_PULLUP);
  pinMode(5, OUTPUT); pinMode(6, OUTPUT); pinMode(8, OUTPUT); pinMode(9, OUTPUT); pinMode(10, OUTPUT); pinMode(11, OUTPUT);
  
  Monitor.begin(19200);
}

void loop() {

  kqADC6 = analogRead(A6); // Read the analog value from A6
  if (kqADC6 < 102) 
  {
    DEN4 = 1; // Đèn lỗi
    VTA = 30; // Giá trị mặc định khi lỗi
  }
  else if (kqADC6 > 920)
  {
    DEN4 = 1; // Đèn lỗi
    VTA = 30; // Giá trị mặc định khi lỗi
  }
  else 
  {
    DEN4 = 0; // Tắt đèn lỗi
    //VTA = (int32_t)(kqADC6 - 102) * 100 / 818; // Chuyển đổi giá trị ADC sang phần trăm;
    VTA = map(kqADC6, 102, 920, 0, 100); // Chuyển đổi giá trị ADC sang phần trăm;
  }
  
  goimaytinh1 = kqADC6;
  goimaytinh2 = VTA;

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

  while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  thoidiem = millis();
}