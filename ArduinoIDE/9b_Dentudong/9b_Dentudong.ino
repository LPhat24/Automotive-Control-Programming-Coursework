#include <TrTmonitor.h>

TrTmonitor Monitor(0);

//--7 SEGMENT LED--
#define LEDa 6  
#define LEDb 8  
#define LEDc 9  
#define LEDd 5  
#define LEDe 10 
#define LEDf 11 
#define LEDg 12 
//khai báo các biến để đồng bộ truyền nhận Arduino và App
int16_t goimaytinh1, goimaytinh2; //gửi lên đồng hồ READ1 & READ2 của App
uint8_t DENgoimaytinh; //gửi hiển thị DEN trên App
uint8_t nhanMaytinh1, nhanMaytinh2, nutMaytinh; //WRITE1 & WRITE2 && nút nhấn trên App
//Khai báo các biến bit để dễ xử lý
unsigned char NUT1, NUT2, NUT3, NUT4;
uint8_t NUT1truoc, NUT2truoc, NUT3truoc, NUT4truoc;
uint8_t TrangthaiNUT1, TrangthaiNUT2, TrangthaiNUT3, TrangthaiNUT4;
unsigned char DEN1, DEN2, DEN3, DEN4;
uint32_t thoidiem;


// Application
int16_t kqADC1;
uint8_t giatriDieukhien;
uint8_t ChedoDen = 0; // Biến để lưu chế độ điều khiển đèn

void setup() {
  pinMode(A1, INPUT);
  pinMode(6, OUTPUT);
  
  Monitor.begin(19200);
}

void loop() {

  kqADC1 = analogRead(A1); // Đọc giá trị cảm biến

  // Nhận diện cạnh lên (vừa nhấn) từ giá trị nút đã giải mã ở vòng lặp trước
  TrangthaiNUT1 = (NUT1 == 1 && NUT1truoc == 0) ? 1 : 0;
  TrangthaiNUT3 = (NUT3 == 1 && NUT3truoc == 0) ? 1 : 0;
  TrangthaiNUT4 = (NUT4 == 1 && NUT4truoc == 0) ? 1 : 0;

  // Lưu trạng thái nút hiện tại làm trạng thái trước cho vòng lặp sau
  NUT1truoc = NUT1;
  NUT3truoc = NUT3;
  NUT4truoc = NUT4;

  // LabVIEW nhấn nút 1,3,4 -> chọn chế độ điều khiển đèn
  if (TrangthaiNUT1 == 1)
  {
    ChedoDen = 1; // Bật chế độ 1
  }
  else if (TrangthaiNUT3 == 1)
  {
    ChedoDen = 3; // Bật chế độ 3
  }
  else if (TrangthaiNUT4 == 1)
  {
    ChedoDen = 4; // Bật chế độ 4
  }

  // Điều khiển đèn dựa trên chế độ
  if (ChedoDen == 1) 
  {
    // Đèn tắt
    DEN1 = 1; DEN3 = 0; DEN4 = 0; // Bật đèn 1 trên LabVIEW
    digitalWrite(LEDa, LOW); // Tắt đèn a
  }
  else if (ChedoDen == 3) 
  {
    // Đèn bật
    DEN1 = 0; DEN3 = 1; DEN4 = 0; // Bật đèn 3 trên LabVIEW
    digitalWrite(LEDa, HIGH); // Bật đèn a
  }
  else if (ChedoDen == 4) 
  {
    // Đèn tự động
    DEN1 = 0; DEN3 = 0; DEN4 = 1; // Bật đèn 4 trên LabVIEW
    if (kqADC1 < 450)
    {
      giatriDieukhien = 0; // Đèn tắt
    }
    else if (kqADC1 > 550)
    {
      giatriDieukhien = 1; // Đèn sáng
    }
    digitalWrite(LEDa, giatriDieukhien); // Điều khiển đèn
  }
  
  goimaytinh1 = kqADC1; // Gửi giá trị cảm biến lên LabVIEW
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
  // if (NUT1 == 1) { digitalWrite(8, HIGH); } else { digitalWrite(8, LOW); }
  // if (NUT2 == 1) { digitalWrite(9, HIGH); } else { digitalWrite(9, LOW); }
  // if (NUT3 == 1) { digitalWrite(10, HIGH); } else { digitalWrite(10, LOW); }
  // if (NUT4 == 1) { digitalWrite(11, HIGH); } else { digitalWrite(11, LOW); }
  // analogWrite(5, nhanMaytinh1); // gửi tín hiệu PWM ra chân 5
  // analogWrite(6, nhanMaytinh2); // gửi tín hiệu PWM ra chân 6

  while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  thoidiem = millis();
}