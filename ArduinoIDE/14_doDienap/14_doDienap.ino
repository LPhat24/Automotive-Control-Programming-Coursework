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
unsigned char DEN1, DEN2, DEN3, DEN4;
uint32_t thoidiem;


// Application
uint16_t kqADC3;
uint8_t dienAp;
float calibFactor = 12/12.3; // Hệ số hiệu chỉnh điện áp


void setup() {
  Monitor.begin(19200);
}

void loop() {

  kqADC3 = analogRead(A3); // Đọc giá trị ADC từ chân A3
  dienAp = map(kqADC3, 0, 1023, 0, 201*calibFactor); // Chuyển đổi giá trị ADC sang điện áp từ 0 đến 201(đơn vị 0.1V)

  goimaytinh1 = kqADC3; // Gửi giá trị ADC lên App
  goimaytinh2 = dienAp; // Gửi giá trị điện áp lên App
  // Đóng gói các bit vào byte trước khi gửi
  DENgoimaytinh = DEN1 + DEN2*2 + DEN3*4 + DEN4*8; 
  // Đồng bộ Arduino & App (truyền nhận Arduino & App): cần tối thiểu 20ms
  Monitor.synch(nhanMaytinh1, nhanMaytinh2, nutMaytinh, goimaytinh1, goimaytinh2, DENgoimaytinh);

  // Chuyển đổi byte nhận vào bit để tiện sử dụng, dịch bit để dưới if có thể cho bằng 0 hoặc 1
  NUT1 = (nutMaytinh&1);
  NUT2 = (nutMaytinh&2)>>1;
  NUT3 = (nutMaytinh&4)>>2;
  NUT4 = (nutMaytinh&8)>>3;

  while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  thoidiem = millis();
}