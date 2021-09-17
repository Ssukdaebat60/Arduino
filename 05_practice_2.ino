#define PIN 7

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(200);
  digitalWrite(PIN7, HIGH);
  delay(1000);
  int num = 0;
  while(1){
    digitalWrite(PIN7, LOW);
    delay(200);
    if (num > 4){
      exit(0);
    }
    digitalWrite(PIN7, HIGH);
    delay(200);
    num++;
  }
}
