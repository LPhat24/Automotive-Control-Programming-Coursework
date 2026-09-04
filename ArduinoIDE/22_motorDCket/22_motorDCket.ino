//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//----------CHƯƠNG TRÌNH ĐIỀU KHIỂN MOTOR DC----------------
//------TrT Monitor----------------------------------------
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
#define ENA 5 //chân ENA điều khiển tốc độ motor DC - d PWM 0-255
#define IN1 11 //chân IN1 điều khiển chiều quay motor DC - f
#define IN2 10 //chân IN2 điều khiển chiều quay motor DC - e

#define ENB 6 //chân ENB điều khiển tốc độ motor DC - a PWM 0-255
#define IN3 9 //chân IN3 điều khiển chiều quay motor DC - c
#define IN4 8 //chân IN4 điều khiển chiều quay motor DC - b

//------Định nghĩa hàm, ví dụ: digitalWrite -> B"x"on hoặc B"x"off--------------------------


//-----------------Application Variable-----------------------
volatile int32_t demXung, demXungSet; //biến đếm xung từ encoder 
uint32_t tdHientai, tdTruoc, chuky; //biến thời gian hiện tại, thời gian trước, chu kỳ
int32_t tanso, tansoTruoc; //biến tần số
int32_t giatoc;
int8_t lenXuong; //biến cờ lên xuống


//-----------------Function-----------------------------------
// Hàm điều khiển motor DC
void DKmotor1 (int16_t x) 
{
  x = constrain(x, -186, 186); //giới hạn tốc độ motor DC do 186 + 69 = 255
  if (x > 0) 
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, x + 69); //giá trị tay ga từ 1-255 -> tốc độ motor DC từ 70-255

  } 
  else if (x < 0) 
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -x + 69);

  } 
  else 
  {
    //analogWrite(ENA, 0); // Quay tự do, IN1 và IN2 không quan trọng
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 170); // Hãm điện động, IN1 và IN2 ngắn mạch

  }
}

// Hàm xử lý ngắt ngoài 1
void ngatngoai1() {
  if (digitalRead(2) == HIGH) 
  {
    demXung++; // Tăng số xung đếm được khi động cơ quay thuận
  } 
  else 
  {
    demXung--; // Giảm số xung đếm được khi động cơ quay ngược
  }

  tdTruoc = tdHientai; // Cập nhật thời điểm trước
  tdHientai = micros(); // Lấy thời điểm hiện tại khi có xung
  chuky = tdHientai - tdTruoc; // Tính chu kỳ
}

//=========================================================
void setup() {
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai1, FALLING); // Gán ngắt cho chân 3
  chuky = 10000000; // Khởi tạo chu kỳ lớn để tránh chia cho 0
  Monitor.begin(19200);

}
//=========================================================
void loop() {

  tansoTruoc = tanso; // Cập nhật tần số trước
  if (micros() - tdHientai > 1000000) 
  {
    // Kiểm tra nếu đã đủ 1 giây
    tanso = 0;
  } 
  else 
  {
    tanso = 1000000 / chuky; // Tính tần số từ chu kỳ
  }

  giatoc = (tanso - tansoTruoc); // Không cần phải chia deltaT vì nó chỉ là hằng số, kết quả là đơn vị không biết 

  if (NUT1)
  {
    demXungSet = 3000; // Set số xung đếm được khi nhấn nút 1
    lenXuong = 1; // Cờ lên
  }
  if (NUT4)
  {
    demXungSet = 0; // Set số xung đếm được khi nhấn nút 4
    lenXuong = -1; // Cờ xuống
  }

  // Kiểm tra kẹt
  if (giatoc < -25) 
  {
    if (lenXuong == -1)
    {
      demXungSet = demXung; // Nếu kẹt, giữ nguyên số xung đếm được
      lenXuong = 0; // Reset cờ xuống
    }
    else if (lenXuong == 1 && demXung < 2950) // Vì lên hết sẽ bị giật lùi (nhầm là bị kẹt) nên phải có demXung < 2950
    {
      DKmotor1(0); // Dừng motor DC nếu kẹt khi đang lên
      delay(1000); // Dừng 1 giây
      demXungSet = demXung - 300; // Giảm số xung đếm được để motor DC quay xuống 300 xung
      if (demXungSet < 0) demXungSet = 0; // Không cho số xung đếm được âm
      lenXuong = 0; // Reset cờ lên
    }
  }

  //------------------------------------------------
  if (demXung < (demXungSet - 50))
  {
    DKmotor1(100); // Quay thuận
  }
  else if (demXung > (demXungSet + 50))
  {
    DKmotor1(-100); // Quay ngược
  }
  else
  {
    DKmotor1(0); // Dừng motor DC
  }

  


  // --------------------------------------------
  goimaytinh1 = demXung; // Gửi số xung đếm được lên App
  goimaytinh2 = giatoc; // Gửi số xung Target lên App
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
  while (millis() - thoidiem < 20) { ; } //tối thiểu 20ms để đồng bộ truyền nhận Arduino & App
  thoidiem = millis();
  //---------------------------------------------------------------------------------------------------------
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx