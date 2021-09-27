int ledPin = 9;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int fadeValue = 0; fadeValue<=255; fadeValue +=5){
    analogWrite(ledPin, fadeValue);
    delay(500);
  }
  for(int fadeValue = 255; fadeValue>0; fadeValue -=5){
    analogWrite(ledPin, fadeValue);
    delay(500);
  } 
}
