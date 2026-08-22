
char kytunhan;
String chuoiNhan;

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
      if (chuoiNhan == "1on")
      {
        digitalWrite(6, HIGH);
        Serial.println("LEa sang");
      }
      if (chuoiNhan == "1off")
      {
        digitalWrite(6, LOW);
        Serial.println("LEa tat");
      }
      if (chuoiNhan == "2on")
      {
        digitalWrite(5, HIGH);
        Serial.println("LEa sang");
      }
      if (chuoiNhan == "2off")
      {
        digitalWrite(5, LOW);
        Serial.println("LEa tat");
      }
      chuoiNhan = "";
    }
  }
  delay (100);

}
