#pragma once

#include "driver/gpio.h"

#define FWD_PWM GPIO_NUM_13
#define RVR_PWM GPIO_NUM_14
#define RIGHT_PWM GPIO_NUM_15
#define LEFT_PWM GPIO_NUM_16
#define SPD_INPUT GPIO_NUM_33
#define FWD_INPUT GPIO_NUM_34
#define RVR_INPUT GPIO_NUM_35
#define RIGHT_INPUT GPIO_NUM_36
#define LEFT_INPUT GPIO_NUM_39

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

