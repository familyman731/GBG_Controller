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

#define LED 2

// States
bool FWD = 0;
bool RVR = 0;
bool LEFT = 0;
bool RIGHT = 0;
bool ESTOP = 0;
int drive_speed = 0;
int steer_speed = 127;
bool bt_in_control = false;

void checkBtUnpair(){
  if (touchRead(bt_Pair_Input)<75) {
    digitalWrite(LED, HIGH);
    BP32.forgetBluetoothKeys();
  }else{
    digitalWrite(LED, LOW);
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  init_gpio();
  init_bluetooth();
}
void loop() {
  checkBtUnpair();
  
// This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

        vTaskDelay(1);
    delay(150);

  // digital_update(FWD_Input, FWD);
  // digital_update(RVR_Input, RVR);
  // digital_update(LEFT_Input, LEFT);
  // digital_update(RIGHT_Input, RIGHT);

  if (bt_turn == btRIGHT || (RIGHT && !lock_joystick)) {
    analogWrite(S_RPWM_Output, steer_speed);
    analogWrite(S_LPWM_Output, 0);
    Serial.println("Turning Right");
  } else if (bt_turn == btLEFT || (LEFT && !lock_joystick)) {
    analogWrite(S_LPWM_Output, steer_speed);
    analogWrite(S_RPWM_Output, 0);
    Serial.println("Turning Left");
  } else {
    analogWrite(S_LPWM_Output, 0);
    analogWrite(S_RPWM_Output, 0);
  }

  if (bt_drive == btFWD || (FWD && !lock_joystick)) {
    analogWrite(D_RPWM_Output, drive_speed);
    analogWrite(D_LPWM_Output, 0);
    Serial.println("Driving FWD");
    Serial.println(drive_speed);
  } else if (bt_drive == btRVR || (RVR && !lock_joystick)) {
    analogWrite(D_LPWM_Output, drive_speed);
    analogWrite(D_RPWM_Output, 0);
    Serial.println("Driving RVR");
  } else {
    analogWrite(D_LPWM_Output, 0);
    analogWrite(D_RPWM_Output, 0);
  }

}