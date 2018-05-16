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
const int dataStoreSize = 10;

int sensorValStore[dataStoreSize];
int stateStore[dataStoreSize];

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

void debounceButton(){
    while(buttonPressed()){
      delay(50);
    }
}

void saveDataPoint(int sensorVal, bool state){

  sensorValStore[storeCursor] = sensorVal;
  stateStore[storeCursor] = state;

  storeCursor ++;

  if (storeCursor >= dataStoreSize){
    storeFilled = true;
    storeCursor = 0;
  }
}

void printStore(){

for(int i=0; i<dataStoreSize; i++){
  Serial.print(sensorValStore[i]);
  Serial.print(',');
}
Serial.println("");

for(int i=0; i<dataStoreSize; i++){
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

  int correctCount = 0;

  int countTo = storeFilled ? 9 : storeCursor-1;

    for(int i = 0; i <= countTo; i++){
       bool pred = predict_with_params(sensorValStore[i], thresh, direction_is_greater);
       if (pred ==  stateStore[i]){
        correctCount++;
       }
    }
  return correctCount;
}

void optimise(){
// do brute force serach to optimial coef and intercept values
  int best_score = 0;
  int current_score = 0;

  for (int direction = 0; direction <=1; direction++){
    for (int thresh = 0; thresh < 1000; thresh+=30){

      current_score = score(thresh, direction);
      
      if(current_score > best_score){
        learnt_thresh = thresh;
        learnt_direction_is_greater = direction;
        best_score = current_score;
      }
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
 sensorValue = analogRead(sensor);
  Serial.println(sensorValue);
 ledState = predict(sensorValue);
 digitalWrite(led, ledState);

  if (buttonPressed()){
    debounceButton();
    saveDataPoint(sensorValue, !ledState);
    printStore();
    optimise();
  }
}
