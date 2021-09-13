#define PIN_LED 13
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("Hello Worldd!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}

int toggle_state(int togle){
  if (toggle==0){
     toggle = 1;
  }
  else{
    toggle = 0;
  }
  return toggle;
}
