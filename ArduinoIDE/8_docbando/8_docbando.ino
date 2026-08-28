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
int16_t nhietdo;
uint8_t vitriCamchung;
uint8_t khoangNhietdo;
uint8_t bandoCamchung[10] = {114, 119, 126, 132, 136, 140, 144, 149, 154, 154}; // Bản đồ vị trí cầm chừng


void setup() {
  Monitor.begin(19200);
}

void loop() {

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

  goimaytinh1 = nhietdo; // Send the temperature to the computer
  goimaytinh2 = vitriCamchung; // Send the position to the computer
  
  // ----------------------------------------------
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; // đóng gói các bit vào byte trước khi gửi
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

  while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  thoidiem = millis();
  //-----------------------------
}