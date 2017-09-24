int analogPin = A0;   // nemsensor analog output
float val = 0;        // variable to store the value read
int setValue = 15;    // TARGET RATE

void setup()
{
  pinMode(7, OUTPUT); // relay digital output
  
  Serial.begin(9600); //debug
}

void loop()
{
//  int sensor = digitalRead(8);
//  if(sensor) {
//    digitalWrite(7,HIGH);
//    Serial.print("toprak kuru, vana açılıyor.\n");
//  } else {
//    digitalWrite(7,LOW);
//    Serial.print("toprak nemli, vana kapatılıyor.\n");
//  }
  val = analogRead(analogPin);
  float rate = 100 - (val / 1023 * 100);
  Serial.print("toprak nemi: %");
  Serial.println((int)rate);
  Serial.print("hedef nem: %");
  Serial.println((int)setValue);
  if(rate > setValue) {
    digitalWrite(7,LOW);
    Serial.println("Sulama Kapalı");
  } else {
    digitalWrite(7,HIGH);   
    Serial.println("Sulama Açık"); 
  }
  Serial.println("");  
  delay(5000);
} 
