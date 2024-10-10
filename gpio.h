#pragma once
#include "esp32-hal-gpio.h"

#define LED 2

// Motor Driver Pins
#define S_RPWM_Output 32  //STEER RIGHT
#define S_LPWM_Output 33  //STEER LEFT
#define D_RPWM_Output 25  //DRIVE FORWARD
#define D_LPWM_Output 26  //DRIVE REVERSE

// Driver Pins
#define FWD_Input 36  //FWD SHIFTER/JOYSTICK FWD
#define RVR_Input 39  //REVERSE SHIFT/JOYSTICK REVERSE
#define RIGHT_Input 34
#define LEFT_Input 35

// BT Pairing Pin
#define bt_Pair_Input 13

void init_gpio(){
  pinMode(LED, OUTPUT);
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

