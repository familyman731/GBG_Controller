#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"


namespace GBG
{
class MotorController
{
    mcpwm_unit_t unit;
    mcpwm_timer_t timer;

public:
    MotorController(mcpwm_unit_t _unit, const int fwd_pin, const int rvr_pin, mcpwm_config_t pwm_config = {.frequency = 1000, .cmpr_a = 0, .cmpr_b = 0, .duty_mode = MCPWM_DUTY_MODE_0, .counter_mode = MCPWM_UP_COUNTER}):
        unit{_unit},
        timer{unit == MCPWM_UNIT_0 ? MCPWM_TIMER_0 : MCPWM_TIMER_1}

    {
        mcpwm_init(unit, timer, &pwm_config);    //Configure PWM0A & PWM0B with above settings
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0A : MCPWM1A, fwd_pin);
        mcpwm_gpio_init(unit, unit == MCPWM_UNIT_0 ? MCPWM0B : MCPWM1B, rvr_pin);
    }

    void drive(bool drive_fwd, float duty_cycle)
    {
        mcpwm_set_signal_low(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A);
        mcpwm_set_duty(unit, timer, drive_fwd ? MCPWM_GEN_A : MCPWM_GEN_B, duty_cycle);
        mcpwm_set_duty_type(unit, timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void stop()
    {
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_A);
        mcpwm_set_signal_low(unit, timer, MCPWM_GEN_B);
    }
};

class CarController
{
    MotorController drive;
    MotorController steer;
    float duty_cycle;
    bool bt_in_control;

public:
    CarController():
        drive{MCPWM_UNIT_0, 15, 16},
        steer{MCPWM_UNIT_1, 17, 18},
        duty_cycle{0.0},
        bt_in_control{false}
    {
    }

    void bt_message(const uint8_t val)
    {
        bt_in_control = true;
        switch (val)
        {
        case 6:
            drive.drive(true, duty_cycle);
            break;
        
        default:
            break;
        } 
    }

    bool bt_in_control()
    {
        return bt_in_control;
    }
};
}
