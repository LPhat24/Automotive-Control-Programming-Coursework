
char kytunhan;
String chuoiNhan;
int soNhan;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, world!");
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) 
  {
    kytunhan = Serial.read();
    if (kytunhan != '\n')
    {
      chuoiNhan += kytunhan;
    }
    else
    {
      //soNhan = (chuoiNhan[0]-48)*100 + (chuoiNhan[1]-48)*10 + (chuoiNhan[2]-48);
      soNhan = chuoiNhan.toInt();
      Serial.print("Chuoi nhan la: ");
      Serial.println (chuoiNhan);
      Serial.print("So nhan la: ");
      Serial.println(soNhan);
      chuoiNhan = "";
    }
  }
  delay (100);

}
