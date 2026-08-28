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


// Application
int16_t kqADC1;
uint8_t giatriDieukhien;

void setup() {
  pinMode(A1, INPUT);
  pinMode(6, OUTPUT);
  
  Monitor.begin(19200);
}

void loop() {

  kqADC1 = analogRead(A1); // Read the analog value from A1
  // Có thể dùng else if cũng được vì khoảng giữa 450 - 550 không làm gì cả
  if (kqADC1 < 450)
  {
    giatriDieukhien = 0; // Đèn tắt
  }
  if (kqADC1 > 550)
  {
    giatriDieukhien = 1; // Đèn sáng
  }

  digitalWrite(6, giatriDieukhien); // Set the digital output on pin 6 based on the control value

  
  goimaytinh1 = kqADC1; // Send the analog value to the computer
  DEN4 = giatriDieukhien; // Send the control value to the computer
  
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