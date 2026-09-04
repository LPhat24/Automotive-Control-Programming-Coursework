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
uint16_t vitriSet;
int16_t vitriHientai; // đơn vị là số lần gọi hàm = 4 bước
int16_t kqADC6;
int16_t nhietdo;
uint8_t vitriCamchung;
uint8_t khoangNhietdo;
uint8_t bandoCamchung[10] = {114, 119, 126, 132, 136, 140, 144, 149, 154, 154}; // Bản đồ vị trí cầm chừng

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

  // Tra bản đồ nhiệt độ -> vị trí cầm chừng
  kqADC6 = analogRead(A6); // Read the analog value from A6
  nhietdo = map (kqADC6, 0, 1023, -40, 215); // Map the analog value to a temperature range (-40 to 215 degrees Celsius)

  if (nhietdo < 0) 
  {
    vitriCamchung = 114; // Set the position to 114 if the temperature is below 0
  } 
  else if (nhietdo >= 140) 
  {
    vitriCamchung = 154; // Set the position to 154 if the temperature is above 140
  } 
  else 
  {
    khoangNhietdo = nhietdo / 14; // Calculate the temperature range index
    vitriCamchung = bandoCamchung[khoangNhietdo]; // Set the position based on the temperature range
  }

  vitriSet = vitriCamchung * 2; // Set the target position to the calculated position, multiplied by 2 to account for the step size of the motor


  // Điều khiển motor bước dựa trên giá trị vitriCamchung và vitriHientai
  if (NUT1)
  {
    chaynguoc(5);
  }
  else 
  {
    // Điều khiển vị trí motor bước dựa trên giá trị vitriCamchung và vitriHientai
    if (vitriHientai < vitriSet)
    {
      vitriHientai  ++; //tăng vị trí lên 1
      chaythuan(5);
   }
    else if (vitriHientai > vitriSet)
   {
     vitriHientai --; //giảm vị trí xuống 1
     chaynguoc(5);
   }
   else
   {
     dung(5);
   }
  }

  // --------------------------------------------

  goimaytinh1 = nhietdo; //gửi nhiệt độ lên App
  goimaytinh2 = vitriCamchung; //gửi vị trí cầm chừng lên App
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