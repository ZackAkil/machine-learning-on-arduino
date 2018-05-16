/*
Machine Learning on Arduino

A simple project that has the Arduino learning
when to turn on an LED using data collected 
from the user.

*/


const int sensor = 0;
const int button = 8;
const int led = 9;

// input varible
int sensorValue = 0;

// output varible
bool ledState = false;

// learnt varibles that are optimised
int learnt_thresh = 700;
bool learnt_direction_is_greater = true;

// data storage varibles 
int sensorValStore[10];
int stateStore[10];

int storeCursor = 0;
bool storeFilled = false;

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

  sensorValStore[storeCursor] = sensorVal;
  stateStore[storeCursor] = state;

  storeCursor ++;

  if (storeCursor >= 10){
    storeFilled = true;
    storeCursor = 0;
  }
}

void printStore(){

for(int i=0; i<10; i++){
  Serial.print(sensorValStore[i]);
  Serial.print(',');
}
Serial.println("");

for(int i=0; i<10; i++){
  Serial.print(stateStore[i]);
  Serial.print(',');
}
Serial.println("");

}

bool predict_with_params(int sensorVal, int thresh, bool direction_is_greater){

  if (direction_is_greater){
    return (sensorVal >= thresh);
  }else{
    return (sensorVal < thresh);
  }
}

bool predict(int sensorVal){
  return  predict_with_params(sensorVal, learnt_thresh, learnt_direction_is_greater);
}


int score(int thresh, bool direction_is_greater){

  int correct = 0;

  int countTo = storeFilled ? 9 : storeCursor-1;

    for(int i = 0; i <= countTo; i++){
      Serial.print(sensorValStore[i]);
      Serial.print("<--->");
      Serial.println(stateStore[i]);

       bool pred = predict_with_params(sensorValStore[i], thresh, direction_is_greater);
       if (pred ==  stateStore[i]){
        correct++;
       }
    }
  return correct;
}

void optimise(){
// do brute force serach to optimial coef and intercept values

}


void loop() {
  // put your main code here, to run repeatedly:
  
 sensorValue = analogRead(sensor);

 ledState = predict(sensorValue);
 Serial.println("led state");
 Serial.println(ledState);

  if (buttonPressed()){
    Serial.println(sensorValue);
    while(buttonPressed()){

      delay(10);
    }

    saveDataPoint(sensorValue, !ledState);

     printStore();

    for(int i = 0; i<10; i++){
      Serial.println("----");
        Serial.println(i);
        Serial.println(score(i*100, true));
    }
     
  }

  digitalWrite(led, ledState);

  delay(400);
}
