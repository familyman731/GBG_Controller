#include <stdio.h>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"


namespace GBG
{
struct motorconfig
{
    mcpwm_unit_t unit;
    mcpwm_io_signals_t signal;
    const int pwm_pin;
    mcpwm_timer_t timer;

    mcpwm_config_t pwm_config {.frequency = 1000, .cmpr_a = 0, .cmpr_b = 0, .duty_mode = MCPWM_DUTY_MODE_0, .counter_mode = MCPWM_UP_COUNTER};
};

class MotorController
{
    std::string name;
    motorconfig conf;

public:
    MotorController(std::string _name, motorconfig _conf, const int fwd_pin, const int rvr_pin):
        name{_name},
        conf{_conf}
    {
        mcpwm_init(conf.unit, conf.timer, &conf.pwm_config);    //Configure PWM0A & PWM0B with above settings
        mcpwm_gpio_init(conf.unit, conf.signal, fwd_pin);
        mcpwm_gpio_init(conf.unit, conf.signal, rvr_pin);
    }

    void drive(bool drive_fwd, float duty_cycle)
    {
        mcpwm_set_signal_low(conf.unit, conf.timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A);
        mcpwm_set_duty(conf.unit, conf.timer, drive_fwd ? MCPWM_GEN_A : MCPWM_GEN_B, duty_cycle);
        mcpwm_set_duty_type(conf.unit, conf.timer, drive_fwd ? MCPWM_GEN_B : MCPWM_GEN_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void stop()
    {
        mcpwm_set_signal_low(conf.unit, conf.timer, MCPWM_GEN_A);
        mcpwm_set_signal_low(conf.unit, conf.timer, MCPWM_GEN_B);
    }
};
}
