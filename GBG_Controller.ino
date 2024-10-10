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

int steer_speed = 127;

void checkBtUnpair(){
  if (touchRead(bt_Pair_Input)<75) {
    digitalWrite(LED, HIGH);
    BP32.forgetBluetoothKeys();
    Serial.println("unpaired controller");
  }else{
    digitalWrite(LED, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  init_gpio();
  init_bluetooth();
}
void loop() {
  checkBtUnpair();
  
  // This call fetches all the controllers' data.
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }

  if (bt_turn == btRIGHT || (digitalRead(RIGHT_Input) && !lock_joystick)) {
    analogWrite(S_RPWM_Output, steer_speed);
    analogWrite(S_LPWM_Output, 0);
    Serial.println("Turning Right");
  } else if (bt_turn == btLEFT || (digitalRead(LEFT_Input) && !lock_joystick)) {
    analogWrite(S_LPWM_Output, steer_speed);
    analogWrite(S_RPWM_Output, 0);
    Serial.println("Turning Left");
  } else {
    analogWrite(S_LPWM_Output, 0);
    analogWrite(S_RPWM_Output, 0);
  }

  if (bt_drive == btFWD || (digitalRead(FWD_Input) && !lock_joystick)) {
    analogWrite(D_RPWM_Output, drive_speed);
    analogWrite(D_LPWM_Output, 0);
    Serial.println("Driving FWD");
    Serial.println(drive_speed);
  } else if (bt_drive == btRVR || (digitalRead(RVR_Input) && !lock_joystick)) {
    analogWrite(D_LPWM_Output, drive_speed);
    analogWrite(D_RPWM_Output, 0);
    Serial.println("Driving RVR");
  } else {
    analogWrite(D_LPWM_Output, 0);
    analogWrite(D_RPWM_Output, 0);
  }

  // Delay needed for bt
  vTaskDelay(1);
  delay(150);
}