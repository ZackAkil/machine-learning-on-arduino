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

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

bool buttonPressed(){
  return digitalRead(button);
}

void saveDataPoint(){
  Serial.println("hello");
}


void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(sensor);

  if (buttonPressed()){
    while(buttonPressed()){
      delay(10);
    }
    saveDataPoint();

    ledState = !ledState;

    Serial.println(ledState);

    digitalWrite(led, ledState);
  }
  
}
