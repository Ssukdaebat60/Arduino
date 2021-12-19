#include <Servo.h>

// Arduino pin assignment
#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10

#define _DUTY_MIN 1812 // servo full clockwise position
#define _DUTY_NEU 1962 // servo neutral position
#define _DUTY_MAX 2112 // servo full counterclockwise position
#define _ANGLE_NEU 255

#define INTERVAL 100
#define _DIST_ALPHA 0.5

#define KP 0.39         //0.66, 0.45
#define KI 0.00052      //
#define KD 24.8           //21 13

float dist_raw = 0;
float min_dist = 500;
float max_dist = 0;
float error = 0;
float iterm = 0;
int duty_target = _DUTY_NEU;
int duty_curr = _DUTY_NEU;
float dist_ema, alpha;
unsigned long last_sampling_time; // unit: ms
int dist_target = _ANGLE_NEU;
float dist_arr[17] = {0,};
Servo myservo;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
  
  alpha = _DIST_ALPHA;
  myservo.attach(PIN_SERVO);
  Serial.print(myservo.read());
  myservo.writeMicroseconds(_DUTY_NEU);
  last_sampling_time = 0;
// initialize serial port
  float temp = ir_distance();
  for (int i = 0; i < 17; i++){
    dist_arr[i] = temp;
  }
  error = 0;
  Serial.begin(115200);

}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return (1.05 * val) - 5;
}

void loop() {
  float past_dist = dist_raw;
  float error_prev = error;
  int n = 17;
  dist_raw = ir_distance();
  
  if (dist_raw > 500 || dist_raw<00){dist_raw = past_dist;}
  
  for (int i = 0; i < n; i++){
    if (i == n-1){
        if (dist_raw - past_dist > 50 || past_dist- dist_raw > 50){dist_raw =dist_raw*0.1 +  past_dist+0.9;}
        dist_arr[i] = dist_raw;
    }
    else{
      dist_arr[i] = dist_arr[i+1];
    }
    dist_ema = alpha*dist_arr[i] + (1-alpha)*dist_ema;
  }
  dist_arr[16] = dist_ema;

  float tmp[17] = {0,};
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

  error = _ANGLE_NEU - dist_raw;
  float pterm = KP*error;
  float dterm = KD*(error - error_prev);
  iterm += KI * error;
  if (iterm > 10) iterm == 10;
  if (iterm < -10) iterm == -10;
  
  duty_curr = _DUTY_NEU + pterm + dterm + iterm;
  if (duty_curr > _DUTY_MAX){duty_curr = _DUTY_MAX;}
  else if (duty_curr < _DUTY_MIN){duty_curr = _DUTY_MIN;}
  myservo.writeMicroseconds(duty_curr);

  Serial.print("IR:");
  Serial.print(dist_raw);
  Serial.print(",T:");
  Serial.print(dist_target);
  Serial.print(",P:");
  Serial.print(map(pterm, -1000, 1000, 510, 610));
  Serial.print(",D:");
  Serial.print(map(dterm, -1000, 1000, 510, 610));
  Serial.print(",I:");
  Serial.print(map(iterm, -1000, 1000, 510, 610));
  Serial.print(",DTT:");
  Serial.print(map(duty_target, -1000, 1000, 510, 610));
  Serial.print(",DTC:");
  Serial.print(map(duty_curr, -1000, 1000, 510, 610));
  Serial.println(",-G:245, +G:265, m:0, M:800");
  
  last_sampling_time += INTERVAL;
  Serial.print("\n");
}
