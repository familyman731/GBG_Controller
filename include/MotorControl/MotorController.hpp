#pragma once

#include <stdio.h>
#include<memory>
#include<string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "bluetooth_hid/esp_hid_host.h"

namespace GBG
{

#define SPD_INPUT GPIO_NUM_2
#define FWD_INPUT GPIO_NUM_3
#define RVR_INPUT GPIO_NUM_4
#define RIGHT_INPUT GPIO_NUM_5
#define LEFT_INPUT GPIO_NUM_6
#define FWD_PWM GPIO_NUM_7
#define RVR_PWM GPIO_NUM_8
#define RIGHT_PWM GPIO_NUM_9
#define LEFT_PWM GPIO_NUM_10

class MotorController
{
    std::string name;
    mcpwm_unit_t unit;
    mcpwm_timer_t timer;
    gpio_num_t fwd_input_pin;
    gpio_num_t rvr_input_pin;

public:
    MotorController(const std::string _name, mcpwm_unit_t _unit, const gpio_num_t fwd_pwm_pin, const gpio_num_t rvr_pwm_pin, const gpio_num_t _fwd_input_pin, const gpio_num_t _rvr_input_pin, mcpwm_config_t pwm_config = {.frequency = 1000, .cmpr_a = 0, .cmpr_b = 0, .duty_mode = MCPWM_DUTY_MODE_0, .counter_mode = MCPWM_UP_COUNTER}):
        name{_name},
        unit{_unit},
        timer{unit == MCPWM_UNIT_0 ? MCPWM_TIMER_0 : MCPWM_TIMER_1},
        fwd_input_pin{_fwd_input_pin},
        rvr_input_pin{_rvr_input_pin}

    {
        mcpwm_init(unit, timer, &pwm_config);    //Configure PWM0A & PWM0B with above settings
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0A : MCPWM1A, fwd_pwm_pin);
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0B : MCPWM1B, rvr_pwm_pin);

        gpio_set_direction(fwd_input_pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(fwd_input_pin, GPIO_PULLDOWN_ONLY);
        gpio_set_direction(rvr_input_pin, GPIO_MODE_INPUT);        
        gpio_set_pull_mode(rvr_input_pin, GPIO_PULLDOWN_ONLY);
    }

    void drive(bool drive_fwd, float duty_cycle)
    {
        mcpwm_set_signal_low(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A);
        mcpwm_set_duty(unit, timer, drive_fwd ? MCPWM_GEN_A : MCPWM_GEN_B, duty_cycle);
        mcpwm_set_duty_type(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void gpio_drive(float duty_cycle)
    {
        if(gpio_get_level(fwd_input_pin))
        {
            ESP_LOGI(name.c_str(), "driving pos");
            drive(true, duty_cycle);
        }else if(gpio_get_level(rvr_input_pin))
        {
            ESP_LOGI(name.c_str(), "driving neg");
            drive(false, duty_cycle);
        }else{
            stop();
        }
    }

    void stop()
    {
        ESP_LOGI(name.c_str(), "stopping");
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_A);
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_B);
    }
};

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
        duty_cycle{0.0}
        // bt_in_control{false}
    {
        ESP_LOGD(name.c_str(), "car initialized");
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
        ESP_LOGD(name.c_str(), "drive");
        if(pthread_mutex_lock(&bt_mutex)==0)
        {
            ESP_LOGD(name.c_str(), "mutex");
            if(bt_in_control)
            {
                ESP_LOGD(name.c_str(), "bt_in_control");
                if(bt_drive)
                {
                    move.drive(bt_drive==FWD, duty_cycle);
                }
                if(bt_turn)
                {
                    steer.drive(bt_turn==RIGHT, duty_cycle);
                }
                pthread_mutex_unlock(&bt_mutex);
            } else {
                ESP_LOGD(name.c_str(), "gpio");
                pthread_mutex_unlock(&bt_mutex);
                move.gpio_drive(duty_cycle);
                steer.gpio_drive(duty_cycle);
            }
        }
    }
};
}
