//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#include <TrTmonitor.h>
TrTmonitor Monitor(0);
//---------------------------------------
//khai báo các biến để đồng bộ truyền nhận Arduino và App
int16_t goimaytinh1, goimaytinh2; //gửi lên đồng hồ READ1 & READ2 của App
uint8_t DENgoimaytinh; //gửi hiển thị DEN trên App
uint8_t nhanMaytinh1, nhanMaytinh2, nutMaytinh; //WRITE1 & WRITE2 && nút nhấn trên App

//Khai báo các biến bit để dễ xử lý
unsigned char NUT1, NUT2, NUT3, NUT4;
unsigned char DEN1, DEN2, DEN3, DEN4;
//---------------------------------------
void setup() {
  pinMode(4,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);

  Monitor.begin(19200);
}
//---------------------------------------
void loop() {

  //thu thập dữ liệu vào các biến để gửi lên máy tính
  DEN1 = !digitalRead(4); DEN2 = !digitalRead(7); DEN3 = 0; DEN4 = 1-DEN4;
  goimaytinh1 = analogRead(A6); goimaytinh2 = analogRead(A7);
  
  // đóng gói các bit vào byte trước khi gửi
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; 

  //đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);

  //chuyển đổi byte nhận vào bit để tiện sử dụng
  NUT1 = (nutMaytinh&1);
  NUT2 = (nutMaytinh&2)>>1;
  NUT3 = (nutMaytinh&4)>>2;
  NUT4 = (nutMaytinh&8)>>3;
  
  //điều khiển theo lệnh máy tính
  analogWrite(5,nhanMaytinh1); analogWrite(6,nhanMaytinh2);
  if (NUT1 == 1) {digitalWrite(8,HIGH);}  else  {digitalWrite(8,LOW);}
  if (NUT2 == 1) {digitalWrite(9,HIGH);}  else  {digitalWrite(9,LOW);}
  if (NUT3 == 1) {digitalWrite(11,HIGH);} else  {digitalWrite(11,LOW);}
  if (NUT4 == 1) {digitalWrite(12,HIGH);} else  {digitalWrite(12,LOW);}

  delay(20);
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx