char kytunhan;

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  Serial.println("Bắt đầu ....");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) 
  {
    kytunhan = Serial.read();
    if (kytunhan == '1') {
      digitalWrite(6, HIGH);
      Serial.println("LEDa sang");
    } 
    if (kytunhan == '0') 
    {
      digitalWrite(6, LOW);
      Serial.println("LEDa tat");
    }
    if (kytunhan == '3') 
    {
      digitalWrite(5, HIGH);
      Serial.println("LEDb sang");
    }
    if (kytunhan == '2') 
    {
      digitalWrite(5, LOW);
      Serial.println("LEDb tat");
    }
  }
}
