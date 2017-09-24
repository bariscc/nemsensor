int analogPin = A0;      // nemsensor analog output
int current_mo = 0;      // variable to store the value read
int target_mo = 15;      // target moisture

void setup()
{
  pinMode(7, OUTPUT);    // relay digital output  
  Serial.begin(9600);    // debug
}

void loop()
{
  current_mo = analogRead(analogPin);
  current_mo = map(current_mo,1023,0,0,100);
  
  Serial.print("mevcut nem: %");
  Serial.println(current_mo);
  Serial.print("hedef nem: %");
  Serial.println(target_mo);
  
  if(current_mo > target_mo) {
    digitalWrite(7,LOW);
    Serial.println("Sulama Kapalı");
  } else {
    digitalWrite(7,HIGH);   
    Serial.println("Sulama Açık"); 
  }
  Serial.println("");
  delay(5000);
} 
