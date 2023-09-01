#pragma once

#include "io.hpp"

#include "MotorControl/MotorController.hpp"

namespace GBG{

class CarController
{
    std::string name;
    MotorController move;
    MotorController steer;
    float duty_cycle;
    // bool bt_in_control;
    static std::shared_ptr<CarController> car_controller;

public:
    CarController():
        name{"car_control"},
        move{"move", MCPWM_UNIT_0, FWD_PWM, RVR_PWM, FWD_INPUT, RVR_INPUT},
        steer{"steer", MCPWM_UNIT_1, RIGHT_PWM, LEFT_PWM, RIGHT_INPUT, LEFT_INPUT},
        duty_cycle{50}
        // bt_in_control{false}
    {
    }

    static std::shared_ptr<CarController> get_or_create()
    {
        if(car_controller)
        {
            return car_controller;
        }else{
            return car_controller = std::make_shared<CarController>();
        }
    }

    void drive()
    {
        if(pthread_mutex_lock(&bt_mutex)==0)
        {
            if(bt_in_control)
            {
                if(bt_drive)
                {
                    move.drive(bt_drive==FWD, duty_cycle);
                }else{
                    move.stop();
                }
                if(bt_turn)
                {
                    steer.drive(bt_turn==RIGHT, duty_cycle);
                }else{
                    steer.stop();
                }
                pthread_mutex_unlock(&bt_mutex);
            } else {
                pthread_mutex_unlock(&bt_mutex);
                move.gpio_drive(duty_cycle);
                steer.gpio_drive(duty_cycle);
            }
        }
    }
};
}