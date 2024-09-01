#include <BeeLineSensorPro.h>

#define in1 3
#define in2 4
#define M1pwm 5
#define in3 6
#define in4 7 
#define M2pwm 8

BeeLineSensorPro sensor = BeeLineSensorPro((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
}, LINE_WHITE);

void setup () {
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(M1pwm, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(M2pwm, OUTPUT);

  for(int i=0;i<500;i++){
    sensor.calibrate();
    delay(5); 

  }


 /* for(int i=0; i<100;i++) {
    sensor.calibrate();
    mdrive(-120,120);
  }

  mdrive(0,0);
  delay(100);

  for(int i=0; i<200;i++) {
    sensor.calibrate();
    mdrive(120,-120);
  }
  mdrive(0,0);
  delay(100);

  for(int i=0; i<100;i++) {
    sensor.calibrate();
    mdrive(-120,120);
  }
  mdrive(0,0);
  delay(1000);*/

}

float kp=0.03;
float kd=0.5;
float ki=0;
int last_value;
int j=0;
int i=0;

void loop ()  {
//mdrive (255, 255) 
  int err=sensor.readSensor();
  for(int i=0;i<8;i++){
    Serial.print(sensor.values[i]);
    Serial.print('\t');

  }

  Serial.println(err);

  int m1=160;
  int m2=160;

  i=i+err;

  int diff=err*kp+(err-last_value)*kd+i*ki;

  last_value=err;
  mdrive(m1-diff,m2+diff);
  j=j+1;

  if( j>25) {
    mdrive(0,0);
    delay(25 );
    j=0;

  }

}

void mdrive (int m1, int m2) {
 if (m1 > 0) {
  if (m1 > 255) {
      m1 = 255;
  }
    digitalWrite (in1, HIGH);
    digitalWrite (in2, LOW);  
    analogWrite (M1pwm, 255-m1) ;
  } else {
  if (m1 < -255) {
    m1 = -255;
    }
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(M1pwm,0);
  }

  if (m2 > 0) {
  if (m2 > 255) {
      m2 = 255;
  }
    digitalWrite (in3, HIGH); 
    digitalWrite (in4, LOW); 
    analogWrite (M2pwm, 255-m2) ;
  } else {
  if (m2 < -255) {
    m2 = -255;
    }
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    analogWrite(M2pwm,0);
  }
}
