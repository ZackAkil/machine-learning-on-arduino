/*
Machine Learning on Arduino

A simple project that has the Arduino learning
when to turn on an LED using data collected 
from the user.

*/

const int sensor = A0;
const int button = 9;
const int led = 8;

// input varible
int sensor_value = 0;

// output varible
bool led_state = false;

// learnt varibles that are optimised
int learnt_thresh = 700;
bool learnt_direction_is_greater = true;

// data storage varibles
const int data_store_size = 10;

int sensore_val_store[data_store_size];
int state_store[data_store_size];

int store_cursor = 0;
bool store_filled = false;

void setup(){
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
}

bool button_pressed(){
  return digitalRead(button);
}

void debounce_button(){
  while (button_pressed()){
    delay(50);
  }
}

void save_data_point(int sensor_val, bool state){
  sensore_val_store[store_cursor] = sensor_val;
  state_store[store_cursor] = state;

  store_cursor++;

  if (store_cursor >= data_store_size){
    store_filled = true;
    store_cursor = 0;
  }
}

bool predict_with_params(int sensor_val, int thresh, bool direction_is_greater){
  if (direction_is_greater){
    return (sensor_val >= thresh);
  }
  else{
    return (sensor_val < thresh);
  }
}

bool predict(int sensor_val){
  return predict_with_params(sensor_val, learnt_thresh, learnt_direction_is_greater);
}

int score(int thresh, bool direction_is_greater){

  int correct_count = 0;

  // if the store array isn't full, then only read upto the store_cursor   
  int count_to = store_filled ? data_store_size : store_cursor;

  for (int i = 0; i < count_to; i++){
    bool pred = predict_with_params(sensore_val_store[i], thresh, direction_is_greater);
    if (pred == state_store[i]){
      correct_count++;
    }
  }
  return correct_count;
}

void optimise(){
  // do brute force serach to optimial coef and intercept values
  int best_score = 0;
  int current_score = 0;

  for (int direction = 0; direction <= 1; direction++){
    for (int thresh = 0; thresh < 1000; thresh += 30){

      current_score = score(thresh, direction);

      if (current_score > best_score){
        learnt_thresh = thresh;
        learnt_direction_is_greater = direction;
        best_score = current_score;
      }
    }
  }
}

void loop(){

  sensor_value = analogRead(sensor);
  led_stateState = predict(sensor_value);
  digitalWrite(led, led_stateState);

  if (button_pressed()){
    debounce_button();
    save_data_point(sensor_value, !led_state);
    optimise();
  }
}
