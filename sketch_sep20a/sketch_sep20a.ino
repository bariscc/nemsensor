void setup()
{
  pinMode(7, OUTPUT); /* 7 numaralı pini çıkış yaptık */
  pinMode(8, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int sensor = digitalRead(8);
  if(sensor) {
    digitalWrite(7,HIGH);
    Serial.print("toprak kuru, vana açılıyor.\n");
  } else {
    digitalWrite(7,LOW);
    Serial.print("toprak nemli, vana kapatılıyor.\n");
  }
  delay(2000);
} 
