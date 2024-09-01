#include <Servo.h>
#include <NewPing.h>
#include <BeeLineSensorPro.h>   

#define MAX_DISTANCE 1000       //Max distance between the object and robot

#define PING_INTERVAL 33   

#define trigPin3 32       

#define echoPin3 30

#define in1 3
#define in2 4
#define M1pwm 5
#define in3 6
#define in4 7 
#define M2pwm 8

BeeLineSensorPro sensor = BeeLineSensorPro((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
}, LINE_WHITE);


NewPing sonar = {   // Define a Newping array to measure the distance
  NewPing(trigPin3, echoPin3, MAX_DISTANCE)
};

Servo myServo;  // create a Servo object to control a servo motor

int servoPin = 34;  // define the pin to which the servo is connected
int x = 10000;

void setup() 
{
  Serial.begin(115200);
  myServo.attach(servoPin);  // attaches the servo to the specified pin
  myServo.write(90);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  digitalWrite(trigPin3, LOW);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(M1pwm, OUTPUT);
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 
  pinMode(M2pwm, OUTPUT); 


  for(int i=0;i<200;i++)
  {
    sensor.calibrate();
    delay(5);
  }

}
float kp=0.06;
float kd=0.9;
int last_value;
int k=0;

void loop() 
{
x = readboxdistance();

if( 150 <= x && x <= 174)
  {
    analogWrite(M1pwm,0);
    analogWrite(M2pwm,0);
    delay(1500);
    arm_down();
    for(int n=0;n<80;n++){
    reverselinefollowing();
    }
  }

else 
{
sensor.readSensor();
if (sensor.values[7] < 200) {
  while (sensor.values[7]<200) {
    mdrive(170,170);
    sensor.readSensor();
  }
  if (sensor.values[3]<200 || sensor.values[4]<200){
    for(int n=0;n<10;n++){
    linefollowing()
      }
  }
  else {
    for(int n=0;n<40;n++){
    turn_right();
    }
    for(int n=0;n<10;n++){
    linefollowing();
    }
    }
  }

else{
  for(int n=0;n<20;n++){
  linefollowing();
    }
}
}
}

void arm_down()
{
  myServo.write(180);
  delay(500);
}

void arm_up()
{
  myServo.write(90);
  delay(500);
}


int readboxdistance()                                 // function to read the left sensor value
{
delay(50);
unsigned boxdistance = sonar.ping();
sonar.timer_stop();
Serial.println(boxdistance);
Serial.println("   box distance");
return boxdistance;
}

void linefollowing()
  {
    int err=sensor.readSensor();
    for(int i=0;i<8;i++){
      Serial.print(sensor.values[i]);
      Serial.print('\t');
    }
  Serial.println(err);

  int m1=185;
  int m2=185;

  int diff=err*kp+(err-last_value)*kd;

  last_value=err;
  mdrive(m1-diff,m2+diff);
  }

void reverselinefollowing()
{
  int err=sensor.readSensor();
  for(int i=0;i<8;i++){\i
    Serial.print(sensor.values[i]);
    Serial.print('\t');
  }
Serial.println(err);

int m1=185;
int m2=185;

int diff=err*kp+(err-last_value)*kd;

last_value=err;
mdrivereverse(m1-diff,m2+diff);
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
  if (m1 < 0) {
    m1 = 0;
    }
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(M1pwm,m1);
  }

  if (m2 > 0) {
  if (m2 > 255) {
      m2 = 255;
  }
    digitalWrite (in3, HIGH); 
    digitalWrite (in4, LOW); 
    analogWrite (M2pwm, 255-m2) ;
  } else {
  if (m2 < 0) {
    m2 = 0;
    }
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    analogWrite(M2pwm,m2);
  }

  void mdrivereverse (int m1, int m2) {
   if (m1 > 0) {
    if (m1 > 255) {
        m1 = 255;
    }
      digitalWrite (in2, HIGH);
      digitalWrite (in1, LOW);  
      analogWrite (M1pwm, 255-m1) ;
    } 
   else {
    if (m1 < 0) {
      m1 = 0;
      }
      digitalWrite(in2,LOW);
      digitalWrite(in1,HIGH);
      analogWrite(M1pwm,m1);
    }

    if (m2 > 0) {
    if (m2 > 255) {
        m2 = 255;
    }
      digitalWrite (in4, HIGH); 
      digitalWrite (in3, LOW); 
      analogWrite (M2pwm, 255-m2) ;
    } else {
    if (m2 < 0) {
      m2 = 0;
      }
      digitalWrite(in4,LOW);
      digitalWrite(in3,HIGH);
      analogWrite(M2pwm,m2);
    }
}
