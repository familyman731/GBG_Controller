#pragma once

#include "io.hpp"
#include "MotorControl/MotorController.hpp"
#include "adc.hpp"

namespace GBG{

class CarController
{
    std::string name;
    MotorController move;
    MotorController steer;
    float duty_cycle;
    // bool bt_in_control;
    static std::shared_ptr<CarController> car_controller;
    adc_oneshot_unit_handle_t adc1_handle;

public:
    CarController():
        name{"car_control"},
        move{"move", MCPWM_UNIT_0, FWD_PWM, RVR_PWM, FWD_INPUT, RVR_INPUT},
        steer{"steer", MCPWM_UNIT_1, RIGHT_PWM, LEFT_PWM, RIGHT_INPUT, LEFT_INPUT},
        duty_cycle{50}
        // bt_in_control{false}
    {
        init_speed_control();
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

    void init_speed_control()
    {
        using namespace ADC;
        //-------------ADC1 Init---------------//
        
        adc_oneshot_unit_init_cfg_t init_config1 = {
            .unit_id = ADC_UNIT_1,
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

        //-------------ADC1 Config---------------//
        adc_oneshot_chan_cfg_t config = {
            .atten = ADC_ATTEN,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHAN, &config));

        //-------------ADC1 Calibration Init---------------//
        adc_cali_handle_t adc1_cali_handle = NULL;
        bool do_calibration1 = adc_calibration_init(&adc1_cali_handle);
    }

    float update_speed()
    {
        int adc_raw[2][10];
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHAN, &adc_raw[0][0]));
        return duty_cycle;
    }

    void drive()
    {
        update_speed();
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