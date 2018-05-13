/*
Machine Learning on Arduino

A simple project that has the Arduino learning
when to turn on an LED using data collected 
from the user.

*/


const int sensor = 0;
const int button = 8;
const int led = 9;

int sensorValue = 0;

bool ledState = false;

float learnt_coef = 0.001;
float learnt_intercept = 0.001;

int sensorValStore[10];
int stateStore[10];

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

bool buttonPressed(){
  return digitalRead(button);
}

void saveDataPoint(int sensorVal, bool state){

  Serial.println(sensorVal);
  Serial.println(state);
}

bool predict(int sensorVal){

  bool state_prediction =  (sensorVal * learnt_coef + learnt_intercept) > 0.8 ;

  return state_prediction;
}

void optimise(){
// do brute force serach to optimial coef and intercept values

}


void loop() {
  // put your main code here, to run repeatedly:
  
 sensorValue = analogRead(sensor);

  if (buttonPressed()){
    while(buttonPressed()){
      delay(10);
    }

    ledState = !ledState;

    saveDataPoint(sensorValue, ledState);
  }

  digitalWrite(led, predict(sensorValue));
}
