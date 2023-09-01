#pragma once

#include <stdio.h>
#include<memory>
#include<string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/mcpwm_prelude.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "bluetooth_hid/esp_hid_host.h"

namespace GBG
{

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
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0A : MCPWM1A, fwd_pwm_pin);
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0B : MCPWM1B, rvr_pwm_pin);
        mcpwm_init(unit, timer, &pwm_config);    //Configure PWM0A & PWM0B with above settings
        gpio_set_direction(fwd_input_pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(fwd_input_pin, GPIO_PULLDOWN_ONLY);
        gpio_set_direction(rvr_input_pin, GPIO_MODE_INPUT);        
        gpio_set_pull_mode(rvr_input_pin, GPIO_PULLDOWN_ONLY);
    }

    void drive(bool drive_fwd, float duty_cycle)
    {
        // ESP_LOGI(name.c_str(), "driving %s", drive_fwd ? "pos" : "neg");
        mcpwm_set_signal_low(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A);
        mcpwm_set_duty(unit, timer, drive_fwd ? MCPWM_GEN_A : MCPWM_GEN_B, duty_cycle);
        mcpwm_set_duty_type(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void gpio_drive(float duty_cycle)
    {
        if(gpio_get_level(fwd_input_pin))
        {
            drive(true, duty_cycle);
        }else if(gpio_get_level(rvr_input_pin))
        {
            drive(false, duty_cycle);
        }else{
            stop();
        }
    }

    void stop()
    {
        // ESP_LOGI(name.c_str(), "stopping");
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_A);
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_B);
    }
};
}
