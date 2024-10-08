#pragma once

// Motor Driver Pins
const int S_RPWM_Output = 2;  //STEER RIGHT
const int S_LPWM_Output = 3;  //STEER LEFT
const int D_RPWM_Output = 4;  //DRIVE FORWARD
const int D_LPWM_Output = 5;  //DRIVE REVERSE

// Driver Pins
const int FWD_Input = 6;  //FWD SHIFTER/JOYSTICK FWD
const int RVR_Input = 7;  //REVERSE SHIFT/JOYSTICK REVERSE
const int RIGHT_Input = 8;
const int LEFT_Input = 9;

void init_gpio(){
  pinMode(S_RPWM_Output, OUTPUT);
  pinMode(S_LPWM_Output, OUTPUT);
  pinMode(D_RPWM_Output, OUTPUT);
  pinMode(D_LPWM_Output, OUTPUT);
  pinMode(FWD_Input, INPUT_PULLDOWN);
  pinMode(RVR_Input, INPUT_PULLDOWN);
  pinMode(LEFT_Input, INPUT_PULLDOWN);
  pinMode(RIGHT_Input, INPUT_PULLDOWN);
}

void digital_update(const int &pin, bool &state) {
  bool new_state = digitalRead(pin);
  if (new_state != state) {
    Serial.print("pin ");
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(new_state);
  }
  state = new_state;
}

void analog_update(const int &pin, int &val) {
  int new_val = analogRead(pin);
  if (abs(new_val - val) > 20) {
    Serial.print("pin ");
    Serial.print(pin);
    Serial.print("new_val: ");
    Serial.println(new_val);
  }
  val = new_val;
}

