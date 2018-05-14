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

bool predict(int sensorVal){

  bool state_prediction =  (sensorVal * learnt_coef + learnt_intercept) > 0.8 ;

  return state_prediction;
}


int score(float coef, float interc){

  int correct = 0;

  int countTo = storeFilled ? 9 : storeCursor;


    for(int i = 0; i <= countTo; i++){
       if (((sensorValStore[i] * coef + interc) > 0.5) ==  stateStore[i]){
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

  if (buttonPressed()){
    while(buttonPressed()){
      delay(10);
    }

    saveDataPoint(sensorValue, !ledState);

     printStore();

    for(int i = 0; i<100; i++){
      Serial.println("----");
        Serial.println(i);
        Serial.println(score(0.0001 * i, 0.0));
    }
     
  }

  digitalWrite(led, ledState);
}
