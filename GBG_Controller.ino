/*==========================================================================
// Author : Quinn Angrick
// Project : Go Baby Go Power Wheels Controller
// Description : 
// Source-Code : GBG.ino
// Program: Control DC motors using BTS7960 H Bridge Driver.
//==========================================================================
*/

#include "gpio.h"
#include "bluetooth.h"

// States
bool FWD = 0;
bool RVR = 0;
bool LEFT = 0;
bool RIGHT = 0;
bool ESTOP = 0;
int drive_speed = 255;
int steer_speed = 127;
bool bt_in_control = false;

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

void setup() {
  Serial.begin(9600);

  pinMode(S_RPWM_Output, OUTPUT);
  pinMode(S_LPWM_Output, OUTPUT);
  pinMode(D_RPWM_Output, OUTPUT);
  pinMode(D_LPWM_Output, OUTPUT);
  pinMode(FWD_Input, INPUT_PULLDOWN);
  pinMode(RVR_Input, INPUT_PULLDOWN);
  pinMode(LEFT_Input, INPUT_PULLDOWN);
  pinMode(RIGHT_Input, INPUT_PULLDOWN);

  init_bluetooth();
}
void loop() {
  digital_update(FWD_Input, FWD);
  digital_update(RVR_Input, RVR);
  digital_update(LEFT_Input, LEFT);
  digital_update(RIGHT_Input, RIGHT);

  if (RIGHT == HIGH) {
    analogWrite(S_RPWM_Output, steer_speed);
    analogWrite(S_LPWM_Output, 0);
  } else if (LEFT == HIGH) {
    analogWrite(S_LPWM_Output, steer_speed);
    analogWrite(S_RPWM_Output, 0);
  } else {
    analogWrite(S_LPWM_Output, 0);
    analogWrite(S_RPWM_Output, 0);
  }

  if (FWD == HIGH) {
    analogWrite(D_RPWM_Output, drive_speed);
    analogWrite(D_LPWM_Output, 0);
    Serial.println(drive_speed);
  } else if (RVR == HIGH) {
    analogWrite(D_LPWM_Output, drive_speed);
    analogWrite(D_RPWM_Output, 0);
  } else {
    analogWrite(D_LPWM_Output, 0);
    analogWrite(D_RPWM_Output, 0);
  }
}