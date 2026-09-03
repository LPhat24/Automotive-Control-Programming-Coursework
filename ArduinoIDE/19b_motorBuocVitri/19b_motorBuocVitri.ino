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
int16_t vitriSet, viTri; // đơn vị là số lần gọi hàm = 4 bước

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

  if (NUT3)
  {
    vitriSet = 0; //nếu nhấn nút 3 thì đặt vị trí về 0
  }
  else if (NUT4)
  {
    vitriSet = 170; //nếu nhấn nút 4 thì đặt vị trí về 170
  }
  
  if (NUT1)
  {
    chaynguoc(5);
  }
  else 
  {
    // Điều khiển vị trí motor bước dựa trên giá trị vitriSet và viTri
    if (viTri < vitriSet)
    {
      viTri ++; //tăng vị trí lên 1
      chaythuan(5);
   }
    else if (viTri > vitriSet)
   {
     viTri --; //giảm vị trí xuống 1
     chaynguoc(5);
   }
   else
   {
     dung(5);
   }
  }


  // Điều khiển vị trí motor bước dựa trên giá trị vitriSet và viTri
  if (viTri < vitriSet)
  {
    viTri ++; //tăng vị trí lên 1
    chaythuan(5);
  }
  else if (viTri > vitriSet)
  {
    viTri --; //giảm vị trí xuống 1
    chaynguoc(5);
  }
  else
  {
    dung(5);
  }
  // --------------------------------------------

  goimaytinh1 = vitriSet; //gửi vị trí hiện tại lên App
  goimaytinh2 = viTri; //gửi vị trí hiện tại lên App
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