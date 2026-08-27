

// HAHA
#include <TrTmonitor.h>

TrTmonitor Monitor(0);

//khai báo các biến để đồng bộ truyền nhận Arduino và App
int16_t goimaytinh1, goimaytinh2; //gửi lên đồng hồ READ1 & READ2 của App
uint8_t DENgoimaytinh; //gửi hiển thị DEN trên App
uint8_t nhanMaytinh1, nhanMaytinh2, nutMaytinh; //WRITE1 & WRITE2 && nút nhấn trên App

uint16_t kqADC6, kqADC7; //kết quả đọc ADC6 & ADC7 của Arduino

void setup() {
  Monitor.begin(19200);
}

void loop() {

  // DENgoimaytinh = nutMaytinh; 
  // goimaytinh1 = nhanMaytinh1; goimaytinh2 = nhanMaytinh2;
  kqADC6 = analogRead(A6); kqADC7 = analogRead(A7);
  goimaytinh1 = kqADC6; goimaytinh2 = kqADC7;

  //đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);
  
  delay(20);
}