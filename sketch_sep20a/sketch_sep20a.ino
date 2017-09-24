int analogPin = A0;   // nemsensor analog output
int val = 0;        // variable to store the value read
int setVal = 15;      // TARGET RATE

void setup()
{
  pinMode(7, OUTPUT); // relay digital output  
  Serial.begin(9600); // debug
}

void loop()
{
  val = analogRead(analogPin);
  float rate = 100 - ((float)val / 1023 * 100);
  
  Serial.print("toprak nemi: %");
  Serial.println((int)rate);
  Serial.print("hedef nem: %");
  Serial.println(setVal);
  
  if(rate > setVal) {
    digitalWrite(7,LOW);
    Serial.println("Sulama Kapalı");
  } else {
    digitalWrite(7,HIGH);   
    Serial.println("Sulama Açık"); 
  }
  Serial.println("");
  delay(5000);
} 
