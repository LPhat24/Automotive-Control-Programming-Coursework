//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//----------CHƯƠNG TRÌNH ĐIỀU KHIỂN MOTOR BƯỚC----------------
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
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B3, OUTPUT);
  pinMode(B4, OUTPUT);

}
//=========================================================
void loop() {
  for (uint16_t i = 0; i < 512; i++) {
    chaythuan(5); // Chạy thuận với thời gian delay 5 ms
  }
  dung(5); // Dừng với thời gian delay 5 ms
  delay(1000); // Dừng 1 giây
  for (uint16_t i = 0; i < 512; i++) {
    chaynguoc(5); // Chạy ngược với thời gian delay 5 ms
  }
  dung(5); // Dừng với thời gian delay 5 ms
  delay(1000); // Dừng 1 giây

}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx