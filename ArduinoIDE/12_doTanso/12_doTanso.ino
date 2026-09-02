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
volatile int32_t demXung = 0;
uint8_t TaySo = 2; // Biến để lưu vị trí tay số
uint8_t TrangthaiNUT1, TrangthaiNUT2, TrangthaiNUT3, TrangthaiNUT4;
uint8_t NUT1truoc, NUT2truoc, NUT3truoc, NUT4truoc;

uint32_t tdHientai, tdTruoc, chuky;
int16_t tanso;

void ngatngoai1() {
  // if (TaySo == 1) 
  // {
  //   demXung++; // Tăng số xung đếm được khi tay số ở vị trí 1
  // }
  // else if (TaySo == 3) 
  // {
  //   demXung--; // Giảm số xung đếm được khi tay số ở vị trí 3
  // }
  demXung ++;

  tdTruoc = tdHientai; // Cập nhật thời điểm trước
  tdHientai = micros(); // Lấy thời điểm hiện tại khi có xung
  chuky = tdHientai - tdTruoc; // Tính chu kỳ
}

void setup() {
  pinMode (5, OUTPUT); // LEDd
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai1, FALLING); // Gán ngắt cho chân 3
  Monitor.begin(19200);
  chuky = 10000000; // Khởi tạo chu kỳ lớn để tránh chia cho 0
}

void loop() {

  if (micros() - tdHientai > 1000000) 
  {
    // Kiểm tra nếu đã đủ 1 giây
    tanso = 0;
  } 
  else 
  {
    tanso = 1000000 / chuky; // Tính tần số từ chu kỳ
  }
  // while (demXung < 10) { ; } // TEST Volatile

  analogWrite (LEDd, (tanso <= 500) ? map(tanso, 0, 500, 0, 255) : 255); // Điều chỉnh độ sáng LED theo tần số

  if (tanso > 400)
  {
    DEN1 = 1; DEN2 = 1; DEN3 = 1; DEN4 = 1;
  }
  else if (tanso > 300)
  {
    DEN1 = 0; DEN2 = 1; DEN3 = 1; DEN4 = 1;
  }
  else if (tanso > 200)
  {
    DEN1 = 0; DEN2 = 0; DEN3 = 1; DEN4 = 1;
  }
  else if (tanso > 100)
  {
    DEN1 = 0; DEN2 = 0; DEN3 = 0; DEN4 = 1;
  }
  else
  {
    DEN1 = DEN2 = DEN3 = DEN4 = 0; // Nếu tần số nhỏ hơn hoặc bằng 100Hz, tắt tất cả LED
  }
  

  goimaytinh1 = tanso; // Gửi tần số lên App
  goimaytinh2 = (tanso <= 500) ? map(tanso, 0, 500, 0, 255) : 255; // Gửi độ sáng LED lên App
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