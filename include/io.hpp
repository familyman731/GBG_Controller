#pragma once

#include "driver/gpio.h"
#define FWD_PWM GPIO_NUM_16
#define RVR_PWM GPIO_NUM_4
#define RIGHT_PWM GPIO_NUM_2
#define LEFT_PWM GPIO_NUM_15
#define SPD_INPUT GPIO_NUM_13
#define FWD_INPUT GPIO_NUM_12
#define RVR_INPUT GPIO_NUM_14
#define RIGHT_INPUT GPIO_NUM_27
#define LEFT_INPUT GPIO_NUM_26

enum bt_messages_t{UP = 0x06, 
                   DOWN = 0x07, 
                   _LEFT = 0x08, 
                   _RIGHT = 0x09, 
                   A = 0x0a, 
                   B = 0x0d, 
                   X = 0x0b, 
                   Y = 0x0c, 
                   SELECT = 0x11, 
                   START = 12, 
                   L = 0x0e, 
                   R = 0x10, 
                   EMPTY = 0x00};

enum bt_drive_t {FWD = UP, RVR = DOWN, STOP = 0} bt_drive;
enum bt_turn_t {LEFT = _LEFT, RIGHT = _RIGHT, STRAIGHT = 0} bt_turn;
bool bt_in_control = false;