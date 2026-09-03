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
uint32_t tdHientai, tdTruoc, chuky;
int16_t tanso;
uint16_t vantoc;
uint32_t quangduong;
uint8_t lockCua;
uint8_t DieukhienThucong;
uint8_t lockCuaTruoc;
uint32_t thoidiemDENtruoc;
uint8_t TrangthaiNUT1, TrangthaiNUT2;
uint8_t NUT1truoc, NUT2truoc;
uint32_t demXung2;
uint32_t quangduong2;


void ngatngoai1() {
  demXung ++; // Biến dùng để xác định trip của xe
  demXung2 ++; // Biến dùng để reset lại quãng đường xe mỗi lần dừng, đảm bảo nếu dừng thì quãng đường sẽ reset về 0, tránh trường hợp xe vừa qua 10km/h là lock cửa
  tdTruoc = tdHientai; // Cập nhật thời điểm trước
  tdHientai = micros(); // Lấy thời điểm hiện tại khi có xung
  chuky = tdHientai - tdTruoc; // Tính chu kỳ
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai1, FALLING); // Gán ngắt cho chân 3
  Monitor.begin(19200);
  chuky = 10000000; // Khởi tạo chu kỳ lớn để tránh chia cho 0, sẽ đơ chương trình
}

void loop() {

  // Bánh xe có 20 xung, chu vi 2m, nên mỗi xung tương ứng với 0.1m quãng đường
  // Nhận diện cạnh lên (vừa nhấn) từ giá trị nút đã giải mã ở vòng lặp trước
  TrangthaiNUT1 = (NUT1 == 1 && NUT1truoc == 0) ? 1 : 0;
  TrangthaiNUT2 = (NUT2 == 1 && NUT2truoc == 0) ? 1 : 0;


  // Lưu trạng thái nút hiện tại làm trạng thái trước cho vòng lặp sau
  NUT1truoc = NUT1;
  NUT2truoc = NUT2;

  
  if (micros() - tdHientai > 360000) 
  {
    vantoc = 0; // Nếu không có xung trong 0.36 giây thì tốc độ = 0
  } 
  else 
  {
    vantoc = 360000 / chuky; // Tính tốc độ từ chu kỳ
  }
  quangduong = demXung / 10; // Tính quãng đường từ số xung, mỗi xung tương ứng với 0.1m quãng đường

  if (NUT4 == 1)
  {
    demXung = 0; // Nếu nút nhấn NUT4 được nhấn thì reset số xung về 0
    demXung2 = 0; // Reset biến đếm xung thứ hai
    quangduong = 0; // Reset quãng đường về 0
  }

  // Dieu khien khoa cua: thu cong uu tien hon tu dong
  if (TrangthaiNUT1 == 1) 
  {
    lockCua = 0; // NUT1 mo khoa thu cong
    DieukhienThucong = 1;
  }
  else if (TrangthaiNUT2 == 1) 
  {
    lockCua = 1; // NUT2 khoa thu cong
    DieukhienThucong = 1;
  }

  // Khoa tu dong khi vantoc > 10 va quangduong2 > 50, chi khi chua bi dieu khien thu cong ghi de
  if (!DieukhienThucong && vantoc > 10 && demXung2 > 500 && lockCua == 0) 
  {
    lockCua = 1; // Khoa cua tu dong
  }

  // Khi dieu kien tu dong khong con thoa thi cho phep tu dong kich hoat lai
  // Xe dung lai, van toc = 0, demXung2 = 0, dieu khien thu cong = 0, cho phep tu dong kich hoat lai
  if (vantoc == 0)
  {
    DieukhienThucong = 0;
    demXung2 = 0; // Reset biến đếm xung thứ hai khi điều kiện tự động không còn thỏa
  }

  // Tao xung 2s cho DEN khi trang thai khoa thay doi
  if (lockCua != lockCuaTruoc)
  {
    if (lockCua == 1)
    {
      DEN2 = 1; DEN1 = 0; // Khoa -> DEN2 sang 2s
      thoidiemDENtruoc = millis();
    }
    else
    {
      DEN1 = 1; DEN2 = 0; // Mo khoa -> DEN1 sang 2s
      thoidiemDENtruoc = millis();
    }
    lockCuaTruoc = lockCua;
  }
  if (millis() - thoidiemDENtruoc >= 2000)
  {
    DEN1 = 0; DEN2 = 0;
  }
  

  goimaytinh1 = vantoc; // Gửi tốc độ lên App
  goimaytinh2 = demXung2 / 10; // Gửi quãng đường lên App
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