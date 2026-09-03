//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//----------CHƯƠNG TRÌNH ĐIỀU KHIỂN MOTOR BƯỚC----------------
//------TrT Monitor   ----------------------------------------
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

//------Khai báo chân-----------------------------------------
#define B1 8 //b
#define B2 9 //c
#define B3 6 //a
#define B4 5 //d
//------Định nghĩa hàm digitalWrite -> B"x"on hoặc B"x"off--------------------------
#define B1on digitalWrite(B1, HIGH)
#define B2on digitalWrite(B2, HIGH)
#define B3on digitalWrite(B3, HIGH)
#define B4on digitalWrite(B4, HIGH)

#define B1off digitalWrite(B1, LOW)
#define B2off digitalWrite(B2, LOW)
#define B3off digitalWrite(B3, LOW) 
#define B4off digitalWrite(B4, LOW)

//-----------------Application Variable-----------------------
uint8_t tayga, delayTocdo; //tayga: nhận giá trị từ App, delayTocdo: tốc độ delay của motor bước
int8_t hienThiTocdo; //hiển thị tốc độ delay trên App

//----------------------------------------------------------
void chaythuan (uint8_t x)
{
  x = constrain(x, 5, 50); // Giới hạn giá trị x từ 5 đến 50 ms
  B1on; B2off; B3off; B4off; delay(x); // Bật cuộn B1, tắt các cuộn còn lại, delay x ms
  B1off; B2on; B3off; B4off; delay(x); // Bật cuộn B2, tắt các cuộn còn lại, delay x ms
  B1off; B2off; B3on; B4off; delay(x); // Bật cuộn B3, tắt các cuộn còn lại, delay x ms
  B1off; B2off; B3off; B4on; delay(x); // Bật cuộn B4, tắt các cuộn còn lại, delay x ms

}
//----------------------------------------------------------
void chaynguoc (uint8_t x)
{
  x = constrain(x, 5, 50); // Giới hạn giá trị x từ 5 đến 50 ms
  // để thứ tự 3,2,1,4 do khi chạy thuận thì dựng lại ở vị trí 4, nên khi chạy nghịch thì phải bắt đầu từ cuộn 3
  B1off; B2off; B3on; B4off; delay(x); // Bật cuộn B3, tắt các cuộn còn lại, delay x ms
  B1off; B2on; B3off; B4off; delay(x); // Bật cuộn B2, tắt các cuộn còn lại, delay x ms
  B1on; B2off; B3off; B4off; delay(x); // Bật cuộn B1, tắt các cuộn còn lại, delay x ms
  B1off; B2off; B3off; B4on; delay(x); // Bật cuộn B4, tắt các cuộn còn lại, delay x ms

}
//----------------------------------------------------------
void dung (uint8_t x)
{
  x = constrain(x, 5, 50); // Giới hạn giá trị x từ 5 đến 50 ms
  B1off; B2off; B3off; B4off; delay(x); delay(x); delay(x); delay(x); // Tắt tất cả các cuộn, delay x ms
}

//=========================================================
void setup() {
  Monitor.begin(19200);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);

}
//=========================================================
void loop() {
  tayga = nhanMaytinh1; //nhận giá trị từ App
  if (tayga <= 110)
  {
    delayTocdo = map(tayga, 0, 110, 5, 50); //chuyển đổi giá trị tay ga từ 0-110 sang tốc độ delay từ 5-50ms
    chaythuan(delayTocdo);
    hienThiTocdo = map(delayTocdo, 5, 50, -100, -1); //chuyển đổi giá trị tốc độ delay từ 5-50ms sang giá trị hiển thị từ -100 đến -1
  }
  else if (tayga >= 145)
  {
    delayTocdo = map(tayga, 145, 255, 50, 5); //chuyển đổi giá trị tay ga từ 145-255 sang tốc độ delay từ 50-5ms
    chaynguoc(delayTocdo);
    hienThiTocdo = map(delayTocdo, 50, 5, 1, 100); //chuyển đổi giá trị tốc độ delay từ 50-5ms sang giá trị hiển thị từ 1 đến 100
  }
  else
  {
    delayTocdo = 5; //đặt tốc độ delay mặc định là 5ms
    dung(delayTocdo); // Dừng với thời gian delay là delayTocdo
    hienThiTocdo = 0; //hiển thị tốc độ là 0
  }

  goimaytinh1 = tayga; //gửi giá trị tay ga lên App
  goimaytinh2 = hienThiTocdo; //gửi giá trị hiển thị tốc độ lên App
  //---------------------------------------------------------------------------------------------------------
  // Đóng gói các bit vào byte trước khi gửi
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; 
  // Đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);
  // Chuyển đổi byte nhận vào bit để tiện sử dụng, dịch bit để dưới if có thể cho bằng 0 hoặc 1
  NUT1 = (nutMaytinh&1);
  NUT2 = (nutMaytinh&2)>>1;
  NUT3 = (nutMaytinh&4)>>2;
  NUT4 = (nutMaytinh&8)>>3;
  // while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  // thoidiem = millis();
  //---------------------------------------------------------------------------------------------------------
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx