#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10

#define _DUTY_MIN 1220 // servo full clockwise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 1800 // servo full counterclockwise position (180 degree)
#define _ANGLE_NEU 400
#define INTERVAL 1
#define _DIST_ALPHA 0.8

#define KP 1.2
#define KI 0
#define kD 0

float min_dist = 500;
float max_dist = 0; 
float dist_ema, alpha;
unsigned long last_sampling_time; // unit: ms
float dist_arr[5] = {0,};
Servo myservo;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
  
  alpha = _DIST_ALPHA;
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  last_sampling_time = 0;
// initialize serial port
  Serial.begin(115200);

}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val - (0.07)*val;
}

void loop() {
  int n = 5;
  float dist_raw = ir_distance();
  
  if (dist_raw > 400){dist_raw = 400;}
  else if (dist_raw<100){dist_raw = 100;}

  for (int i = 0; i < n; i++){
    if (i == n-1){
        dist_arr[i] = dist_raw;
    }
    else{
      dist_arr[i] = dist_arr[i+1];
    }
    dist_ema = alpha*dist_arr[i] + (1-alpha)*dist_ema;
  }


  float tmp[5] = {0,};
  for (int j = 0; j < n; j++){
    tmp[j] = dist_arr[j];
  }
  for(int i=n-1; i>0; i--){
    for(int j=0; j<i; j++){
      if(tmp[j]<tmp[j+1]){
        float temp = tmp[j];
        tmp[j] = tmp[j+1];
        tmp[j+1] = temp;
      }
    }
  }
  dist_raw = tmp[n/2];

  float error = _ANGLE_NEU - dist_raw;
  float pid = _DUTY_NEU + KP*error;
  if (pid > _DUTY_MAX){pid = _DUTY_MAX;}
  else if (pid<_DUTY_MIN){pid = _DUTY_MIN;}
  myservo.writeMicroseconds(pid);

  Serial.print("Min:100,raw:");
  Serial.print(dist_raw);
  Serial.print(",servo:");
  Serial.print(myservo.read());
  Serial.println(",Max:400");
  last_sampling_time += INTERVAL;
}
