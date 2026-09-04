//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//----------CHƯƠNG TRÌNH ĐIỀU KHIỂN MOTOR DC----------------
//------TrT Monitor----------------------------------------
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
#define ENA 5 //chân ENA điều khiển tốc độ motor DC - d PWM 0-255
#define IN1 11 //chân IN1 điều khiển chiều quay motor DC - f
#define IN2 10 //chân IN2 điều khiển chiều quay motor DC - e

#define ENB 6 //chân ENB điều khiển tốc độ motor DC - a PWM 0-255
#define IN3 9 //chân IN3 điều khiển chiều quay motor DC - c
#define IN4 8 //chân IN4 điều khiển chiều quay motor DC - b

//------Định nghĩa hàm, ví dụ: digitalWrite -> B"x"on hoặc B"x"off--------------------------


//-----------------Application Variable-----------------------
uint8_t tayga; //giá trị tay ga nhận từ App
int16_t giatriTocdo; //tốc độ motor DC


//-----------------Function-----------------------------------
void DKmotor1 (int16_t x) 
{
  x = constrain(x, -186, 186); //giới hạn tốc độ motor DC do 186 + 69 = 255
  if (x > 0) 
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, x + 69); //giá trị tay ga từ 1-255 -> tốc độ motor DC từ 70-255
  } 
  else if (x < 0) 
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -x + 69);
  } 
  else 
  {
    analogWrite(ENA, 0); // Quay tự do, IN1 và IN2 không quan trọng
  }
}

//=========================================================
void setup() {
  Monitor.begin(19200);
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  nhanMaytinh1 = 128; // Set 128 để ban đầu nó dừng

}
//=========================================================
void loop() {

  tayga = nhanMaytinh1;  // Nhận giá trị tay ga từ App

  if (tayga <= 110)
  {
    giatriTocdo = map(tayga, 0, 110, -186, -1); //giá trị tay ga từ 0-110 -> tốc độ motor DC từ -186 đến -1
    DKmotor1(giatriTocdo);
  }
  else if (tayga >= 145)
  {
    giatriTocdo = map(tayga, 145, 255, 1, 186); //giá trị tay ga từ 145-255 -> tốc độ motor DC từ 1 đến 186
    DKmotor1(giatriTocdo);
  }
  else
  {
    giatriTocdo = 0; //đặt tốc độ motor DC là 0
    DKmotor1(giatriTocdo);
  }

  // --------------------------------------------
  goimaytinh1 = tayga; //gửi giá trị tay ga lên App
  goimaytinh2 = giatriTocdo; //gửi tốc độ motor DC lên App
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