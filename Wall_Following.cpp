#include <NewPing.h>

#define SONAR_NUM   1          //define Number of ultrasonic sensor used

#define MAX_DISTANCE 800       //Max distance between the object and robot

#define PING_INTERVAL 33           



#define trigPin1 26        // define pin connections for sensor and motor

#define echoPin1 28    

#define in1 3
#define in2 4
#define M1pwm 5
#define in3 6
#define in4 7 
#define M2pwm 8
  

int rangeWall  = 0;



NewPing sonar[SONAR_NUM] = {                // Define a Newping array to measure the distance

  NewPing(trigPin1, echoPin1, MAX_DISTANCE);



};

void setup()   // setup function to make pin configuration to define whether it is input or output

{

  Serial.begin(9600);

  int i;

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(M1pwm, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(M2pwm, OUTPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  digitalWrite(trigPin1, LOW);

}



int toCloseWall = 1800;  //Initialze and define the values for distance between wall and robot

int toFarWall = 2500;

void loop()

{

Main:

  rangeWall = readRangeWall();

  Serial.print(rangeWall);

  Serial.print(" Wall");

  Serial.println(); 

  
  //As the sensor value reads the small values we are manipulating to high values

  if(rangeWall <= 400)

  {
    
    rangeWall = 3000;
    
  }


  if(rangeWall > toCloseWall && rangeWall < toFarWall) //condition to check distance measured by front and side sensor is maintained correctly

  {

    drive_forward();

    Serial.print(" Drive Forward");

    Serial.println();

    goto Main;

  } 



  if (rangeWall < toCloseWall)                       //condition to check side wall is close to robot

  {

    delay(100);

    turn_left();

    //turn_right();

    delay(500);

    Serial.print(" Turn Left");

    drive_forward();

    Serial.print(" Drive Forward");

    Serial.println();

    goto Main;

  }

  if (rangeWall > toFarWall)                         //condition to check robot is far from side wall

  {

    delay(100);

    turn_right();

    //turn_left();

    Serial.print(" Turn Right");

    delay(500);

    drive_forward();

    Serial.print(" Drive Forward");

    Serial.println();

    goto Main;

  } 

}

                
void drive_forward()                               // function to drive the robot forward

{   

    digitalWrite(in1, HIGH);

    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);

    digitalWrite(in4, LOW);

    analogWrite(M1pwm,50);
    analogWrite(M2pwm,50);

    

}



void turn_left()                                  // function to turn the robot left

{  

    digitalWrite(in1, HIGH);

    digitalWrite(in2, LOW);

    digitalWrite(in3, High);

    digitalWrite(in4, LOW);

    analogWrite(M1pwm,50);
    analogWrite(M2pwm,0);

}



void turn_right()                                     //function to turn the robot right

{

    digitalWrite(in1, LOW);

    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);

    digitalWrite(in4, LOW);

    analogWrite(M1pwmpwm,0);
    analogWrite(M2pwm,50);

}




int readRangeWall()                                 // function to read the left sensor value

{

  delay(50);

  unsigned rangeWall = sonar.ping();

  sonar.timer_stop();

  return rangeWall;

}
