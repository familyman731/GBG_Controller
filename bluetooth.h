#pragma once

#include <Bluepad32.h>
#include <EEPROM.h>

#define EEPROM_SIZE 4

bool lock_joystick = false;
bool lock_joystick_button = false;
bool bt_in_control = false;

int drive_speed = 255;
int speed_change_inc = 51;

enum bt_messages_t{
                    UP = -512, 
                   DOWN = 511, 
                   _LEFT = -512, 
                   _RIGHT = 511, 
                   A = 0x02, 
                   B = 0x01, 
                   X = 0x08, 
                   Y = 0x04, 
                   SELECT = 0x02, 
                   START = 0x04, 
                   L = 0x10, 
                   R = 0x20, 
                   EMPTY = 0x00};

enum bt_drive_t {btFWD = UP, btRVR = DOWN, btSTOP = 0} bt_drive;
enum bt_turn_t {btLEFT = _LEFT, btRIGHT = _RIGHT, btSTRAIGHT = 0} bt_turn;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.

void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void init_bluetooth(){
	Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  // BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);

  EEPROM.begin(EEPROM_SIZE);
  drive_speed = EEPROM.read(0);
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}


void processGamepad(ControllerPtr ctl) {
    
  if (ctl->buttons() & X){
    lock_joystick_button = !lock_joystick_button;
    if(lock_joystick_button){
      Serial.println("Parent locked joystick");
    }else{
      Serial.println("Parent unlocked joystick");
    }
  }

  if (ctl->buttons() & R && drive_speed < 255){
    drive_speed += speed_change_inc;
    EEPROM.write(0, drive_speed);
    EEPROM.commit();
    Serial.print("Drive speed increased to ");
    Serial.println(drive_speed);
  }else if(ctl->buttons() & L && drive_speed > speed_change_inc){
    drive_speed -= speed_change_inc;
    EEPROM.write(0, drive_speed);
    EEPROM.commit();
    Serial.print("Drive speed decreased to ");
    Serial.println(drive_speed);
  }

  bt_drive = static_cast<bt_drive_t>(ctl->axisY());
  bt_turn = static_cast<bt_turn_t>(ctl->axisX());

  bt_in_control = !(bt_drive == EMPTY & bt_turn == EMPTY);

  lock_joystick = bt_in_control || lock_joystick_button;

  if(lock_joystick){
    Serial.println("joystick locked");
  }else{
    Serial.println("joystick unlocked");
  }

  // Another way to query controller data is by getting the buttons() function.
  // See how the different "dump*" functions dump the Controller info.
  dumpGamepad(ctl);
}


void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}