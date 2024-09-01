#include <BeeLineSensorPro.h>
#include "arduinoFFT.h"

#define SAMPLES 128             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.
#define in1 3
#define in2 4
#define M1pwm 5
#define in3 6
#define in4 7 
#define M2pwm 8


arduinoFFT FFT = arduinoFFT();

BeeLineSensorPro sensor = BeeLineSensorPro((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
}, LINE_WHITE);


unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

void setup() 
{
    Serial.begin(115200); //Baud rate for the Serial Monitor
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
    Serial.begin(115200);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(M1pwm, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(M2pwm, OUTPUT);

    for(int i=0;i<500;i++){
      sensor.calibrate();
      delay(5); }

}

float kp=0.06;
float kd=0.5;
int last_value;
int rfreq=0;

void loop() {  

  rfreq=freq();

  if (900<rfreq && rfreq<1100){
   
    analogWrite(M1pwm,0);
    analogWrite(M2pwm,0);
    }
  else{
    for(int n=0;n<30;n++){
      linefollowing();
    }
    analogWrite(M1pwm,0);
    analogWrite(M2pwm,0);
  }

}

int freq(){
   /*Sample SAMPLES times*/
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 

        vReal[i] = analogRead(A9); //Reads the value from analog pin 9 (A9), quantize it and save it as a real term.
        vImag[i] = 0; //Makes imaginary term 0 always

        /*remaining wait time between samples if necessary*/
        while(micros() < (microSeconds + samplingPeriod))
        {
          //do nothing
        }
    }

    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    /*Find peak frequency and print peak*/
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    Serial.println(peak);     //Print out the most dominant frequency.
    return peak;  
}

void linefollowing(){
  //mdrive (255, 255) 
  int err=sensor.readSensor();
  for(int i=0;i<8;i++){
    Serial.print(sensor.values[i]);
    Serial.print('\t');

  }

  Serial.println(err);

  int m1=205;
  int m2=205;


  int diff=err*kp+(err-last_value)*kd;

  last_value=err;
  mdrive(m1-diff,m2+diff);
  //j=j+1;

  /*if( j>=15) {
    mdrive(0,0);
    delay(35);
    j=0;*/



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
