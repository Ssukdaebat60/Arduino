#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10

#define _DUTY_MIN 1000 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2000 // servo full counterclockwise position (180 degree)
#define INTERVAL 40

float min_dist = 500;
float max_dist = 0; 
unsigned long last_sampling_time; // unit: ms
Servo myservo;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  last_sampling_time = 0;
// initialize serial port
  Serial.begin(57600);

}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
  float dist_raw = ir_distance();
  
  if (dist_raw > 500){dist_raw = 500;}
  else if (dist_raw<0){dist_raw = 0;}

  if (dist_raw > 255){
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else{
    myservo.writeMicroseconds(_DUTY_MAX);
  }

  Serial.print("Min:100,raw:");
  Serial.print(dist_raw);
  Serial.print(",servo:");
  Serial.print(myservo.read());
  Serial.println(",Max:400");
  last_sampling_time += INTERVAL;
}
