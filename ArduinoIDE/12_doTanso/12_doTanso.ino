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


// Application
int32_t demXung = 0;
uint8_t TaySo = 2; // Biến để lưu vị trí tay số
uint8_t TrangthaiNUT1, TrangthaiNUT2, TrangthaiNUT3, TrangthaiNUT4;
uint8_t NUT1truoc, NUT2truoc, NUT3truoc, NUT4truoc;

uint32_t tdHientai, tdTruoc, chuky;
int16_t tanso;

void ngatngoai1() {
  if (TaySo == 1) 
  {
    demXung++; // Tăng số xung đếm được khi tay số ở vị trí 1
  }
  else if (TaySo == 3) 
  {
    demXung--; // Giảm số xung đếm được khi tay số ở vị trí 3
  }

  tdTruoc = tdHientai; // Cập nhật thời điểm trước
  tdHientai = micros(); // Lấy thời điểm hiện tại khi có xung
  chuky = tdHientai - tdTruoc; // Tính chu kỳ
}

void setup() {
  
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai1, FALLING); // Gán ngắt cho chân 3

  Monitor.begin(19200);
}

void loop() {

  tanso = 1000000 / chuky; // Tính tần số từ chu kỳ
  
  // Nhận diện cạnh lên (vừa nhấn) từ giá trị nút đã giải mã ở vòng lặp trước
  TrangthaiNUT1 = (NUT1 == 1 && NUT1truoc == 0) ? 1 : 0;
  TrangthaiNUT2 = (NUT2 == 1 && NUT2truoc == 0) ? 1 : 0;
  TrangthaiNUT3 = (NUT3 == 1 && NUT3truoc == 0) ? 1 : 0;
  TrangthaiNUT4 = (NUT4 == 1 && NUT4truoc == 0) ? 1 : 0;

  // Lưu trạng thái nút hiện tại làm trạng thái trước cho vòng lặp sau
  NUT1truoc = NUT1;
  NUT2truoc = NUT2;
  NUT3truoc = NUT3;
  NUT4truoc = NUT4;

  // LabVIEW nhấn nút 1,3,4 -> chọn chế độ điều khiển đèn
  if (TrangthaiNUT1 == 1)
  {
    TaySo = 1; // Số D
  }
  else if (TrangthaiNUT2 == 1)
  {
    TaySo = 2; // Số N
  }
  else if (TrangthaiNUT3 == 1)
  {
    TaySo = 3; // Số R
  }
  else if (TrangthaiNUT4 == 1)
  {
    TaySo = 4; // Reset số đếm
  }

  if (TaySo == 1) 
  {
    DEN1 = 1; DEN2 = 0; DEN3 = 0; DEN4 = 0; // Bật đèn 1 trên LabVIEW
  } 
  else if (TaySo == 2) 
  {
    DEN1 = 0; DEN2 = 1; DEN3 = 0; DEN4 = 0; // Bật đèn 2 trên LabVIEW
  } 
  else if (TaySo == 3) 
  {
    DEN1 = 0; DEN2 = 0; DEN3 = 1; DEN4 = 0; // Bật đèn 3 trên LabVIEW
  } 
  else if (TaySo == 4) 
  {
    demXung = 0; // Reset số xung đếm được
    TaySo = 2; // Reset tay số về trạng thái ban đầu
    DEN1 = 0; DEN2 = 0; DEN3 = 0; DEN4 = 1; // Bật đèn Reset trên LabVIEW
  }


  goimaytinh1 = demXung; // Gửi số xung đếm được lên App
  goimaytinh2 = tanso; // Gửi tần số lên App
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